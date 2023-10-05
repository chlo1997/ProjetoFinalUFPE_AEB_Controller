#include <stdio.h>
#include "../Unity/unity.h"
#include "../Unity/unity_internals.h"
#include "../Unity/unity.c"
#include "../include/AEBFunctions.h"
#include "../src/AEBFunctions.c"
#include <stdbool.h>

void setUp(){};

void tearDown(){};

//test case 1
void test_saf1(){
  enum enum_AEBStatus AEBStatus = ALERT;
  bool soundCmd = soundAlertControl(AEBStatus);

  TEST_ASSERT_TRUE(soundCmd);

}

//test case 2
void test_saf2(){
  enum enum_AEBStatus AEBStatus = ALERT_BRAKE;
  bool soundCmd = soundAlertControl(AEBStatus);

  TEST_ASSERT_TRUE(soundCmd);

}

//test case 3
void test_saf3(){
  enum enum_AEBStatus AEBStatus = NORMAL;
  bool soundCmd = soundAlertControl(AEBStatus);

  TEST_ASSERT_FALSE(soundCmd);

}
//int AEBStatus, enum Gear GearPos
//test case 4 
void test_vaf1(){
  int AEBStatus = NORMAL;
  enum Gear GearPos = Park; 
  int visualCmd = visualAlertControl(AEBStatus, GearPos);
  TEST_ASSERT_EQUAL(0, visualCmd);
}

//test case 5 
void test_vaf2(){
  int AEBStatus = NORMAL;
  enum Gear GearPos = Neutro; 
  int visualCmd = visualAlertControl(AEBStatus, GearPos);
  TEST_ASSERT_EQUAL(0, visualCmd);
}

//test case 6 
void test_vaf3(){
  int AEBStatus = NORMAL;
  enum Gear GearPos = Reverse; 
  int visualCmd = visualAlertControl(AEBStatus, GearPos);
  TEST_ASSERT_EQUAL(0, visualCmd);
}

//test case 7 
void test_vaf4(){
  int AEBStatus = NORMAL;
  enum Gear GearPos = Drive; 
  int visualCmd = visualAlertControl(AEBStatus, GearPos);
  TEST_ASSERT_EQUAL(0, visualCmd);
}

//test case 8 
void test_vaf5(){
  int AEBStatus = ALERT;
  enum Gear GearPos = Park;
  int visualCmd = visualAlertControl(AEBStatus, GearPos);
  TEST_ASSERT_EQUAL(0, visualCmd);
}

//test case 9 
void test_vaf6(){
  int AEBStatus = ALERT;
  enum Gear GearPos = Neutro;
  int visualCmd = visualAlertControl(AEBStatus, GearPos);
  TEST_ASSERT_EQUAL(0, visualCmd);
}

//test case 10 
void test_vaf7(){
  int AEBStatus = ALERT;
  enum Gear GearPos = Reverse;
  int visualCmd = visualAlertControl(AEBStatus, GearPos);
  TEST_ASSERT_EQUAL(2, visualCmd);
}

//test case 11 
void test_vaf8(){
  int AEBStatus = ALERT;
  enum Gear GearPos = Drive;
  int visualCmd = visualAlertControl(AEBStatus, GearPos);
  TEST_ASSERT_EQUAL(1, visualCmd);
}

//test case 12 
void test_vaf9(){
  int AEBStatus = ALERT_BRAKE;
  enum Gear GearPos = Park;
  int visualCmd = visualAlertControl(AEBStatus, GearPos);
  TEST_ASSERT_EQUAL(0, visualCmd);
}

//test case 13 
void test_vaf10(){
  int AEBStatus = ALERT_BRAKE;
  enum Gear GearPos = Neutro;
  int visualCmd = visualAlertControl(AEBStatus, GearPos);
  TEST_ASSERT_EQUAL(0, visualCmd);
}

//test case 14 
void test_vaf11(){
  int AEBStatus = ALERT_BRAKE;
  enum Gear GearPos = Reverse;
  int visualCmd = visualAlertControl(AEBStatus, GearPos);
  TEST_ASSERT_EQUAL(2, visualCmd);
}

//test case 15 
void test_vaf12(){
  int AEBStatus = ALERT_BRAKE;
  enum Gear GearPos = Drive;
  int visualCmd = visualAlertControl(AEBStatus, GearPos);
  TEST_ASSERT_EQUAL(1, visualCmd);
}

//test case 16
void test_TTCCalc1(){

  float relativeDistance = 100;
  float VehSpeed = 14;
  float TTC = TTCCalculation(relativeDistance, VehSpeed);
  
  TEST_ASSERT_EQUAL_FLOAT(7.142857, TTC);
  
}

