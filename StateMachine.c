#include <stdio.h>
#include <math.h>
#include <string.h>
#define OFF 0
#define ON 1
#define R 0
#define D 1
#define MARGIN_TIME 1.1
#define TimeReact 1.5
enum enum_AEBStatus{NORMAL=0, ALERT=1, ALERT_BRAKE=2};
enum enum_AEBDecel{ZeroDecel = 0, PBdecel = 6, FBdecel = 8};//tive que alterara pra numero inteiro por causa do enum
enum enum_States{IDLE_STATE=1, FCW_STATE=2, PB_STATE=3, FB_STATE=4, RCW_STATE=5};
enum Gear {Drive =1, Reverse =0};
enum brakePedal{Pressed = 1, NotPressed = 0};

/**
@brief Time to Collision (TTC) with the front or rear vehicle obstacle shall be determined by the "TTCCalculation" function

@param[in] relativeDistance float. Relative Distance between Vehicle and Road Obstacle (lead vehicle)
@param[in] VehSpeed float. Actual Vehicle Speed
 **/
float TimetoCollisionCalculation(float relativeDistance,float VehSpeed)
{   
    // Initialization of the variables
    float TTC;

    // While relativeDistance, VehSpeed are provided, the "TTCCalculation" function shall calculate TTC using following equation
    TTC = relativeDistance / VehSpeed;

    return TTC;
}

int CollisionDecisionFunction(float TTC, int GearPos, float PBStoppingTime, float FBStoppingTime, int brakePedalStat, enum enum_States NextStateAEB)
{
    // Initialization of the variables
    float CWarnTime = OFF;
    CWarnTime = PBStoppingTime + 1;

    printf("TTC = %f\n", TTC );
    printf("CWarnTime = %f\n", CWarnTime );
    printf("PBStoppingTime = %f\n",PBStoppingTime );
    printf("FBStoppingTime = %f\n",FBStoppingTime );
    printf("MARGIN_TIME*PBStoppingTime = %f\n",MARGIN_TIME*PBStoppingTime);
    printf("GearPos = %d\n",GearPos );
    printf("brakePedalStat = %d\n",brakePedalStat);
    
    if (TTC > MARGIN_TIME*CWarnTime) // Conditions to reach the IDLE state
    {
        NextStateAEB= IDLE_STATE;

    }
    else if(((TTC <= CWarnTime) && (GearPos == D))||((brakePedalStat == ON ) && (GearPos == D))||(brakePedalStat == ON)||(TTC > MARGIN_TIME*PBStoppingTime)) //Conditions to reach the FCW state
    {       
        NextStateAEB = FCW_STATE;

    }
    else if(((TTC <= PBStoppingTime) && (brakePedalStat == 0))||((TTC > MARGIN_TIME*FBStoppingTime) && (GearPos == D))) //  Conditions to reach the PBraking state
    {   
        NextStateAEB = PB_STATE;

    }
    else if((TTC <= FBStoppingTime)||((TTC <= FBStoppingTime) && (brakePedalStat == 0)))// Conditions to reach the FBraking state
    {   
        NextStateAEB = FB_STATE;

    }else if(((brakePedalStat == ON) && (GearPos == R))||((TTC <= CWarnTime) && (GearPos == R))||((TTC > MARGIN_TIME*PBStoppingTime) && (GearPos == R)))// Conditions to reach the RCW state
    {
        NextStateAEB = RCW_STATE;       

    }else
    {
        NextStateAEB = IDLE_STATE;

    }
    printf("\nNextStateAEB: %d\n", NextStateAEB);

    return NextStateAEB; // return the state of AEB based on the conditional conditions
}


/**
@brief Estimates current vehicle stopping time based on current vehicle speed, road conditions, adds driver reaction time

@param[in] VehSpeed float. Actual Vehicle Speed
@param[in] roadCondCoeff float. Percentage of friction coefficient of the road based on vehicle measured climate conditions (wet or dry)
@param[in] driverTimeReact float. Configurable additional Driver's Time Reaction to press Brake pedal 
@param[in] PBDecel float. Configurable Partial Braking Deceleration used by AEB as deceleration reference
@param[in] FBDecel float. Configurable Full Braking Deceleration used by AEB as deceleration reference
 **/
