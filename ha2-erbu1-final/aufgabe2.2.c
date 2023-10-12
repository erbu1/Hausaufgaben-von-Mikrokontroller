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
    P1DIR |= BIT0;                  // konfiguriere P1.0 als output
    P1OUT |= BIT1;                  // konfiguriere internal pullup resistor auf P1.1
    P2OUT |= BIT1;                  // konfiguriere internal pullup resistor auf P2.1
    P1REN |= BIT1;                  // aktiviere internal pullup/pulldown resistor auf P1.1
    P2REN |= BIT1;                  // aktiviere internal pullup/pulldown resistor auf P2.1
//-------------------------------------------------------------------------------------------------

    volatile unsigned int i;        //  definiere  eine variable,um programm zu optimieren
//
//
//
//------------------------------------------------main---------------------------------------------
    while(1)
    {
        if (P2IN & BIT1)            // ist Tater (P1.1, low) gedrükt ?
            P4OUT &= ~BIT7;           // schalte P4.7 (LED) aus
        else {
            P4OUT ^= BIT7;          // blinkt P4.7
            for(i=10000; i>0; i--); // verzogerung(erstens habe ich i als 0xffff definiert aber diese wert war zu viel, dass p1.0 einschalten kann, sobald man p1.1 gedruckt,deswegen habe ich es verringert und habe für den wert 10000 optimale entscheidet)
        }
        if (P1IN & BIT1)            // ist Tater (P1.1, low) gedrükt ?
            P1OUT &= ~BIT0;           // schalte P4.7 (LED) aus
        else {
            P1OUT |= BIT0;          // schalte  P1.0 ein

        }
    }
}

