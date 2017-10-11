#include <msp430.h>
volatile unsigned int i = 500;
volatile unsigned int btnPress = 0;

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    P1DIR |= BIT2;                       // P1.2 and P1.3 output
    P1SEL |= BIT2;                       // P1.2 and P1.3 options select
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;
    TA0CCR0 = 1000 - 1;                          // PWM Period
    TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA0CCR1 = i - 1;                            // CCR1 PWM duty cycle
    TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR

    P1DIR &= ~(BIT1); //set P1.1 input
    P1REN |= BIT1; //enable pull-up resistor
    P1OUT |= BIT1;
    P1IE |= BIT1; //enable the interrupt on P1.1
    P1IES |= ~BIT1; //set to look for falling edge
    P1IFG &= ~(BIT1); //clear interrupt flag

    TA1CCR0 = 4000;
    TA1CCTL0 |= CCIE;
    TA1CTL |= TASSEL_2 + MC_1;

    __enable_interrupt();

    __bis_SR_register(GIE);             // Enter LPM0
    while (1) {
    }
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_Debounce(void) {
    TA1CCTL0 &= ~CCIE; // Turn off interrupt enable
    TA1CTL = MC_0; // Stop timer
    P1IE |= BIT1; // Enable button interrupt
    if (btnPress == 1) {
        P4OUT ^= BIT7; // Toggle LED
        if (i >= 1000) {
            i = 1;
        }

        else {
            i += 100;
        }
    }
    TA0CCR1 = i - 1;
    btnPress = 0;
    P1IFG &= ~BIT1; // Clear flag
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void) {
    TA1CTL |= TASSEL_2 + MC_1; // Start Timer 1
    P1IE &= ~BIT1; // Turn off interrupt enable
    TA1CCTL0 |= CCIE; //
    btnPress = 1;
    P1IFG &= ~BIT1; // Clear flag
}
