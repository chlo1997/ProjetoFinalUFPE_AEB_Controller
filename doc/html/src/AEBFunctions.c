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
 **/
bool EnablerFunction(float TTC, enum Gear GearPos)
{   
    // Initialization of the variables
    bool EnablerStatus = OFF;

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


/**
@brief Control alert to the driver about emergency collision event using display command

@param[in] AEBStatus enum. Current AEB Status (internal command to activate Visual and Sound alert)
@param[in] GearPos enum. Actual Selected Gear Position
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

@param[in] AEBStatus enum. Current AEB Status (internal command to activate Visual and Sound alert)
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