#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "AEBFunctions.h"



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
        TTC = TTCCalculation(relativeDistance, VehSpeed_ms);
        printf("\nTTC = %f\n", TTC );
        EnableAEB =  EnablerFunction(TTC, GearPos);
        if (EnableAEB == ON)
        {
            StoppingTime = StoppingTimeCalculation(VehSpeed_ms,roadCondCoeff, TimeReact , PBdecel, FBdecel);
            AEBState = CollisionDecisionFunction(TTC, GearPos, *(StoppingTime), *(StoppingTime+1), brakePedalStat, AEBState);
            VisualCMD = visualAlertControl(AEBStatus, GearPos);

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
