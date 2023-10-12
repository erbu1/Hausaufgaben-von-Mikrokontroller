#include <msp430.h>


/**
 * blink.c
 */
void main(void)
{
//-------------------------------------beendet  watchdog timer-------------------------------------
    WDTCTL = WDTPW | WDTHOLD;
//-------------------------------------------------------------------------------------------------
//
//
//
//-------------------------------------ein-ausgabe konfiguration-----------------------------------
    P4DIR |= BIT7;                  // konfiguriere P4.7 als output
    P2OUT |= BIT1;                  // konfiguriere internal pullup resistor auf P2.1
    P2REN |= BIT1;                  // aktiviere internal pullup/pulldown resistor auf P2.1
//-------------------------------------------------------------------------------------------------

    volatile unsigned int i;        //  definiere  eine variable,um programm zu optimieren
//
//
//
//------------------------------------------------main---------------------------------------------
    while(1)
    {
        if (P2IN & BIT1)            // ist Tater (P2.1, low) gedrükt ?
            P4OUT &= ~BIT7;           // schalte P4.7 (LED) aus
        else {
            P4OUT ^= BIT7;          // blinkt P4.7
            for(i=10000; i>0; i--); // verzogerung
        }
    }
}
