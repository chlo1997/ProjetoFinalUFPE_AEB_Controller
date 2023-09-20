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
    bool soundCmd = false;

    // Verification if the AEBStatus is in either ALERT or ALERT_BRAKE
    if(AEBStatus != OFF){
        soundCmd = true;
    }else{
        soundCmd = false;
    }

    return soundCmd;
}

/**
@brief Visual alert to the driver about emergency collision event shall be controlled by the "visualAlertControl" function

@param[in] AEBStatus enum. Current AEB Status (internal command to activate Visual and Sound alert)
@param[in] GearPos enum. Actual Selected Gear Position
 **/
enum VisualAlertFunction(enum AEBStatus, enum GearPos) //(1) Precisamos verificar se podemos utilizar "enum" como tipo de chamada da função VisualAlertFunction
{   
    // Initialization of the variables
    enum visualCmd;

    // Verification it´s status in order to activate the Visual Alert
    if(AEBStatus == OFF)
    {
        visualCmd = 0; // (2)Favor verificar todas as condições deste condicional. Estes valores recebidos pela variável VisualCmd
    }else if(GearPos == P || GearPos == N)// fazem sentido?? como devemos escrever na variável de forma coreta? Att: Bruno Rosa
    {
        visualCmd = 1;
    }else if(GearPos == R)
    {
        visualCmd = 2;
    }else
    {
        visualCmd = 3;
    }

    return visualCmd;
}

