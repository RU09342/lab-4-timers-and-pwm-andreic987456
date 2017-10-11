#include <msp430g2553.h>

#define LEDG   BIT6
#define BUTTON  BIT3
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // stop watchdog timer
    P1DIR  |= LEDG;                     // Set P1.6 Green LED to output
    P1REN  |= BUTTON;                   // Enables resistor for BUTTON P1.3
    P1OUT  = BUTTON;                    // Makes resistor P1.3 a pull up
    P1IES = 0x0000;                     // Interrupts when BUTTON is pressed or low to high transition
    P1IE |= BUTTON;                     // Interrupt enable set to BUTTON
    P1IFG &= ~BUTTON;                   // Clear interrupt flag

    TA0CCTL0 = 0x0010;                  // Enables Capture/compare register for interrupt of timer A0
    TA0CCR0 = 60000;                    // Set value for Timer A0 to count up to
    TA0CTL = TASSEL_2 + MC_0;           // Timer_A Control Register - SMCLK high frequency clock, timer stopped

    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE);      // Low Power Mode w/ general interrupts enabled

}

// Interrupt Service Routine 1 which toggles on the LED on the release of the Button and starts timer a to act as a delay
#pragma vector = PORT1_VECTOR
__interrupt void Interrupt_1(void)
{
    P1OUT ^= LEDG;                      // Toggle LED
    P1IE &= ~BUTTON;                    // reset interrupt
    TA0CTL = TASSEL_2 + MC_1;           // Timer_A Control Register - SMCLK high frequency clock, Up
}

// Interrupt Service Routine 2 which stops Timer A0 and resets the interrupt
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Interrupt_2(void)
{
    TA0CTL = MC_0;                      // Stop timer
    P1IE |= BUTTON;                     // Interrupt enable set to BUTTON
    P1IFG = 0x0000;                     // Clear interrupt flag
}
