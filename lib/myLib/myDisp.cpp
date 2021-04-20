#include "myDisp.h"
#include "myWiFiSetting.h"

// Define a 4-digit display module. Pin suggestions:
TM1640 dispModule(16, 17, 9); // data, clock, 9 digits

void displayWholeTime(unsigned hh, unsigned mm, unsigned ss, unsigned nAry)
{
  displayTime(hh, DIG_HOUR, nAry, true);
  displayTime(mm, DIG_MIN, nAry, false);
  displayTime(ss, DIG_SEC, nAry, false);
  dispModule.setSegments(swapbit(hh), 6);
  dispModule.setSegments(swapbit(mm), 7);
  dispModule.setSegments(swapbit(ss), 8);
}

void displayTime(unsigned num, byte position, unsigned nAry, boolean dp)
{
  byte dig2 = 0;
  byte dig1 = 0;
  separateDigit(num, &dig2, &dig1, nAry);
  dispModule.sendChar(position, TM16XX_NUMBER_FONT[dig2], false);
  dispModule.sendChar(position + 1, TM16XX_NUMBER_FONT[dig1], dp);
}

bool separateDigit(unsigned num, byte *dig2, byte *dig1, unsigned nAry)
{
  if (nAry == 0)
  {
    return false;
  }
  *dig2 = 0;
  *dig1 = 0;
  if (num != 0)
  {
    *dig2 = num / nAry;
    *dig1 = num % nAry;
  }
  return true;
}

unsigned swapbit(unsigned x)
{
  unsigned r = 0;
  int b = 8;
  while (b--)
  {
    r <<= 1;
    r |= (x & 1);
    x >>= 1;
  }
  return r;
}

void sequenceModeChange(unsigned *nAryMode, bool withRTC)
{
  bool switchDone = false;
  if (digitalRead(TactSW) == 0)
  {
    dispModule.clearDisplay();
    if (connectionSuccess != true && withRTC == true)
    {
      dispModule.setDisplayToString("-rtc- ");
      delay(1000);
    }
    dispModule.setDisplayToString("n-Ary ");
    delay(1000);
    if (digitalRead(TactSW) == 1)
    {
      switchDone = true;
    }

    sequenceModeChangeSub(AryHEX, nAryMode, &switchDone);
    sequenceModeChangeSub(AryDEC, nAryMode, &switchDone);
    sequenceModeChangeSub(AryOCT, nAryMode, &switchDone);
  }
}

void sequenceModeChangeSub(byte Mode, unsigned *nAryMode, bool *switchDone)
{
  if (*switchDone != true)
  {
    dispMode(Mode);
    modeChange(Mode, nAryMode, switchDone);
  }
}

void dispMode(byte Mode)
{
  char dispString[7] = "N/A___";
  if (Mode == AryHEX)
  {
    strcpy(dispString, "HEX   ");
  }
  else if (Mode == AryDEC)
  {
    strcpy(dispString, "dEC   ");
  }
  else if (Mode == AryOCT)
  {
    strcpy(dispString, "OCT   ");
  }
  dispModule.clearDisplay();
  dispModule.setDisplayToString(dispString);
}

void modeChange(byte Mode, unsigned *nAryMode, bool *switchDone)
{
  int nAryWait = 0;
  for (nAryWait = 0; nAryWait < 1000; nAryWait++)
  {
    if (digitalRead(TactSW) == 1)
    {
      *nAryMode = Mode;
      *switchDone = true;
      break;
    }
    delay(1);
  }
}
