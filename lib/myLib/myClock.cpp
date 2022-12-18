#include "myClock.h"
#include "myDisp.h"

RTC_DS3231 rtc;

/* struct tm
{
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
}; */

struct tm timeinfo;

bool initRTC()
{
    // RTC初期化
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        // dispModule.setDisplayToString("no rtc");
        delay(1000);
        return false;
    }
    else
    {
        Serial.println("RTC Initialized");
        return true;
    }
}

void updateRTC(tm timeinfo)
{
    rtc.adjust(DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
    Serial.println("RTC adjusted");
    dispModule.setupDisplay(true, 7); // sometimes noise on the line may change the intensity level
    dispModule.setDisplayToString("rtc up");
    delay(1000);
}

void adjustByNTP(bool withRTC)
{
    // dispModule.clearDisplay();
    dispModule.setupDisplay(true, 7); // sometimes noise on the line may change the intensity level
    dispModule.setDisplayToString("AdjUST#  ");
    // dispModule.setSegments(swapbit(255), 6);
    // dispModule.setSegments(swapbit(0), 7);
    // dispModule.setSegments(swapbit(0), 8);
    delay(1000);
    // dispModule.clearDisplay();

    //configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp"); // NTPの設定
    configTzTime("JST-9", "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
    Serial.print("[NTP Svr] Connecting.");
    u8_t retryCount = 0;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET ) {
        Serial.print(".");
        //dispModule.setDisplayToString("rEtrY    ");
        delay(1000); // 1秒毎にリトライ
        retryCount++;
        if (retryCount > 10){
            dispModule.setDisplayToString("ntPErr");
            delay(5000);
            break;
        }
    }
    dispModule.setDisplayToString("    ok###");
    delay(1000);
    while (!getLocalTime(&timeinfo))
    {
        delay(500);
    }
    if (withRTC == true && (getLocalTime(&timeinfo) == true))
    {
        updateRTC(timeinfo);
    }
}

bool getLocalTimebyNTPMode(unsigned *currentHour, unsigned *currentMin, unsigned *currentSec)
{
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("getLocalTime Error");
        dispModule.setDisplayToString("Error ");
        return false;
    }
    else
    {
        *currentHour = timeinfo.tm_hour;
        *currentMin = timeinfo.tm_min;
        *currentSec = timeinfo.tm_sec;
        return true;
    }
}
bool getLocalTimebyRTCMode(unsigned *currentHour, unsigned *currentMin, unsigned *currentSec)
{
    DateTime now = rtc.now();
    *currentHour = now.hour();
    *currentMin = now.minute();
    *currentSec = now.second();
    return true;
}