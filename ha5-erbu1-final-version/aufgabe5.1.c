#include <msp430.h>
unsigned int i,zahler,stand=0,port=0,timer=1;
void main(void)
{
//-------------------------------------beendet  watchdog timer-------------------------------------
    WDTCTL = WDTPW | WDTHOLD;
//-------------------------------------------------------------------------------------------------
//
//
//
//-------------------------------------ein-ausgabe konfiguration--------------------------------//
    P1OUT = 0;                                                                                  //---> alle p1 ausgangen sind 0
    P4OUT = 0;                                                                                  //---> alle p4 ausgangen sind 0
    P4DIR |= BIT7;                                                                              //---> konfiguriere P4.7 als output
    P1DIR |= BIT0;                                                                              //---> konfiguriere P1.0 als output
    P1OUT |= BIT1;                                                                              //---> konfiguriere internal pullup resistor auf P1.1
    P2OUT |= BIT1;                                                                              //---> konfiguriere internal pullup resistor auf P2.1
    P1REN |= BIT1;                                                                              //---> aktiviere internal pullup/pulldown resistor auf P1.1
    P2REN |= BIT1;                                                                              //---> aktiviere internal pullup/pulldown resistor auf P2.1
    P2IES |= BIT1;
    P2IE |= BIT1;
//----------------------------------------------------------------------------------------------//
//                                                                                              //
//                                                                                              //
//                                                                                              //
//-------------------------------------------TÄ±mer Konfiguration-----------------------------//
    TB0CTL = TBSSEL_2+MC_1;                                                                     //---> takt quelle ist smclk und clock ist startet
    TB0CCR0 = 6000;                                                                             //---> timer zahlt bis 6000
    TB0R = 0;                                                                                   //---> TB0R Regiester ist zurÃ¼ckgesetzt
    TBCCTL0 = CCIE;                           													// CCR0 interrupt enabled
//----------------------------------------------------------------------------------------------//
//

   __enable_interrupt(); 																		// aktiviert die interrupt

//                                                                                             //
//------------------------------------------------main-----------------------------------------//
    while (1);                                                                         		   //--->****main loop****

}	
// Timer0 A0 interrupt Funktion
#pragma vector=TIMERB0_VECTOR
__interrupt void TIMERB0_ISR (void){
    TB0CCTL0 &= ~CCIFG;                                                           //---> flag setzt zurÃ¼ck
    if (port){
        timer =0; // timer zustand ist 0
        switch (stand) {
        case 0:
            if (zahler > 0)                                                                                 //---> ist zahler mehr als 0
                                {                                                                               //
                                    zahler = zahler - 1;                                                        //---> subrahiere 1 von zahler
                                }
            else  {
            P1OUT |= BIT0; // P1.0 ist an
            TB0CCR0 = 50000; // neue CCR0 wert ist 50000
            stand =3;} // neue timer zustand ist 3
            break;
        case 3:
            P1OUT &= ~BIT0; // P1.0 ist aus
            port = 0; // port ist 0
            timer =1; // timer ist an
            TB0CCR0 = 6000; // neue CCR0 wert ist 6000
            break;
}
    }


}

// PORT 2 interrupt Funktion
#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{   P2IFG &= ~BIT1; // FLAG wird zuruckgesetz
    if(timer){
    port =1;							 // port ist 1
    stand =0; 											//  timer zustand ist 0
    P1OUT &= ~BIT0;								//---> p1.0 ist low
    P4OUT &= ~BIT7;                     //---> p4.7 ist low
    zahler = TB0R;                       //---> die zufallige zahl ist die aktuelle wert in der TB0R
    TB0R = 0;                             //---> zahler reset
    TB0CCR0 = 500;			// neue CCR0 wert ist 500
    timer=0;					 //timer ist 0
}
}