float *StoppingTimeCalculation(float VehSpeed,float roadCondCoeff, float driverTimeReact, float PBDecel, float FBDecel)
{   
    // Initialization of the variables
    static float StoppingTime[2];
    float PBStoppingTime = 0;
    float FBStoppingTime = 0;

    // Calculation for Stopping Time
    PBStoppingTime = (VehSpeed / (PBDecel * roadCondCoeff))  + driverTimeReact; // Formula for partial breaking time
    FBStoppingTime = (VehSpeed / (FBDecel * roadCondCoeff))  + driverTimeReact; // Formula for full breaking time

    StoppingTime[0] = PBStoppingTime;
    StoppingTime[1] = FBStoppingTime;

    return StoppingTime;
}

/**
@brief Visual alert to the driver about emergency collision event shall be controlled by the "visualAlertControl" function

@param[in] AEBStatus enum. Current AEB Status (internal command to activate Visual and Sound alert)
@param[in] GearPos enum. Actual Selected Gear Position
 **/
int VisualAlertFunction(int AEBStatus, enum Gear GearPos) //(1) Precisamos verificar se podemos utilizar "enum" como tipo de chamada da função VisualAlertFunction
{   
    // Initialization of the variables
    int visualCmd = 0 ;

    // Verification it´s status in order to activate the Visual Alert
    if(AEBStatus == NORMAL)
    {
        visualCmd = 0; // (2)Favor verificar todas as condições deste condicional. Estes valores recebidos pela variável VisualCmd
    }
    else if((AEBStatus == ALERT && GearPos == D))// fazem sentido?? como devemos escrever na variável de forma coreta? Att: Bruno Rosa -
    {
         visualCmd = 1;
    }
    else if((AEBStatus == ALERT && GearPos == R))
    {
        visualCmd = 2;
    }
    else if((AEBStatus == ALERT_BRAKE && GearPos == D))// fazem sentido?? como devemos escrever na variável de forma coreta? Att: Bruno Rosa -
    {
         visualCmd = 1;
    }
    else if((AEBStatus == ALERT_BRAKE && GearPos == R))
    {
        visualCmd = 2;
    }
    else
    {
        visualCmd = 0; 
    }

    return visualCmd;
}

int main(int argc, char *argv[])
{
    printf("--------Parametros Iniciais de Simulacao------\n");
    enum brakePedal brakePedalStat = NotPressed; 
    enum Gear GearPos = 1;
    float PBStoppingTime = 1.19;
    float FBStoppingTime = 1.18;


    float relativeDistance= 10.0;
    float VehSpeed = 30;
    float roadCondCoeff = 0.625;
    float *StoppingTime; 
    float TTC; 
    int AEBState;
    int VisualCMD;
    enum enum_AEBStatus AEBStatus;
    enum enum_AEBDecel AEBDecelRef;
    enum enum_AEBStatus NextState = IDLE_STATE;// Nao vi necessidade de colocar..o programa estará no loop, entao a todo momento está vendo as condicoes do CollisionDecisionFunction
    
    TTC = TimetoCollisionCalculation(relativeDistance,VehSpeed);
    StoppingTime = StoppingTimeCalculation(VehSpeed,roadCondCoeff, TimeReact , PBdecel, FBdecel);
    AEBState = CollisionDecisionFunction(TTC, GearPos, *(StoppingTime), *(StoppingTime+1), brakePedalStat, NextState);
 
    switch (AEBState)
    {
    case IDLE_STATE:
        AEBStatus = NORMAL;
        AEBDecelRef = ZeroDecel;
        NextState = FCW_STATE;
        break;
    case FCW_STATE:      
        AEBStatus = ALERT;
        AEBDecelRef = ZeroDecel;
        break;
    case PB_STATE:
        AEBStatus = ALERT_BRAKE;
        AEBDecelRef = PBdecel; // Value for the Partial Breaking between [1 and 6]        
        break;
    case FB_STATE:
        AEBStatus = ALERT_BRAKE;
        AEBDecelRef = FBdecel; // Value for the Full Breaking between [6 and 9]
        break;
    case RCW_STATE:
        AEBStatus = ALERT;
        AEBDecelRef = ZeroDecel;
        break;
    default:
        break;
    }
    VisualCMD = VisualAlertFunction(AEBStatus, GearPos);

    //printf("\nAEBState: %d", AEBState);
    printf("\n-----------Saidas----------\n");
    printf("\nAEBStatus: %d", AEBStatus);
    printf("\nAEBDecelRef: %d\n", AEBDecelRef);
    printf("\n VisualCMD: %d\n",  VisualCMD);

    return 0;
}
