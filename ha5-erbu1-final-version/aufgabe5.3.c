#include <msp430.h>                                                                             //
unsigned int zahler, stand = 0, port = 0;                                                       //
int r, timer = 1,s2=0,s1=0,led;                                                                 //
volatile unsigned int i;                                                                        //
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
    P1IFG=0;                                                                                    //---> P1 Interrupt flaggen sind zurÃ¼ckgesetz
    P2IFG=0;                                                                                    //---> P2 Interrupt flaggen sind zurÃ¼ckgesetz
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
//--------------------------------TÃƒâ€Ã‚Â±mer Konfiguration------------------------------------//
    TB0CTL = TBSSEL_2 + MC_1;                                                                   //---> takt quelle ist smclk und clock ist startet
    TB0CCR0 = 6000;                                                                             //---> timer zahlt bis 6000
    TB0R = 0;                                                                                   //---> TB0R Regiester ist zurÃƒÆ’Ã‚Â¼ckgesetzt
    TBCCTL0 = CCIE;                                                                             //---> CCR0 interrupt enabled
//----------------------------------------------------------------------------------------------//
//                                                                                              //
    __enable_interrupt();                                                                       //---> interrupt ist aktiviert                                                                                         //
//                                                                                              //
//------------------------------------------------main------------------------------------------//
    while (1);                                                                                  //
}                                                                                               //--->****main loop****
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
    TB0CCTL0 &= ~CCIFG;                                                                         //---> flag setzt zurÃƒÆ’Ã‚Â¼ck
        switch (stand){                                                                         //---> in welchem zustand findet sich timer ?
        case 1:                                                                                 //---> wenn zustand 1 ist, subrakthiere zahler bis 0
            if (zahler > 0)                                                                     //
                {zahler = zahler - 1;}                                                          //
            else                                                                                //---> wenn zahler null ist, 
            {   led = TB0R%2;                                                                   //---> ist TB0R gerade oder ungerade ?
                TB0CCR0 = 30000;                                                                //---> neue CCR0 wert ist 30000
                if( led > 0 )                                                                   //---> wenn TB0R gerade ist,ist die P1.0 an
                {   P1OUT |= BIT0;                                                              //
                    timer =1;                                                                   //---> p2interrupt  ist an
                    s1 =1;  }                                                                   //---> s1 ist an
                else                                                                            //
                {   P4OUT |= BIT7;                                                              //---> wenn TB0R ungerade ist,ist die P1.0 an
                    s2=1;   }                                                                   //---> s2 ist an
                stand = 3;                                                                      //---> jetzt ist timer im zustand 3
            }                                                                                   //
            break;                                                                              //
        case 0:                                                                                 //---> fur anfang und neue runde ist timer zustand 0,damit timer nichts tut
        break;                                                                                  //
        case 3:                                                                                 //---> wenn zustand 3 ist, ist
            TB0CCR0 = 50000;                                                                    //---> neue CCR0 wert ist 30000
            P1OUT &= ~BIT0;                                                                     //---> die rote led aus
            P4OUT &= ~BIT7;                                                                     //---> p4.7 ist aus
            TB0CTL |= ID_1;                                                                     //---> takt ist durch 2 dividiert
            TB0EX0 = TBIDEX_5;                                                                  //---> takt ist durch 5 dividiert   TB0R 25000 HEiSST 250 milisekunde
            stand =7;                                                                           //---> neue zustand  ist 7
            r=0;                                                                                //---> r ist 0. r ist eine variable um zu prÃ¼fen, wie viel runde verlauft nach dem blink
            TB0R = 0;                                                                           //
            break;                                                                              //
        case 7 :                                                                                //---> wenn zustand 7 ist
            r++;                                                                                //---> addiere 1 zu r
            break;                                                                              //
        case 8:                                                                                 //
            TB0CCR0 = 6000;                                                                     //---> neue ccr0 ist 6000
            P2IFG &= ~BIT1;                                                                     //---> löscht ınterrupt auf p2.1 um sicher zu sein
            timer =1;                                                                           //---> interrupt auf p2 ist an
            stand =0;                                                                           //---> neue zustand ist 0
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
{                                                                                               //---> interruptflag wird gelÃ¶scht
    if (timer)                                                                                  //---> ist timer 1? wenn ja : ist interrupt an
    {                                                                                           //
        if (s1)                                                                                 //---> ist s1 1?
        {                                                                                       //
            if (TB0R <= 25000 && r == 0)                                                        //---> ist r 0 und TB0R kleiner als 25000?
            {                                                                                   //
                P4OUT |= BIT7;                                                                  //---> ja ---> schalte p4.7  (grune led) ein
                                                                                                //
            }                                                                                   //
            else                                                                                //
            {                                                                                   //
                P1OUT |= BIT0;                                                                  //---> nein ---> schalte p1.0  (rote led) ein
                                                                                                //
                r=0;                                                                            //---> r zuruckgesetzt
            }                                                                                   //
            s1=0;                                                                               //---> s1 ist 0
            timer =0;                                                                           //---> intterrupt auf p2 ist aus
            TB0CTL &= ~ID_3;                                                                    //---> divider ist 1
            TB0EX0 = 0;                                                                         //---> exdivider ist 1
            stand = 8;                                                                          //---> neue zustand ist 8
                                                                                                //
            for(i=5000; i>0; i--);                                                              //---> verzogerung
        }                                                                                       //
                                                                                                //
    else {                                                                                      // wenn s1 0 ist 
        stand = 1;                                                                              //---> zustand ist 1
        P1OUT &= ~BIT0;                                                                         //---> p1.0 ist low
        P4OUT &= ~BIT7;                                                                         //---> p4.7 ist low
        zahler = TB0R;                                                                          //---> die zufallige zahl ist die aktuelle wert in der TB0R
        TB0R = 0;                                                                               //---> zahler reset
        TB0CCR0 = 500;                                                                          //---> neue ccr0 wert ist 500
        timer = 0;                                                                              //---> timer ist 0
        r=0; }                                                                                  //---> r ist reset
    }                                                                                           //
    P2IFG &= ~BIT1;                                                                             //---> loscht interrupt flag
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
    if (s2){                                                                                    //---> ist s2 an?
    if (TB0R <= 25000 && r == 0)                                                                //---> ist r 0 und TB0R kleiner als 25000?
    {                                                                                           //
        P4OUT |= BIT7;                                                                          //---> ja ---> schalte p4.7  (grune led) ein
                                                                                                //
    }                                                                                           //
    else                                                                                        //
    {                                                                                           //
        P1OUT |= BIT0;                                                                          //---> nein  ---> schalte p1.0  (rote led) ein
        r=0;                                                                                    //
                                                                                                //
    }                                                                                           //
    TB0CCR0 = 6000;                                                                             //---> neue ccr0 ist 6000
    TB0CTL &= ~ID_3;                                                                            //---> divider ist 1
    TB0EX0 = 0;                                                                                 //---> exdivider ist 1
    timer = 1;                                                                                  //---> interrupt auf p2 ist an
    s2=0;                                                                                       //---> s2 ist aus
       }                                                                                        //
    for(i=5000; i>0; i--);                                                                      //---> verzogerung
    P1IFG &= ~BIT1;                                                                             //---> interruptFLAG wird gelÃ¶scht
}                                                                                               //
//**************************************Ende****************************************************//
