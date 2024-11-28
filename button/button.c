#include <msp430g2553.h>

void button_init(void)
{
    // P1DIR |= BIT0;
    // P1OUT &= ~BIT0;
    
    P2REN &= ~(BIT6 | BIT7); // P2.6 7 pullup
    P2IE |= BIT6 | BIT7; // interrupt enabled
    P2IES |= BIT3; // P2.3 Hi-low edge
    P2IFG &= ~(BIT6 | BIT7); // P1.3 IFG cleared

    _BIS_SR(GIE); // Enable global interrupt

}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    
 if (P2IFG & BIT6)
 {
    //  P1OUT |= BIT0;
     P2IFG &= ~BIT6;
 }else if (P2IFG & BIT7)
 {
    //  P1OUT &= ~BIT0;
     P2IFG &= ~BIT7;
 }
}
