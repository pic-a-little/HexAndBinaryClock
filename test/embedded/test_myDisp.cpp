#include <unity.h>
#include <Arduino.h>
#include <myDisp.h>

#define TactSW (4)

void test_swapbit()
{
    TEST_ASSERT_EQUAL(0b00000001, swapbit(0b10000000));
}
void test_swapbit2()
{
    TEST_ASSERT_EQUAL(0b00000010, swapbit(0b01000000));
}
void test_swapbit3()
{
    TEST_ASSERT_EQUAL(0b00000100, swapbit(0b00100000));
}
void test_swapbit4()
{
    TEST_ASSERT_EQUAL(0b00001000, swapbit(0b00010000));
}
void test_swapbit5()
{
    TEST_ASSERT_EQUAL(0b00010000, swapbit(0b00001000));
}
void test_swapbit6()
{
    TEST_ASSERT_EQUAL(0b00100000, swapbit(0b00000100));
}
void test_swapbit7()
{
    TEST_ASSERT_EQUAL(0b01000000, swapbit(0b00000010));
}
void test_swapbit8()
{
    TEST_ASSERT_EQUAL(0b10000000, swapbit(0b00000001));
}
void test_separateDigit(){
    byte dig2;
    byte dig1;
    TEST_ASSERT_FALSE(separateDigit(16, &dig2, &dig1, 0))
}
void test_separateDigit2(){
    byte dig2;
    byte dig1;
    separateDigit(16, &dig2, &dig1, 10);
    TEST_ASSERT_EQUAL(1,dig2);
}
void test_separateDigit3(){
    byte dig2;
    byte dig1;
    separateDigit(16, &dig2, &dig1, 10);
    TEST_ASSERT_EQUAL(6,dig1);
}
void test_separateDigit4(){
    byte dig2;
    byte dig1;
    separateDigit(59, &dig2, &dig1, 16);
    TEST_ASSERT_EQUAL(3,dig2);
}
void test_separateDigit5(){
    byte dig2;
    byte dig1;
    separateDigit(59, &dig2, &dig1, 16);
    TEST_ASSERT_EQUAL(11,dig1);
}
void test_separateDigit6(){
    byte dig2;
    byte dig1;
    separateDigit(59, &dig2, &dig1, 10);
    TEST_ASSERT_EQUAL(5,dig2);
}
void test_separateDigit7(){
    byte dig2;
    byte dig1;
    separateDigit(59, &dig2, &dig1, 10);
    TEST_ASSERT_EQUAL(9,dig1);
}
void test_separateDigit8(){
    byte dig2;
    byte dig1;
    separateDigit(59, &dig2, &dig1, 8);
    TEST_ASSERT_EQUAL(7,dig2);
}
void test_separateDigit9(){
    byte dig2;
    byte dig1;
    separateDigit(59, &dig2, &dig1, 8);
    TEST_ASSERT_EQUAL(3,dig1);
}

void test_modeChange(){
    byte Mode = 10;
    unsigned nAryMode;
    bool switchDone;
    modeChange(Mode, &nAryMode, &switchDone);
    TEST_ASSERT_EQUAL(true,switchDone);
}
void test_modeChange2(){
    byte Mode = 10;
    unsigned nAryMode;
    bool switchDone;
    modeChange(Mode, &nAryMode, &switchDone);
    TEST_ASSERT_EQUAL(10,nAryMode);
}


void setup(){
    pinMode(TactSW, INPUT_PULLUP);
}


void loop(){
    UNITY_BEGIN();
    RUN_TEST(test_swapbit);
    RUN_TEST(test_swapbit2);
    RUN_TEST(test_swapbit3);
    RUN_TEST(test_swapbit4);
    RUN_TEST(test_swapbit5);
    RUN_TEST(test_swapbit6);
    RUN_TEST(test_swapbit7);
    RUN_TEST(test_swapbit8);

    RUN_TEST(test_separateDigit);
    RUN_TEST(test_separateDigit2);
    RUN_TEST(test_separateDigit3);
    RUN_TEST(test_separateDigit4);
    RUN_TEST(test_separateDigit5);
    RUN_TEST(test_separateDigit6);
    RUN_TEST(test_separateDigit7);
    RUN_TEST(test_separateDigit8);
    RUN_TEST(test_separateDigit9);

    RUN_TEST(test_modeChange);
    RUN_TEST(test_modeChange2);



    UNITY_END();
}