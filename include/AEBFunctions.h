#ifndef AEBFUNCTIONS_H
#define AEBFUNCTIONS_H


// MACROS
#define OFF 0
#define ON 1
#define R 0
#define D 1
#define MARGIN_TIME 1.1
#define TimeReact 1.5
#define FactorKM_MS 3.6


// ENUM
enum enum_AEBStatus{NORMAL=0, ALERT=1, ALERT_BRAKE=2};
enum enum_AEBDecel{ZeroDecel = 0, PBdecel = 6, FBdecel = 8};//tive que alterara pra numero inteiro por causa do enum
enum enum_States{IDLE_STATE=0, FCW_STATE=1, PB_STATE=2, FB_STATE=3, RCW_STATE=4};
enum Gear {Drive =1, Reverse =0, Park = 2, Neutro = 3};
enum brakePedal{Pressed = 1, NotPressed = 0};


// PROTOTYPES
float *StoppingTimeCalculation(float VehSpeed,float roadCondCoeff, float driverTimeReact, float PBDecel, float FBDecel);
float TTCCalculation(float relativeDistance, float VehSpeed);
bool EnablerFunction(float TTC, enum Gear GearPos);
int visualAlertControl(int AEBStatus, enum Gear GearPos);
bool soundAlertControl(enum enum_AEBStatus AEBStatus);
int CollisionDecisionFunction(float TTC, int GearPos, float PBStoppingTime, float FBStoppingTime, int brakePedalStat, enum enum_States NextStateAEB);

#endif //AEBFUNCTIONS_H
