#include "msp430g2553.h"
#include "stdint.h"
// SMCLK = 1MHz

uint8_t flag1 = 0, flag2 = 0; //是否记录过时间
uint16_t t11, t12; //第一个通道
uint16_t t21, t22; //第二个通道

uint16_t T1 = 0, T2 = 0; //周期

// 时间戳 65535max
uint16_t time_stamp = 0;

void CalculateT(void)
{
	T1 = t12 - t11;
	T2 = t22 - t21;
}

uint16_t get_time_stamp(void)
{
	return time_stamp;
}

void timer_init(void)
{
	// Timer1_A3计时器初始化 时间戳
	TA1CCR0 = 100 - 1;                //voir texte   1/1mhz * 50000  0.5s
	TA1CCTL0 = CCIE; // enable compare interrupt
	TA1CTL |= (TASSEL_2 | ID_0 | MC_1 + TACLR);   //source SMCLK, pas de predivision ID_0
	TA1CTL |= TAIE;                 //autorisation interruption TAIE
    __enable_interrupt();

}

// Timer A1 CCR0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A(void)
{
    time_stamp++;
    TA1CTL&=~(TAIFG); //Reset the interrupt flag
}

//#pragma vector=TIMER0_A0_VECTOR
//__interrupt void freq_capture(void)
//{
//    capt1 = TA0CCR2;
//    P1OUT |= BIT0;
//}

void freq_init(void)
{
    P1DIR |= BIT0;
    // P1OUT &= ~BIT0;
    
	// 1.1 1.2 输入方波
    P1REN &= ~(BIT1 | BIT2); //  pullup
    P1IE |= BIT1 | BIT2; // interrupt enabled
    // P2IES |= BIT3; //  Hi-low edge
    P1IFG &= ~(BIT1 | BIT2); // IFG cleared

    _BIS_SR(GIE); // Enable global interrupt

}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	P1OUT ^= BIT0;
 if (P1IFG & BIT1)
 {
	if(flag1){
		t11 = time_stamp;
		flag1 = 0;
	}else {
		t12 = time_stamp;
		flag1 = 1;
	}
	P2IFG &= ~BIT6;
 }else if (P1IFG & BIT2)
 {
    if(flag2){
		t21 = time_stamp;
		flag2 = 0;
	}else {
		t22 = time_stamp;
		flag2 = 1;
	}
    P2IFG &= ~BIT7;
 }
}

