#include <msp430.h>                                                                             //
unsigned int i, zahler, stand = 0, port = 0;                                                    //
int r, timer = 1,s2=0;                                                                          //
void main(void)                                                                                 //
{                                                                                               //
//-------------------------------------beendet  watchdog timer----------------------------------//
                                                                                                //
    WDTCTL = WDTPW | WDTHOLD;                                                                   //
//----------------------------------------------------------------------------------------------//
//                                                                                              //
//                                                                                              //
//                                                                                              //
//-------------------------------------ein-ausgabe konfiguration--------------------------------//
    P1IFG=0;                                                                                    //---> P1 Interrupt flaggen sind zurückgesetz
    P2IFG=0;                                                                                    //---> P2 Interrupt flaggen sind zurückgesetz
    P1OUT = 0;                                                                                  //---> alle p1 ausgangen sind 0
    P4OUT = 0;                                                                                  //---> alle p4 ausgangen sind 0
    P4DIR |= BIT7;                                                                              //---> konfiguriere P4.7 als output
    P1DIR |= BIT0;                                                                              //---> konfiguriere P1.0 als output
    P1OUT |= BIT1;                                                                              //---> konfiguriere internal pullup resistor auf P1.1
    P2OUT |= BIT1;                                                                              //---> konfiguriere internal pullup resistor auf P2.1
    P1REN |= BIT1;                                                                              //---> aktiviere internal pullup/pulldown resistor auf P1.1
    P2REN |= BIT1;                                                                              //---> aktiviere internal pullup/pulldown resistor auf P2.1
    P2IES |= BIT1;                                                                              //---> high-to-low erzeugt interrupt auf p2.1
    P2IE |= BIT1;                                                                               //---> Interrupt ist auf p2.1 an
    P1IES |= BIT1;                                                                              //---> high-to-low erzeugt interrupt auf p1.1
    P1IE |= BIT1;                                                                               //---> Interrupt ist auf p1.1 an
//----------------------------------------------------------------------------------------------//
//                                                                                              //
//                                                                                              //
//                                                                                              //
//-------------------------------------------TÃ„Â±mer Konfiguration-----------------------------//
    TB0CTL = TBSSEL_2 + MC_1;                                                                   //---> takt quelle ist smclk und clock ist startet
    TB0CCR0 = 6000;                                                                             //---> timer zahlt bis 6000
    TB0R = 0;                                                                                   //---> TB0R Regiester ist zurÃƒÂ¼ckgesetzt
    TBCCTL0 = CCIE;                                                                             //---> CCR0 interrupt enabled
//----------------------------------------------------------------------------------------------//
//                                                                                              //
    __enable_interrupt();                                                                       //---> interrupt ist aktiviert                                                                                         //
//                                                                                              //
//------------------------------------------------main------------------------------------------//
    while (1);}                                                                                 //--->****main loop****
