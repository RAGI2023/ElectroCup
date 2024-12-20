#include <msp430g2553.h>
#include <stdint.h>
#include "res/res.h"
#include "ctrl/ctrl.h"

extern uint8_t freq_times;
extern uint8_t freq_change;
extern uint8_t res_chage;
extern uint16_t res;
extern uint16_t t1_stamp, time_stamp;
extern uint8_t phi_detect2;
extern uint16_t amp;
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
        freq_times++;
        freq_change = 1;
        if (freq_times > 5)
        {
            freq_times = 2;
        }
        set_freq(freq_times);
        //  P1OUT |= BIT0;
        P2IFG &= ~BIT6;
    }else if (P2IFG & BIT7)
    {
    //  P1OUT &= ~BIT0;
        amp++;
        if(amp >= 4)
        {
            amp = 1;
        }
        set_volt(amp-1);
        P2IFG &= ~BIT7;
    }
}
