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
@brief Sound alert to the driver about emergency collision event shall be controlled by the "soundAlertControl" function

@param[in] AEBStatus enum. Current AEB Status (internal command to activate Visual and Sound alert)
 **/
bool SoundAlertFunction(enum AEBStatus)
{   
    // Initialization of the variables
    bool soundCmd;

    // Verification if the AEBStatus is in either ALERT or ALERT_BRAKE
    if(AEBStatus != 0 && AEBStatus != 1){
        soundCmd = true;
    }else{
        soundCmd = false;
    }

    return soundCmd;
}
