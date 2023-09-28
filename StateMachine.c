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
#define FactorKM_MS 3.6
enum enum_AEBStatus{NORMAL=0, ALERT=1, ALERT_BRAKE=2};
enum enum_AEBDecel{ZeroDecel = 0, PBdecel = 6, FBdecel = 8};//tive que alterara pra numero inteiro por causa do enum
enum enum_States{IDLE_STATE=0, FCW_STATE=1, PB_STATE=2, FB_STATE=3, RCW_STATE=4};
enum Gear {Drive =1, Reverse =0, Park = 2, Neutro = 3};
enum brakePedal{Pressed = 1, NotPressed = 0};

/**
@brief Time to Collision (TTC) with the front or rear vehicle obstacle shall be determined by the "TTCCalculation" function

@param[in] relativeDistance float. Relative Distance between Vehicle and Road Obstacle (lead vehicle)
@param[in] VehSpeed float. Actual Vehicle Speed
 **/
float TimetoCollisionCalculation(float relativeDistance,float VehSpeed)
{   
    // Initialization of the variables
    float TTC = OFF;

    // While relativeDistance, VehSpeed are provided, the "TTCCalculation" function shall calculate TTC using following equation
    TTC = relativeDistance / VehSpeed;

    return TTC;
}

int CollisionDecisionFunction(float TTC, int GearPos, float PBStoppingTime, float FBStoppingTime, int brakePedalStat, enum enum_States NextStateAEB)
{
    // Initialization of the variables
    float CWarnTime = OFF;
    CWarnTime = PBStoppingTime + MARGIN_TIME;
    printf("CWarnTime = %f\n", CWarnTime );
    printf("PBStoppingTime = %f\n",PBStoppingTime );
    printf("FBStoppingTime = %f\n",FBStoppingTime );
    printf("\nNextStateAEB_BEFORE Switch: %d\n", NextStateAEB);

    switch (NextStateAEB)
    {
        case IDLE_STATE:
            if(((TTC <= CWarnTime) && (GearPos == D))/**||((brakePedalStat == Pressed ) && (GearPos == D))||(brakePedalStat == Pressed)||(TTC > MARGIN_TIME*PBStoppingTime) **/) //Conditions to reach the FCW state
            {       
                NextStateAEB = FCW_STATE;      
            }
            else if(((TTC <= CWarnTime) && (GearPos == R)))
            {
                NextStateAEB = RCW_STATE;      
            }
            else // Conditions to reach the IDLE state
            {   
                NextStateAEB= IDLE_STATE;
            }

            break;
        case FCW_STATE:

            if(((TTC <= PBStoppingTime) && (brakePedalStat == NotPressed))) //  Conditions to reach the PBraking state
            {   
                NextStateAEB = PB_STATE;   
            }
            else if(TTC > MARGIN_TIME*CWarnTime)
            {
                NextStateAEB = IDLE_STATE;
            }
            else
            {   
                NextStateAEB = FCW_STATE; 
            }
            break;
        case PB_STATE:
        {
            if((TTC <= FBStoppingTime)||((TTC <= FBStoppingTime) && (brakePedalStat == NotPressed)))// Conditions to reach the FBraking state
            {   
                NextStateAEB = FB_STATE;

            }
            else if(TTC > MARGIN_TIME*PBStoppingTime || brakePedalStat == Pressed)
            {   
                NextStateAEB = FCW_STATE;
            }
            else
            {  
                NextStateAEB = PB_STATE;
            } 
            break;
        }
        case FB_STATE:
        { 
            if((TTC <= FBStoppingTime)||((TTC <= FBStoppingTime) && (brakePedalStat == NotPressed)))// Conditions to reach the FBraking state
            {   
                NextStateAEB = FB_STATE;
            }
            else if((TTC > MARGIN_TIME*PBStoppingTime) && (GearPos == R) || (brakePedalStat == Pressed ) && (GearPos == R))
            {          
                NextStateAEB = RCW_STATE;
            }
            else if(TTC > MARGIN_TIME*FBStoppingTime && (GearPos == D))
            {   
                NextStateAEB = PB_STATE;   
            }
            else if( (brakePedalStat == Pressed ) && (GearPos == D))
            {   
                NextStateAEB = FCW_STATE;
            }        
            else
            {   
                NextStateAEB = FB_STATE;
            } 
            break;
        case RCW_STATE:
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

bool EnablerFunction(float TTC, enum Gear GearPos)
{   
    // Initialization of the variables
    bool EnablerStatus;

    // Logical Process to begin the system
    if((GearPos == D) && ( TTC >= 0 && TTC <= 7.2 ))
    {
        EnablerStatus = ON;
    }
    else if((GearPos == R) && ( TTC >= 0 && TTC <= 2 ))
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
    int AEBState=OFF;
    int VisualCMD=OFF;
    int SoundCMD=OFF;
    enum brakePedal brakePedalStat = NotPressed; 
    enum enum_AEBStatus AEBStatus = NORMAL;
    enum enum_AEBDecel AEBDecelRef = ZeroDecel;
    enum enum_AEBStatus NextState = IDLE_STATE;
    bool EnableAEB = false;

    printf("-----------Parametros Iniciais de Simulacao---------\n"); 
    enum Gear GearPos = 1;
    float relativeDistance= 40.0;
    float VehSpeed = 15.5;
    float roadCondCoeff = 0.625;
    int count = 0;
    float TTC=0;
    float *StoppingTime; 

    printf("\n Insira a Velocidade Inicial (km/h):");
    scanf("%f",&VehSpeed);
    float VehSpeed_ms = VehSpeed/FactorKM_MS;
    printf("\n Insira a Distancia Relativa Inicial (m):");
    scanf("%f",&relativeDistance);    
    printf("\n Insira a Macha (0-R/1-D/2-P/3-N):");
    scanf("%d",&GearPos);
    printf("\n Insira o roadCondCoeff (0.625-Chuva ou 1-Seco):");
    scanf("%f",&roadCondCoeff);  
    printf("\n Insira o status do brakePedal (0-NotPressed ou 1-Pressed):");
    scanf("%fd",&brakePedalStat); 
    printf("\n-----------------------------------------------\n");
   
    while(count<20)
    {   
        printf("\n------------------Simulacao--------------------\n"); 
        printf("\nActual VehSpeed: %f km/h",  VehSpeed_ms*FactorKM_MS);
        printf("\nActual relativeDistance: %f  (m)",  relativeDistance);
        printf("\nroadCondCoeff: %f",  roadCondCoeff);
        TTC = TimetoCollisionCalculation(relativeDistance, VehSpeed_ms);
        printf("\nTTC = %f\n", TTC );
        EnableAEB =  EnablerFunction(TTC, GearPos);
        if (EnableAEB == ON)
        {
            StoppingTime = StoppingTimeCalculation(VehSpeed_ms,roadCondCoeff, TimeReact , PBdecel, FBdecel);
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
            
            if(VehSpeed_ms-1 < 0 || relativeDistance - 2 < 0 )///SIMULATE VELOCITY AND DISTANCE
            {
                break;
            }
            relativeDistance = relativeDistance - 2;
            VehSpeed_ms = VehSpeed_ms - 1;
            
        } 
        else
        {
            printf("\nEnableAEB: %d\n",  EnableAEB);      
        }
        printf("\n---------------------Saidas--------------------\n");
        printf("\nAEBStatus: %d", AEBStatus);
        printf("\nAEBDecelRef: %d", AEBDecelRef);
        printf("\nVisualCMD: %d",  VisualCMD);
        printf("\nSoundCMD: %d",  SoundCMD);
        printf("\n-----------------------------------------------\n");


        count ++;
    }
    return 0;
}