//test case 17
void test_TTCCalc2(){

  float relativeDistance = 1;
  float VehSpeed = 1;
  float TTC = TTCCalculation(relativeDistance, VehSpeed);
  
  TEST_ASSERT_EQUAL_FLOAT(1, TTC);
  
}

//test case 18
void test_enab1(){

  float TTC = 3;
  enum Gear GearPos = Drive;
  bool EnablerStatus = EnablerFunction(TTC, GearPos);

  TEST_ASSERT_TRUE(EnablerStatus);
  
}

//test case 19
void test_enab2(){

  float TTC = 1;
  enum Gear GearPos = Reverse;
  bool EnablerStatus = EnablerFunction(TTC, GearPos);

  TEST_ASSERT_TRUE(EnablerStatus);
  
}

//test case 20
void test_enab3(){

  float TTC = 3;
  enum Gear GearPos = Reverse;
  bool EnablerStatus = EnablerFunction(TTC, GearPos);

  TEST_ASSERT_FALSE(EnablerStatus);
  
}

//test case 21
void test_enab4(){

  float TTC = 8;
  enum Gear GearPos = Drive;
  bool EnablerStatus = EnablerFunction(TTC, GearPos);

  TEST_ASSERT_FALSE(EnablerStatus);
  
}

//test case 22
void test_stc1(){

  float VehSpeed = 14;
  float roadCondCoeff = 1;
  float driverTimeReact = 2;
  float PBDecel = 6;
  float FBDecel = 9;
  float StoppingTime = *StoppingTimeCalculation(VehSpeed, roadCondCoeff, driverTimeReact, PBDecel, FBDecel);

  TEST_ASSERT_EQUAL_FLOAT(4.333334, StoppingTime);
  
}

//test case 23
void test_stc2(){

  float VehSpeed = 1;
  float roadCondCoeff = 0.625;
  float driverTimeReact = 1;
  float PBDecel = 1;
  float FBDecel = 6.5;
  float StoppingTime = *StoppingTimeCalculation(VehSpeed, roadCondCoeff, driverTimeReact, PBDecel, FBDecel);

  TEST_ASSERT_EQUAL_FLOAT(2.6, StoppingTime);
  
}

//test case 24
void test_cdf1(){

  float TTC = 100;
  int GearPos = 1;
  float PBStoppingTime = 100;
  float FBStoppingTime = 100;
  int brakePedalStat = 1;
  enum enum_States CurrentStateAEB = IDLE_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(1, NextStateAEB);
  
}

//test case 25
void test_cdf2(){

  float TTC = 1;
  int GearPos = 0;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 1;
  enum enum_States CurrentStateAEB = IDLE_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(4, NextStateAEB);
  
}

//test case 26
void test_cdf3(){

  float TTC = 150;
  int GearPos = 3;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 1;
  enum enum_States CurrentStateAEB = IDLE_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(0, NextStateAEB);
  
}

//test case 27
void test_cdf4(){

  float TTC = 100;
  int GearPos = 1;
  float PBStoppingTime = 100;
  float FBStoppingTime = 100;
  int brakePedalStat = 0;
  enum enum_States CurrentStateAEB = FCW_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(2, NextStateAEB);
  
}

//test case 28
void test_cdf5(){

  float TTC = 100;
  int GearPos = 0;
  float PBStoppingTime = 5;
  float FBStoppingTime = 5;
  int brakePedalStat = 1;
  enum enum_States CurrentStateAEB = FCW_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(0, NextStateAEB);
  
}

//test case 29
void test_cdf6(){

  float TTC = 10;
  int GearPos = 3;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 1;
  enum enum_States CurrentStateAEB = FCW_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(1, NextStateAEB);
  
}

//test case 30
void test_cdf7(){

  float TTC = 10;
  int GearPos = 3;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 0;
  enum enum_States CurrentStateAEB = PB_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(3, NextStateAEB);
  
}

//test case 31
void test_cdf8(){

  float TTC = 10;
  int GearPos = 3;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 1;
  enum enum_States CurrentStateAEB = PB_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(3, NextStateAEB);
  
}

//test case 32
void test_cdf9(){

  float TTC = 100;
  int GearPos = 3;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 0;
  enum enum_States CurrentStateAEB = PB_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(1, NextStateAEB);
  
}

//test case 33
void test_cdf10(){

  float TTC = 10;
  int GearPos = 3;
  float PBStoppingTime = 50;
  float FBStoppingTime = 5;
  int brakePedalStat = 1;
  enum enum_States CurrentStateAEB = PB_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(1, NextStateAEB);
  
}

