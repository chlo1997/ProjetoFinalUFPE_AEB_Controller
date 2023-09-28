#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#define OFF 0
#define ON 1
#define R 0
#define D 1
#define MARGIN_TIME 1.1
#define TimeReact 1.5
enum enum_AEBStatus{NORMAL=0, ALERT=1, ALERT_BRAKE=2};
enum enum_AEBDecel{ZeroDecel = 0, PBdecel = 6, FBdecel = 8};//tive que alterara pra numero inteiro por causa do enum
enum enum_States{IDLE_STATE=0, FCW_STATE=1, PB_STATE=2, FB_STATE=3, RCW_STATE=4};
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
    float TTC = 0;

    // While relativeDistance, VehSpeed are provided, the "TTCCalculation" function shall calculate TTC using following equation
    TTC = relativeDistance / VehSpeed;

    return TTC;
}

int CollisionDecisionFunction(float TTC, int GearPos, float PBStoppingTime, float FBStoppingTime, int brakePedalStat, enum enum_States NextStateAEB)
{
    // Initialization of the variables
    float CWarnTime = 0;
    CWarnTime = PBStoppingTime + 1;

    printf("TTC = %f\n", TTC );
    printf("CWarnTime = %f\n", CWarnTime );
    printf("PBStoppingTime = %f\n",PBStoppingTime );
    printf("FBStoppingTime = %f\n",FBStoppingTime );
    printf("MARGIN_TIME*PBStoppingTime = %f\n",MARGIN_TIME*PBStoppingTime);
    printf("GearPos = %d\n",GearPos );
    printf("brakePedalStat = %d\n",brakePedalStat);
    printf("\nNextStateAEB_BEFORE Switch: %d\n", NextStateAEB);

    switch (NextStateAEB)
    {
    case IDLE_STATE:
        if(((TTC <= CWarnTime) && (GearPos == D))/**||((brakePedalStat == Pressed ) && (GearPos == D))||(brakePedalStat == Pressed)||(TTC > MARGIN_TIME*PBStoppingTime) **/) //Conditions to reach the FCW state
        {       
            printf("\nFCW_STATE1\n");
            NextStateAEB = FCW_STATE;      
        }
        else if(((TTC <= CWarnTime) && (GearPos == R)))
        {
            NextStateAEB = RCW_STATE;      
        }
        else // Conditions to reach the IDLE state
        {   printf("\nIDLE_STATE1\n");
            NextStateAEB= IDLE_STATE;
        }
  
        break;
    case FCW_STATE:

        if(((TTC <= PBStoppingTime) && (brakePedalStat == NotPressed))) //  Conditions to reach the PBraking state
        {   
            printf("\nPB_STATE1\n");
            NextStateAEB = PB_STATE;   
        }
        else if(TTC > MARGIN_TIME*CWarnTime)
        {
            printf("\nIDLE_STATE2\n");
            NextStateAEB = IDLE_STATE;
        }
        else
        {   
            printf("\nFCW_STATE2\n");
            NextStateAEB = FCW_STATE; 
        }
        break;
    case PB_STATE:
    {
       if((TTC <= FBStoppingTime)||((TTC <= FBStoppingTime) && (brakePedalStat == NotPressed)))// Conditions to reach the FBraking state
        {   printf("\nFB_STATE1\n");
            NextStateAEB = FB_STATE;

        }
        else if(TTC > MARGIN_TIME*PBStoppingTime || brakePedalStat == Pressed)
        {   printf("\nFCW_STATE3\n");
            NextStateAEB = FCW_STATE;
        }
        else
        {  printf("\nPB_STATE2\n");
            NextStateAEB = PB_STATE;
        } 
        break;
    }
    case FB_STATE:
    { 
        if((TTC <= FBStoppingTime)||((TTC <= FBStoppingTime) && (brakePedalStat == NotPressed)))// Conditions to reach the FBraking state
        {   printf("\nFB_STATE2\n");
            NextStateAEB = FB_STATE;

        }
        else if((TTC > MARGIN_TIME*PBStoppingTime) && (GearPos == R) || (brakePedalStat == Pressed ) && (GearPos == R))
        {   printf("\nFB_STATE1 - from RCW STATE\n");       
            NextStateAEB = RCW_STATE;
        }
        else if(TTC > MARGIN_TIME*FBStoppingTime && (GearPos == D))
        {   printf("\nPB_STATE3\n");
            NextStateAEB = PB_STATE;   
        }
        else if( (brakePedalStat == Pressed ) && (GearPos == D))
        {   printf("\nFCW_STATE4\n");
            NextStateAEB = FCW_STATE;
        }        
        else
        {   printf("\nFB_STATE3\n");
            NextStateAEB = FB_STATE;
        } 
        break;
    case RCW_STATE:
            printf("\nRCW_STATE\n");
            if((TTC <= PBStoppingTime) && (brakePedalStat == NotPressed))
            {
                NextStateAEB = FB_STATE;  
            }
            else if(TTC > MARGIN_TIME*CWarnTime)
            {
                NextStateAEB = IDLE_STATE;  
            }
            else 
            {
                NextStateAEB = RCW_STATE;  
            }
        break;  
    }
    default:
        break;
    }
  
  

    printf("\nNextStateAEB_AFTER Switch: %d\n", NextStateAEB);

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
    else if((AEBStatus == ALERT && GearPos == D) || (AEBStatus == ALERT_BRAKE && GearPos == D))// fazem sentido?? como devemos escrever na variável de forma coreta? Att: Bruno Rosa -
    {
         visualCmd = 1;
    }
    else if((AEBStatus == ALERT && GearPos == R) || (AEBStatus == ALERT_BRAKE && GearPos == R))
    {
        visualCmd = 2;
    }
    else
    {
        visualCmd = 0; 
    }

    return visualCmd;
}

