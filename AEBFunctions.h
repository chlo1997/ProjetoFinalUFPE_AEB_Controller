#ifndef AEBFunctions_H
#define AEBFunctions_H

float *StoppingTimeCalculation(float VehSpeed,float roadCondCoeff, float driverTimeReact, float PBDecel, float FBDecel);
bool SoundAlertFunction(enum AEBStatus);
enum VisualAlertFunction(enum AEBStatus, enum GearPos);
bool EnablerFunction(float VehSpeed, enum GearPos);
float TimetoCollisionCalculation(float relativeDistance,float VehSpeed);
float *CollisionDecisionFunction(float TTC, enum GearPos, float PBStoppingTime, float FBStoppingTime, enum brakePedalStat);

#endif