//test case 34
void test_cdf11(){

  float TTC = 10;
  int GearPos = 3;
  float PBStoppingTime = 50;
  float FBStoppingTime = 5;
  int brakePedalStat = 0;
  enum enum_States CurrentStateAEB = PB_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(2, NextStateAEB);
  
}

//test case 35
void test_cdf12(){

  float TTC = 10;
  int GearPos = 3;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 0;
  enum enum_States CurrentStateAEB = FB_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(3, NextStateAEB);
  
}

//test case 36
void test_cdf13(){

  float TTC = 10;
  int GearPos = 3;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 1;
  enum enum_States CurrentStateAEB = FB_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(3, NextStateAEB);
  
}

//test case 37
void test_cdf14(){

  float TTC = 100;
  int GearPos = 0;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 0;
  enum enum_States CurrentStateAEB = FB_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(4, NextStateAEB);
  
}

//test case 38
void test_cdf15(){

  float TTC = 100;
  int GearPos = 0;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 1;
  enum enum_States CurrentStateAEB = FB_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(4, NextStateAEB);
  
}

//test case 39
void test_cdf16(){

  float TTC = 100;
  int GearPos = 1;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 0;
  enum enum_States CurrentStateAEB = FB_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(2, NextStateAEB);
  
}

//test case 40
void test_cdf17(){

  float TTC = 99;
  int GearPos = 1;
  float PBStoppingTime = 50;
  float FBStoppingTime = 90;
  int brakePedalStat = 1;
  enum enum_States CurrentStateAEB = FB_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(1, NextStateAEB);
  
}

//test case 41
void test_cdf18(){

  float TTC = 100;
  int GearPos = 2;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 0;
  enum enum_States CurrentStateAEB = FB_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(3, NextStateAEB);
  
}

//test case 42
void test_cdf19(){

  float TTC = 10;
  int GearPos = 2;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 0;
  enum enum_States CurrentStateAEB = RCW_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(3, NextStateAEB);
  
}

//test case 43
void test_cdf20(){

  float TTC = 100;
  int GearPos = 2;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 0;
  enum enum_States CurrentStateAEB = RCW_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(0, NextStateAEB);
  
}

//test case 44
void test_cdf21(){

  float TTC = 55;
  int GearPos = 2;
  float PBStoppingTime = 50;
  float FBStoppingTime = 50;
  int brakePedalStat = 0;
  enum enum_States CurrentStateAEB = RCW_STATE;
  int NextStateAEB = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, brakePedalStat, CurrentStateAEB);

  TEST_ASSERT_EQUAL_INT(4, NextStateAEB);
  
}

//main
int main(){
  
  UNITY_BEGIN();
  RUN_TEST(test_saf1);
  RUN_TEST(test_saf2);
  RUN_TEST(test_saf3);
  RUN_TEST(test_vaf1);
  RUN_TEST(test_vaf2);
  RUN_TEST(test_vaf3);
  RUN_TEST(test_vaf4);
  RUN_TEST(test_vaf5);
  RUN_TEST(test_vaf6);
  RUN_TEST(test_vaf7);
  RUN_TEST(test_vaf8);
  RUN_TEST(test_vaf9);
  RUN_TEST(test_vaf10);
  RUN_TEST(test_vaf11);
  RUN_TEST(test_vaf12);
  RUN_TEST(test_TTCCalc1);
  RUN_TEST(test_TTCCalc2);
  RUN_TEST(test_enab1);
  RUN_TEST(test_enab2);
  RUN_TEST(test_enab3);
  RUN_TEST(test_enab4);
  RUN_TEST(test_stc1);
  RUN_TEST(test_stc2);
  RUN_TEST(test_cdf1);
  RUN_TEST(test_cdf2);
  RUN_TEST(test_cdf3);
  RUN_TEST(test_cdf4);
  RUN_TEST(test_cdf5);
  RUN_TEST(test_cdf6);
  RUN_TEST(test_cdf7);
  RUN_TEST(test_cdf8);
  RUN_TEST(test_cdf9);
  RUN_TEST(test_cdf10);
  RUN_TEST(test_cdf11);
  RUN_TEST(test_cdf12);
  RUN_TEST(test_cdf13);
  RUN_TEST(test_cdf14);
  RUN_TEST(test_cdf15);
  RUN_TEST(test_cdf16);
  RUN_TEST(test_cdf17);
  RUN_TEST(test_cdf18);
  RUN_TEST(test_cdf19);
  RUN_TEST(test_cdf20);
  RUN_TEST(test_cdf21);
  
  return UNITY_END();
}



