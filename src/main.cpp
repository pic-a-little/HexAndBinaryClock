#include <Arduino.h>
#include "main.h"


#ifdef debug
volatile boolean otaFlag = true;
#else
volatile boolean otaFlag = false;
#endif


int WiFiMode = CLIENTMODE;
bool withRTC = false;

void setup(void)
{
  // dispModule.setupDisplay(true, 7); // sometimes noise on the line may change the intensity level
  byte i;  
  Serial.begin(115200);
 for(i = 0 ; i <10 ; i ++){
    dispModule.clearDisplay();
    delay(50);
  }
  withRTC = initRTC();
  getSPIFFSSetting();
  pinMode(TactSW, INPUT_PULLUP);

  for(i = 0 ; i <20 ; i ++){
    dispModule.setupDisplay(true, 7); // sometimes noise on the line may change the intensity level
    dispModule.setDisplayToString("boot    #");
    delay(50);
  }

  Serial.println("Booting");
  if (digitalRead(TactSW) == 0)
  { //Set WPS mode
    // dispModule.clearDisplay(); 
    dispModule.setupDisplay(true, 7); // sometimes noise on the line may change the intensity level
    dispModule.setDisplayToString("AP-nEt  #");
    // dispModule.setSegments(swapbit(0), 6);
    // dispModule.setSegments(swapbit(0), 7);
    // dispModule.setSegments(swapbit(255), 8);
    WiFiMode = APMODE;
    setAccessPointMode();
    delay(2000);
  }
  else
  { //Set ClientMode
    WiFiMode = CLIENTMODE;
    setClientMode(&WiFiMode);
    if(WiFiMode == APMODE){//初回時などSPIFFS情報が破損している場合、APModeで繋ぎ直すため表示を更新
      // dispModule.clearDisplay();
      dispModule.setupDisplay(true, 7); // sometimes noise on the line may change the intensity level
      dispModule.setDisplayToString("AP-nEt  #");
      // dispModule.setSegments(swapbit(0), 6);
      // dispModule.setSegments(swapbit(0), 7);
      // dispModule.setSegments(swapbit(255), 8);
    }
    if (WiFiMode == CLIENTMODE && connectionSuccess == true)
    {
      adjustByNTP(withRTC);
      // dispModule.clearDisplay();
    }
  }
}

int lastSec;
unsigned nAryMode = AryHEX;
unsigned currentHour, currentMin, currentSec;
bool getLocalTimeSucceed = false;


void loop(void)
{
  if (WiFiMode == APMODE)
  {
    // Handle incoming connections
    server.handleClient();
    delay(100);
  }
  else //client mode
  {
    if (connectionSuccess == true && otaFlag == true)
    {
      ArduinoOTA.handle();
    }
    if (connectionSuccess == true || (connectionSuccess != true && withRTC == true))
    {
      updateDisplayWithModeChangeCheck();
    }
    delay(10);
  }
}

void updateDisplayWithModeChangeCheck()
{
  dispModule.setupDisplay(true, 7); // sometimes noise on the line may change the intensity level

  if (connectionSuccess == true)
  {
    getLocalTimeSucceed = getLocalTimebyNTPMode(&currentHour, &currentMin, &currentSec);
  }
  else if (connectionSuccess != true && withRTC == true)
  {
    getLocalTimeSucceed = getLocalTimebyRTCMode(&currentHour, &currentMin, &currentSec);
  }
  if (lastSec != currentSec && getLocalTimeSucceed == true)
  {
    displayWholeTime(currentHour, currentMin, currentSec, nAryMode);
    lastSec = currentSec;
  }
  sequenceModeChange(&nAryMode, withRTC);
}