#include <stdio.h>
#include <assert.h>
#include "AEBFunctions.h"

void test_StoppingTimeCalculation() {
  // Test case 1
  float VehSpeed = 50.0;
  float roadCondCoeff = 0.8;
  float driverTimeReact = 1.0;
  float PBDecel = 5.85;
  float FBDecel = 7.92;

  float *StoppingTime = StoppingTimeCalculation(VehSpeed, roadCondCoeff, driverTimeReact, PBDecel, FBDecel);

  assert(StoppingTime[0] == 8.66);
  assert(StoppingTime[1] == 6.33);

  free(StoppingTime);
}

void test_SoundAlertFunction() {
  // Test case 1
  enum AEBStatus AEBStatus = ALERT;
  bool soundCmd = SoundAlertFunction(AEBStatus);

  assert(soundCmd == true);

  // Test case 2
  AEBStatus = ALERT_BRAKE;
  soundCmd = SoundAlertFunction(AEBStatus);

  assert(soundCmd == true);

  // Test case 3
  AEBStatus = OFF;
  soundCmd = SoundAlertFunction(AEBStatus);

  assert(soundCmd == false);
}

void test_VisualAlertFunction() {
  // Test case 1
  enum AEBStatus AEBStatus = ALERT;
  enum GearPos GearPos = P;
  enum visualCmd visualCmd = VisualAlertFunction(AEBStatus, GearPos);

  assert(visualCmd == 1);

  // Test case 2
  AEBStatus = ALERT_BRAKE;
  GearPos = N;
  visualCmd = VisualAlertFunction(AEBStatus, GearPos);

  assert(visualCmd == 1);

  // Test case 3
  AEBStatus = OFF;
  GearPos = D;
  visualCmd = VisualAlertFunction(AEBStatus, GearPos);

  assert(visualCmd == 0);
}

void test_EnablerFunction() {
  // Test case 1
  float VehSpeed = 10.0;
  enum GearPos GearPos = R;
  bool EnablerStatus = EnablerFunction(VehSpeed, GearPos);

  assert(EnablerStatus == true);

  // Test case 2
  VehSpeed = 20.0;
  GearPos = D;
  EnablerStatus = EnablerFunction(VehSpeed, GearPos);

  assert(EnablerStatus == true);

  // Test case 3
  VehSpeed = 0.0;
  GearPos = P;
  EnablerStatus = EnablerFunction(VehSpeed, GearPos);

  assert(EnablerStatus == false);
}

void test_TimetoCollisionCalculation() {
  // Test case 1
  float relativeDistance = 50.0;
  float VehSpeed = 50.0;
  float TTC = TimetoCollisionCalculation(relativeDistance, VehSpeed);

  assert(TTC == 1.0);

  // Test case 2
  relativeDistance = 100.0;
  VehSpeed = 100.0;
  TTC = TimetoCollisionCalculation(relativeDistance, VehSpeed);

  assert(TTC == 1.0);

  // Test case 3
  relativeDistance = 0.0;
  VehSpeed = 0.0;
  TTC = TimetoCollisionCalculation(relativeDistance, VehSpeed);

  assert(TTC == 0.0);
}

int main() {
  test_StoppingTimeCalculation();
  test_SoundAlertFunction();
  test_VisualAlertFunction();
  test_EnablerFunction();
  test_TimetoCollisionCalculation();

  printf("All tests passed!\n");

  return 0;
}