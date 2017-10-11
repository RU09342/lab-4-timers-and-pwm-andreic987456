#include <msp430fr2311.h>

#define LED1   BIT0
#define BUTTON  BIT1
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;               // Disable GPIO default high-impedance mode
    P1DIR  = LED1;                      // Set P1.0 RED LED to output
    P1REN = BUTTON;                       // Enables resistor for button P1.3
    P1OUT = BUTTON;                       // Makes resistor P1.3 a pull up

    while(1)
    {
        if(!(P1IN & BUTTON))              // Check for button Press
        {
            P1OUT ^= LED1;     // Activate LED 1 RED
        }
        else
        {
            P1OUT ^= 0x00;
        }
        _delay_cycles( 100000);

    }
    return 0;
}
