#include <stdio.h>
#include <math.h>
#include <string.h>
#define OFF 0
#define ON 1
#define R 0
#define D 1
#define MARGIN_TIME 1.1
enum enum_AEBStatus{NORMAL=0, ALERT=1, ALERT_BRAKE=2};
enum enum_AEBDecel{ZeroDecel = 0, PBdecel = 6, FBdecel = 8};//tive que alterara pra numero inteiro por causa do enum
enum enum_States{IDLE_STATE=1, FCW_STATE=2, PB_STATE=3, FB_STATE=4, RCW_STATE=5};

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
};


int main(int argc, char *argv[])
{
    printf("--------Parametros Iniciais de Simulacao------\n");
    enum Gear {Drive =1, Reverse =0}GearPos;
    enum brakePedal{Pressed = 1, NotPressed = 0}brakePedalStat;
    float TTC = 0.18;
    GearPos = 0;
    float PBStoppingTime = 1.19;
    float FBStoppingTime = 1.18;
    brakePedalStat = Pressed;
    enum enum_AEBStatus AEBStatus;
    enum enum_AEBDecel AEBDecelRef;
    enum enum_AEBStatus NextState = IDLE_STATE;// Nao vi necessidade de colocar..o programa estará no loop, entao a todo momento está vendo as condicoes do CollisionDecisionFunction

    int AEBState = CollisionDecisionFunction(TTC, GearPos, PBStoppingTime, FBStoppingTime, NotPressed, NextState);
    switch (AEBState)
    {
    case IDLE_STATE:
        AEBStatus = NORMAL;
        AEBDecelRef = ZeroDecel;
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
    //printf("\nAEBState: %d", AEBState);
    printf("-----------Saidas----------\n");
    printf("\nAEBStatus: %d", AEBStatus);
    printf("\nAEBDecelRef: %d\n", AEBDecelRef);

    return 0;
}
