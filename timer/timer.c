#include "timer.h"
#include "msp430g2553.h"
#include "stdint.h"
// SMCLK = 8MHz

uint8_t phi_detect1 = 0; //相位差检测标志位
uint8_t phi_detect2 = 0; //相位差检测标志位
uint8_t t1_stamp = 0;
uint8_t t2_stamp = 0;

uint16_t T1 = 0; //周期

// 时间戳 65535max
uint16_t time_stamp = 0;

// void CalculateT(void)
// {
// 	// T1 = t12 - t11;
// 	while(!flag1 || !t1[1]); //等待记录完毕
// 	// FREQ_OFF;
// 	P1IE &= ~(BIT1);
// 	T1 = t1[1] - t1[0];
// 	T1 = T1 > 65535 - T1 ? 65536 - T1 : T1; 
// 	while(!flag2 );
// 	// T2 = t22 - t22;
// 	P1IE &= ~(BIT2);
// 	T2 = t2[1] - t2[0];
// 	T2 = T2 > 65535 - T2 ? 65536 - T2 : T2; 
// //	FREQ_ON;
// 	P1IE |= BIT1;
// }

float CalculatePhi(void)
{
	if (t1_stamp > t2_stamp)
	{
		return (float)(65536 + t1_stamp - t2_stamp) / T1 * 360 * -1.0;
	}
	return (float)(t2_stamp - t1_stamp) / T1 * 360;
}

float Phi_Detect(void)
{
	phi_detect1 = 1;
	phi_detect2 = 1;
	// FREQ_ON;
	while(!phi_detect1 && !phi_detect2);
	FREQ_OFF;
	return CalculatePhi();
}

uint16_t get_time_stamp(void)
{
	return time_stamp;
}

void timer_init(void)
{
	// Timer1_A3计时器初始化 时间戳
	TA1CCR0 = 10 - 1;                //voir texte   1/8mhz * 
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
	P1OUT ^= BIT0;
    TA1CTL&=~(TAIFG); //Reset the interrupt flag
}

void freq_init(uint16_t T)
{
    T1 = T; //x波的周期
    
	// 1.1 1.2 输入方波
    P1REN &= ~(BIT1); //  pullup
    P1IE |= BIT1; // interrupt enabled
    // P2IES |= BIT3; //  low edge
    P1IFG &= ~(BIT1); // IFG cleared

	P2REN &= ~(BIT0); //  pullup
    P2IE |= BIT0; // interrupt enabled
    // P2IES |= BIT3; //  low edge
    P2IFG &= ~(BIT0); // IFG cleared

    _BIS_SR(GIE); // Enable global interrupt

}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	// 
	if (P1IFG & BIT1)
	{
		if(phi_detect1)
		{
			t1_stamp = time_stamp;
			phi_detect1 = 0;
		}
		P1IFG &= ~BIT1;
		P1OUT ^= BIT3;
	}
// 	if (P1IFG & BIT2)
// 	{
// 		 if (phi_detect2)
// 		 {
// 		 	t2_stamp = time_stamp;
// 			phi_detect2 = 0;
// 		 }
// 		P1IFG &= ~BIT2;
// 		P1OUT ^= BIT4;
//  }
}

