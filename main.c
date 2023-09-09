#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define PB1_decel 5.27
#define PB2_decel 6.29
#define FB_decel 7.3
#define rainParameter 0.625
#define minVelocity 1.38
#define reactTime 1.5
#define FrontalSafetyDistance 10.0
#define RearSafetyDistance 0.1
#define True 1
#define False 0

float distanceVehDiff, velocityVehDiff, velocityVehEgo, Decel, PB1_decel_RESULT,PB2_decel_RESULT, FB_decel_RESULT, PB1StoppingTime, PB2StoppingTime, FBStoppingTime, 
FCWStoppingTime, TTC = 0;
int roadCondition, gear, AEBStatus, FCWactivate, EgoCarStop, FrontalStatusColision, RearStatusColision, StatusCollision = 0;

void TTCCalculation(float distanceDiff, float velocityDiff, int gear_state)
{   
    float RealDistVehDiff = 0;

    if(gear_state !=0)//Frontal Collision
    {
        RealDistVehDiff = distanceDiff + FrontalSafetyDistance; 
        TTC = RealDistVehDiff/fabs(velocityDiff);
        if(RealDistVehDiff<1.5)
        {
            FrontalStatusColision = True;
        }
        else
        {
            FrontalStatusColision = False;    
        }
    }
    else
    {
        RealDistVehDiff = distanceDiff + RearSafetyDistance; 
        TTC = RealDistVehDiff/fabs(velocityDiff);
        if(RealDistVehDiff<1.5)
        {
            RearStatusColision = True;
        }
        else
        {
            RearStatusColision = False;    
        }
    }
    printf("\ngear %d\n", gear_state);
    printf("distanceDiff = %f\n", distanceDiff );
    printf("RealDistVehDiff = %f\n", RealDistVehDiff);
    printf("TTC %f\n", TTC);
}

void StoppingTimeCalculation(float distanceDiff, float egoVelocity)
{
    FCWStoppingTime = egoVelocity/PB1_decel + reactTime;
    PB1StoppingTime = egoVelocity/PB1_decel;
    PB2StoppingTime = egoVelocity/PB2_decel;
    FBStoppingTime = egoVelocity/PB2_decel;
    printf("\nFCWStoppingTime %f\n", FCWStoppingTime);
    printf("PB1StoppingTime %f\n", PB1StoppingTime);
    printf("PB2StoppingTime %f\n", PB1StoppingTime);
    printf("FBStoppingTime %f\n", FBStoppingTime);
    
}
//StateMachineAEB(TTC, FCWStoppingTime, PB1StoppingTime, PB2StoppingTime, FBStoppingTime,PB1_decel_RESULT, PB2_decel_RESULT,FB_decel_RESULT)
//{
    
//}


void AEB(float distanceVehDiff_aux, float velocityVehDiff_aux, float Decel_aux, int roadCondition_aux, int gear_aux, int AEBStatus_aux, int EgoCarStop_aux, int StatusCollision_aux)
{
    //roadCondition = getRoadCondition();
    if(roadCondition !=0)
    {
        PB1_decel_RESULT = PB1_decel * rainParameter;
        PB2_decel_RESULT = PB2_decel * rainParameter;
        FB_decel_RESULT = FB_decel * rainParameter;
        
    }
    else
    {
        PB1_decel_RESULT = PB1_decel;
        PB2_decel_RESULT = PB2_decel;
        FB_decel_RESULT = FB_decel;
    }
    if (ve)
    {
        /* code */
    }
    
    StoppingTimeCalculation(distanceVehDiff_aux,velocityVehDiff_aux);
    TTCCalculation(distanceVehDiff_aux,velocityVehDiff_aux, gear_aux);
   // StateMachineAEB(TTC, FCWStoppingTime, PB1StoppingTime, PB2StoppingTime, FBStoppingTime,PB1_decel_RESULT, PB2_decel_RESULT,FB_decel_RESULT);
}


int getRoadCondition()
{
    return 0;
}
int main(int argc, char *argv[])
{
    printf("--------Parametros Iniciais de Simulacao------\n");
    distanceVehDiff = 50.0;
    velocityVehDiff = 100.0;
    velocityVehEgo = 100.0;
    Decel = 0;
    roadCondition = 1;
    gear = 1;
    printf("distanceVehDiff = %f\n", distanceVehDiff);
    printf("velocityVehDiff = %f\n", velocityVehDiff);
    printf("Decel = %f\n", Decel);
    printf("roadCondition = %d\n", roadCondition);
    printf("gear = %d\n", gear);
    printf("AEBStatus = %d\n", AEBStatus);
    printf("EgoCarStop = %d\n", EgoCarStop);
    printf("StatusCollision = %d\n", StatusCollision);
    AEB(distanceVehDiff,velocityVehDiff, Decel, roadCondition, gear, AEBStatus, EgoCarStop, StatusCollision);
        
    return 0;
}