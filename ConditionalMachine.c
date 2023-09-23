
#include <stdio.h>
#include <math.h>
#include <string.h>
#define OFF 0
#define ON 1
#define R 0
#define D 1
#define MARGIN_TIME 1.1


void CollisionDecisionFunction(float TTC, int GearPos, float PBStoppingTime, float FBStoppingTime, int brakePedalStat)
{   
    // Initialization of the variables
    enum Status{NORMAL=0, ALERT=1, ALERT_BRAKE=2}AEBStatus;
    enum AEBDecel{StateDecel = 0, PBdecel = 6, FBdecel = 8}AEBDecelRef;
    float CWarnTime;

    CWarnTime = PBStoppingTime + 1;
    printf("TTC = %f\n", TTC );
    printf("CWarnTime = %f\n", CWarnTime );
    printf("PBStoppingTime = %f\n",PBStoppingTime );
    printf("FBStoppingTime = %f\n",FBStoppingTime );
    printf("GearPos = %d\n",GearPos );
    printf("brakePedalStat = %d\n",brakePedalStat);

  
    if (TTC > 1.1*CWarnTime) // Conditions to reach the IDLE state
    {   
        AEBStatus = NORMAL;
        AEBDecelRef = StateDecel;
    }
    else if(((TTC <= CWarnTime) && (GearPos == R))||((brakePedalStat == ON ) && (GearPos == D))||(brakePedalStat == ON)||(TTC > MARGIN_TIME*PBStoppingTime)) //Conditions to reach the FCW state
    {  
        AEBStatus = ALERT;
        AEBDecelRef = StateDecel;
    }
    else if(((TTC <= PBStoppingTime) && (brakePedalStat == 0))||((TTC > 1.1*FBStoppingTime) && (GearPos == D))) //  Conditions to reach the PBraking state
    {
        printf("\nPassei por aqui");
        AEBStatus = ALERT_BRAKE;
        AEBDecelRef = PBdecel; // Value for the Partial Breaking between [1 and 6]

    }else if((TTC <= FBStoppingTime)||((TTC <= FBStoppingTime) && (brakePedalStat == 0)))// Conditions to reach the FBraking state
    {
        AEBStatus = ALERT_BRAKE;
        AEBDecelRef = FBdecel; // Value for the Full Breaking between [6 and 9]

    }else if(((brakePedalStat == ON) && (GearPos == R))||((TTC <= CWarnTime) && (GearPos == R))||((TTC > MARGIN_TIME*PBStoppingTime) && (GearPos == R)))// Conditions to reach the RCW state
    {
        AEBStatus = ALERT;
        AEBDecelRef = 0;

    }else
    {
        // Na duvida freia o carro!
    }
    printf("\nAEBStatus: %d", AEBStatus);
    printf("\nAEBDecelRef: %d\n", AEBDecelRef);

    //return AEBStatus, AEBDecelRef; // Como que retorna??
};


int main(int argc, char *argv[])
{
    printf("--------Parametros Iniciais de Simulacao------\n");
    enum Gear {Drive =1, Reverse =0}GearPos;
    enum brakePedal{Pressed = 1, NotPressed = 0}brakePedalStat;
    float TTC = 0.22;
    GearPos = Drive;
    float PBStoppingTime = 0.227;
    float FBStoppingTime = 0.218; 
    brakePedalStat = Pressed;

    while(1)
    {
        enum Status{ALERT=1,ALERT_BRAKE=2}AEBStatus;
        enum AEBDecel{State = 0, PBdecel = 6, FBdecel = 8}AEBDecelRef;
        CollisionDecisionFunction(TTC, Drive, PBStoppingTime, FBStoppingTime, NotPressed );

    }
    return 0;
}