#include <RTClib.h>
#include <Wire.h>
#include <esp_sntp.h>

extern RTC_DS3231 rtc;
extern bool withRTC;

extern struct tm timeinfo;

//RTCの接続をチェックし、あればInitする
extern bool initRTC();

//NTPの時刻でSYSTEMClockを同期する（RTCがあれば、RTCもNTPの時刻に同期する）
extern void adjustByNTP(bool withRTC);

//RTCの現在時刻をtimeinfoで同期する
extern void updateRTC(tm timeinfo);

//現在時刻をNTPに同期したSYSTEMClockから取得する
extern bool getLocalTimebyNTPMode(unsigned *currentHour, unsigned *currentMin, unsigned *currentSec);
//現在時刻をRTCから取得する
extern bool getLocalTimebyRTCMode(unsigned *currentHour, unsigned *currentMin, unsigned *currentSec);