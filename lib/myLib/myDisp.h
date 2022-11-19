#pragma once

#include <SPI.h>
#include <TM1640.h>

#define TactSW (4)

#define AryDEC (10)
#define AryHEX (16)
#define AryOCT (8)

//TM1640の各桁の開始番号
#define DIG_HOUR (0)
#define DIG_MIN (2)
#define DIG_SEC (4)

extern TM1640 dispModule;

//数字＋バイナリで時、分、秒を表示する
extern void displayWholeTime(unsigned hh,unsigned mm, unsigned ss, unsigned nAry);

//バイナリ表示のためにビットオーダーを反転する
unsigned swapbit(unsigned x);

//数値表示の時、分、秒　各桁を表示するサブルーチン
void displayTime(unsigned num, byte position, unsigned nAry,boolean dp);
bool separateDigit(unsigned num, byte *dig2, byte *dig1, unsigned nAry);

//ボタン押下時に16進、10進、8進を切り替えるUI表示
extern void sequenceModeChange(unsigned *nAryMode , bool withRTC);
// extern void modeChangeCheck(unsigned *nAryMode);

void sequenceModeChangeSub(byte Mode, unsigned *nAryMode, bool *switchDone);
void dispMode(byte Mode);
void modeChange(byte Mode,unsigned *nAryMode, bool *switchDone);