//                                                                                              //
//                                                                                              //
//                                                                                              //
//*************************************Ende*****************************************************//
//                                                                                              //
//                                                                                              //
//                                                                                              //
//                                                                                              //
//                                                                                              //
//--------------------------- Timer0 A0 interrupt  Funktion-------------------------------------//
#pragma vector=TIMERB0_VECTOR                                                                   //
__interrupt void TIMERB0_ISR(void){                                                             //---> interrupt funktion wird definiert
    TB0CCTL0 &= ~CCIFG;                                                                         //---> flag setzt zurÃƒÂ¼ck
        switch (stand){                                                                         //---> in welchem zustand findet sich timer ?
        case 1:                                                                                 //---> wenn zustand 1 ist, subrakthiere zahler bis 0
            if (zahler > 0)                                                                     //
                {zahler = zahler - 1;}                                                          //
            else                                                                                //---> wenn zahler null ist, ist die rote led an
            {                                                                                   //
                P1OUT |= BIT0;                                                                  //
                TB0CCR0 = 50000;                                                                //---> neue ccr0 wert ist 50.000 damit eine runde 0.05 sekunde dauert
                stand = 3;                                                                      //---> jetzt ist timer im zustand 3
            }                                                                                   //
            break;                                                                              //
        case 0:                                                                                 //---> für anfang und neue runde ist timer zustand 0,damit timer nichts tut
        break;                                                                                  //
        case 3:                                                                                 //---> wenn zustand 3 ist, ist
            P1OUT &= ~BIT0;                                                                     //---> die rote led aus
            TB0CTL |= ID_1;                                                                     //---> takt ist durch 2 dividiert
            TB0EX0 = TBIDEX_4;                                                                  //---> takt ist durch 5 dividiert
            stand =7;                                                                           //---> neue zustand  ist 7
            r=0;                                                                                //---> r ist 0. r ist eine variable um zu prüfen, wie viel runde verlauft nach dem blink
            s2=1;                                                                               //---> s2 ist an,
            break;                                                                              //
        case 7 :                                                                                //---> wenn zustand 7 ist
            r++;                                                                                //---> addiere 1 zu r
            break;                                                                              //
        }                                                                                       //
}                                                                                               //
//************************************Ende******************************************************//
//                                                                                              //
//                                                                                              //
//                                                                                              //
//                                                                                              //
//                                                                                              //
//------------------------------ Port 2 Interrupt Funktion--------------------------------------//
#pragma vector=PORT2_VECTOR                                                                     //
__interrupt void Port2_ISR(void)                                                                //---> P2.1 interrupt funktion wird definiert
{                                                                                               //
    P2IFG &= ~BIT1;                                                                             //---> interruptflag wird gelöscht
    if (timer)                                                                                  //---> ist timer 1? wenn ja :
    {                                                                                           //
        stand = 1;                                                                              //---> zustand ist 1
        P1OUT &= ~BIT0;                                                                         //---> p1.0 ist low
        P4OUT &= ~BIT7;                                                                         //---> p4.7 ist low
        zahler = TB0R;                                                                          //---> die zufallige zahl ist die aktuelle wert in der TB0R
        TB0R = 0;                                                                               //---> zahler reset
        TB0CCR0 = 500;                                                                          //
        timer = 0;                                                                              //---> timer ist 0
        r=0;                                                                                    //---> r ist reset
    }                                                                                           //
}                                                                                               //
//**************************************Ende****************************************************//
//                                                                                              //
//                                                                                              //
//                                                                                              //
//                                                                                              //
//                                                                                              //
//------------------------------ Port 1 Interrupt Funktion--------------------------------------//
#pragma vector=PORT1_VECTOR                                                                     //
__interrupt void Port1_ISR(void)                                                                //---> P1.1 interrupt funktion wird definiert
{                                                                                               //
    P1IFG &= ~BIT1;                                                                             //---> interruptFLAG wird gelöscht
    if (s2){                                                                                    //---> ist s2 an?
        TB0CTL &= ~ID_3;                                                                        //---> divider ist 1
        TB0EX0 = 0;                                                                             //---> exdivider ist 1
        stand = 0;                                                                              //---> zustand ist 0
        timer = 1;                                                                              //---> timer ist 1
        TB0CCR0 = 6000;                                                                         //---> neue ccr0 ist 6000
        s2=0;                                                                                   //---> s2 ist aus
    if (TB0R <= 25000 && r == 0)                                                                //---> ist r 0 und TB0R kleiner als 25000?
    {                                                                                           // 
        P4OUT |= BIT7;                                                                          //---> ja ---> schalte p4.7  (grune led) ein
    }                                                                                           //
    else if (r>0)                                                                               //---> ist r grosser als 0
    {                                                                                           //
        P1OUT |= BIT0;                                                                          //---> ja ---> schalte p1.0  (rote led) ein
    }                                                                                           //
    }                                                                                           //
}                                                                                               //
//**************************************Ende****************************************************//
