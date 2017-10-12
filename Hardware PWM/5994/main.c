#include <msp430.h> 
#include <config.h>

int count=5;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //disables high impedence mode

    LED2INIT;
    LED2OFF;
    BTN1INIT;
    BTN2INIT;

    //Sets up PWM (P1.0, same as LED)
    P1SEL1&=~BIT0;
    P1SEL0|=BIT0;
    P1DIR|=BIT0;

    TA0CTL = TASSEL_2 + MC_1; //Sets Timer A0 to SMCLK
    TA0CTL &= ~(TBIE | TBIFG); //Disable TBIV and flag

    TA0CCTL0 = CCIE; //Enables CCR0 interrupt
    TA0CCR0 = 1000; //Sets CCR0 to 1 kHz

    TA0CCTL1 = CCIE+OUTMOD_7; //Enables CCR1 interrupt
    TA0CCR1 = 500; //Sets CCR1 to .5 kHz

    _BIS_SR(LPM0_bits+GIE);

    while(1);

}

#pragma vector=PORT5_VECTOR
__interrupt void button (void)
{
    if(!BTN1UP)
    {
        if(count==10)
        {
            count=0;
            TA0CCR1=0;
        }
        else
        {
            count++;
            TA0CCR1 = count*100; //Increments CCR1 by .1 kHz
            //TA0CCR1 = exp(count)/44; //exponentially increases CCR1
        }
    }
    else
    {
        if(count==0)
        {
            count=10;
            TA0CCR1=1000;
        }
        else
        {
            count--;
            TA0CCR1 = count*100; //Increments CCR1 by .1 kHz
            //TA0CCR1 = exp(count)/44; //exponentially increases CCR1
        }
    }


    LED2SWITCH;

    BTN1CLR;//clear ifg
    BTN2CLR;
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void timerA (void)
{
    TA0IV=0;
}
#pragma vector=TIMER0_A1_VECTOR
__interrupt void timerA1 (void)
{
    TA0IV=0;
}
