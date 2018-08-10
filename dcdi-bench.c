
#include  <msp430g2553.h>

#define SMCLK_FREQ      1048000
#define PERIOD          SMCLK_FREQ/200/2        // 200Hz charging coil freq (2400 RPM crankshaft)
#define DEAD_CLKS       20*SMCLK_FREQ/1000000   // Dead time 10us on both channels (20 total)

unsigned char cnt;

__attribute__((interrupt(TIMER1_A0_VECTOR))) void 
TIMER1_A0_ISR (void)
{
    volatile unsigned short int i;

    if(cnt++ != 4)
        return;

    // divide by 5 (only handle each 5th irq)

    cnt = 0;

    P2OUT &= ~0x4; // trigger coil voltage goes high

    // FIXME: this must be the width of the trigger cam on the flywheel (~22 degrees depending on the flywheelmodel)
    for(i=0;i<100;i++) 
        ;

    P2OUT |= 0x4; // trigger coil voltage goes low
}

void 
main()
{
    cnt = 0;
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    P2DIR |= 0x16;
    P2SEL |= 0x12;

    P2OUT |= 0x4;

    TA1CCR0 = PERIOD;
    TA1CCTL0 = CCIE;
    TA1CCTL1 = OUTMOD_6;                    // toggle/set
    TA1CCTL2 = OUTMOD_2;                    // toggle/reset
    TA1CCR1 = PERIOD/2 - DEAD_CLKS;
    TA1CCR2 = PERIOD/2 + DEAD_CLKS;
    TA1CTL = TASSEL_2 | TAIE | MC_3;        // TASSEL_2 = SMCLK, MC_3 = up/down mode, TAIE = enable overflow interrupt

    _BIS_SR(LPM0_bits | GIE);               // Enter LPM0 + enable interrupts
}
