#include <stdio.h>
#include <stdbool.h>
#include "AEBFunctions.h"


/**
@brief Estimates current vehicle stopping time based on current vehicle speed, road conditions, adds driver reaction time

@param[in] VehSpeed float. Actual Vehicle Speed
@param[in] roadCondCoeff float. Percentage of friction coefficient of the road based on vehicle measured climate conditions (wet or dry)
@param[in] driverTimeReact float. Configurable additional Driver's Time Reaction to press Brake pedal 
@param[in] PBDecel float. Configurable Partial Braking Deceleration used by AEB as deceleration reference
@param[in] FBDecel float. Configurable Full Braking Deceleration used by AEB as deceleration reference
@return StoppingTime float. Estimates current vehicle stopping time based on current vehicle speed, road conditions, adds driver reaction time

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
@brief Calculates current vehicle time to (TTC) with the obstacle

@param[in] relativeDistance float. Relative distance between vehicle and obstacle
@param[in] VehSpeed float. Current vehicle speed
@return TTC float. Time to collision.
 **/
float TTCCalculation(float relativeDistance, float VehSpeed)
{   
    float TTC = 0;
    TTC = relativeDistance / VehSpeed;

    return TTC;
}


/**
@brief Enables AEB decision to arbitrates about AEB status and Deceleration reference

@param[in] TTC float. Time To Collision
@param[in] GearPos enum. Current selected gear position
@return EnablerStatus bool. Provides status for enabling AEB decision
 **/
bool EnablerFunction(float TTC, enum Gear GearPos)
{   
    // Initialization of the variables
    bool EnablerStatus = false;

    // Logical Process to begin the system
    if((GearPos == D) && ( TTC >= 0 && TTC <= 7.2 ))
    {
        EnablerStatus = true;
    }
    else if((GearPos == R) && ( TTC >= 0 && TTC <= 2 ))
    {
        EnablerStatus = true;
    }
    else
    {
        EnablerStatus = true;
    }

    return EnablerStatus;
}


/**
@brief Control alert to the driver about emergency collision event using display command

@param[in] AEBStatus enum. Current AEB Status
@param[in] GearPos enum. Actual Selected Gear Position
@return visualCmd int. Activation command of AEB visual alert
 **/
int visualAlertControl(int AEBStatus, enum Gear GearPos) //(1) Precisamos verificar se podemos utilizar "enum" como tipo de chamada da função VisualAlertFunction
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


/**
@brief Control alert to the driver about emergency collision event using sound command

@param[in] AEBStatus enum. Current AEB Status
@return soundCmd enum. Activation command of AEB sound alert
 **/
bool soundAlertControl(enum enum_AEBStatus AEBStatus)
{   
    // Initialization of the variables
    bool soundCmd = false;

    // Verification if the AEBStatus is in either ALERT or ALERT_BRAKE
    if(AEBStatus == NORMAL){
        soundCmd = false;
    }
    else
    {
        soundCmd = true;
    }

    return soundCmd;
}


/**
@brief Arbitrates about AEB status and required vehicle deceleration for safe brake.

This function implements the AEB status arbitration which is a decision fucntion to a finite state machine (FSM).

@param[in] TTC float. Time To Collision
@param[in] GearPos enum. Current selected gear position
@param[in] PBStoppingTime float. Maximum time to apply a partial braking deceleration
@param[in] FBStoppingTime float. Maximum time to apply a Full braking deceleration
@param[in] brakePedalStat int. Brake pedal pressed status
@param[in] CurrentStateAEB enum. Receives current AEB status according to the last arbitration
@return NextStateAEB enum. Decides next AEB status according to the last state and system inputs
 **/
int CollisionDecisionFunction(float TTC, int GearPos, float PBStoppingTime, float FBStoppingTime, int brakePedalStat, enum enum_States CurrentStateAEB)
{
    // Initialization of the variables
    enum enum_States NextStateAEB = IDLE_STATE;
    float CWarnTime = OFF;
    CWarnTime = PBStoppingTime + MARGIN_TIME;
    printf("CWarnTime = %f\n", CWarnTime );
    printf("PBStoppingTime = %f\n",PBStoppingTime );
    printf("FBStoppingTime = %f\n",FBStoppingTime );
    printf("\nNextStateAEB_BEFORE Switch: %d\n", CurrentStateAEB);

    switch (CurrentStateAEB)
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