bool EnablerFunction(float VehSpeed, enum Gear GearPos)
{   
    // Initialization of the variables
    bool EnablerStatus;
    float VehSpeed_ms = 0.00;

    // Must do a conversion for VehSpeed[km/h] in order to convert to [m/s]
    VehSpeed_ms = VehSpeed/3.6;  
    // Logical Process to begin the system
    if((GearPos == R) && ((1 <= VehSpeed_ms ) && (VehSpeed_ms  <= 10)))
    {
        EnablerStatus = ON;
    }
    else if((GearPos == D) && ((5 <= VehSpeed_ms ) && (VehSpeed_ms  <= 50)))
    {
        EnablerStatus = ON;
    }
    else
    {
        EnablerStatus = OFF;
    }

    return EnablerStatus;
}

int main(int argc, char *argv[])
{
    printf("--------Parametros Iniciais de Simulacao------\n");
    enum brakePedal brakePedalStat = NotPressed; 
    enum Gear GearPos = 1;
    float relativeDistance= 40.0;
    float VehSpeed = 15.5;
    float VehSpeed_km = VehSpeed*3.6;
    float roadCondCoeff = 0.625;
     int count = 0;
    float *StoppingTime; 
    float TTC=0; 

    int AEBState=OFF;
    int VisualCMD=OFF;
    int SoundCMD=OFF;
    enum enum_AEBStatus AEBStatus = NORMAL;
    enum enum_AEBDecel AEBDecelRef = ZeroDecel;
    enum enum_AEBStatus NextState = IDLE_STATE;
    bool EnableAEB = false;

    EnableAEB =  EnablerFunction(VehSpeed_km, GearPos);
    if (EnableAEB == ON)
    {
        while(count<20)
        {
            printf("\nActual relativeDistance: %f\n",  relativeDistance);
            printf("\nActual VehSpeed: %f m/s\n",  VehSpeed);
            printf("\nroadCondCoeff: %f\n",  roadCondCoeff);
            printf("\n");

            TTC = TimetoCollisionCalculation(relativeDistance,VehSpeed);
            StoppingTime = StoppingTimeCalculation(VehSpeed,roadCondCoeff, TimeReact , PBdecel, FBdecel);
            AEBState = CollisionDecisionFunction(TTC, GearPos, *(StoppingTime), *(StoppingTime+1), brakePedalStat, AEBState);
            VisualCMD = VisualAlertFunction(AEBStatus, GearPos);
            switch (AEBState)
            {
            case IDLE_STATE:
                AEBStatus = NORMAL;
                AEBDecelRef = ZeroDecel;
                SoundCMD=OFF;      
                break;
            case FCW_STATE:      
                AEBStatus = ALERT;
                AEBDecelRef = ZeroDecel;
                SoundCMD=ON;   
                break;
            case PB_STATE:
                AEBStatus = ALERT_BRAKE;
                AEBDecelRef = PBdecel; // Value for the Partial Breaking between [1 and 6] 
                SoundCMD=ON;     
                break;
            case FB_STATE:
                AEBStatus = ALERT_BRAKE;
                AEBDecelRef = FBdecel; // Value for the Full Breaking between [6 and 9]
                SoundCMD=ON;    
                break;
            case RCW_STATE:
                AEBStatus = ALERT;
                AEBDecelRef = ZeroDecel;
                SoundCMD=ON;      
                break;
            default:
                break;
            }
            //printf("\nAEBState: %d", AEBState);
            printf("\n---------------Saidas--------------\n");
            printf("\nAEBStatus: %d", AEBStatus);
            printf("\nAEBDecelRef: %d\n", AEBDecelRef);
            printf("\nVisualCMD: %d\n",  VisualCMD);
            printf("\n-----------------------------\n");

            count ++;
            if(VehSpeed-1 < 0 || relativeDistance - 2 < 0 )///somente simular a velocidade caindo
            {
                break;
            }
            relativeDistance = relativeDistance - 2;
            VehSpeed = VehSpeed - 1;
            
        }

    }
    else
    {
        printf("\nEnableAEB: %d\n",  EnableAEB);      
    }
    return 0;
}
