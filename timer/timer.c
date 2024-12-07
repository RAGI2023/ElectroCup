#include "timer.h"
#include "msp430g2553.h"
#include "stdint.h"
// SMCLK = 1MHz

uint16_t flag1 = 0, flag2 = 0;
uint8_t index1 = 0, index2 = 0; //记录数组的索引
uint16_t t1[2] = {0,0}; //第一个通道
uint16_t t2[2] = {0, 0}; //第二个通道

uint8_t phi_detect1 = 1; //相位差检测标志位
uint8_t phi_detect2 = 1; //相位差检测标志位
uint8_t t1_stamp = 0;
uint8_t t2_stamp = 0;

uint16_t T1 = 0, T2 = 0; //周期

// 时间戳 65535max
uint16_t time_stamp = 0;
double f1 = 0, f2 = 0; //kHZ

void CalculateT(void)
{
	
	// T1 = t12 - t11;
	while(!flag1 || !t1[1]); //等待记录完毕
	// FREQ_OFF;
	P1IE &= ~(BIT1);
	T1 = t1[1] - t1[0];
	T1 = T1 > 65535 - T1 ? 65536 - T1 : T1; 

	while(!flag2 );
	// T2 = t22 - t22;
	P1IE &= ~(BIT2);
	T2 = t2[1] - t2[0];
	T2 = T2 > 65535 - T2 ? 65536 - T2 : T2; 
//	FREQ_ON;
	P1IE |= BIT1;
}

void CalculateF(void)
{
	f1 = 1e-3 / T1;
	f2 = 1e-3 / T2;
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

//#pragma vector=TIMER0_A0_VECTOR
//__interrupt void freq_capture(void)
//{
//    capt1 = TA0CCR2;
//    P1OUT |= BIT0;
//}

void freq_init(void)
{
    
    // P1OUT &= ~BIT0;
    
	// 1.1 1.2 输入方波
    P1REN &= ~(BIT1 | BIT2); //  pullup
    P1IE |= BIT1 | BIT2; // interrupt enabled
    // P2IES |= BIT3; //  low edge
    P1IFG &= ~(BIT1 | BIT2); // IFG cleared

    _BIS_SR(GIE); // Enable global interrupt

}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	// 
	if (P1IFG & BIT1)
	{
		 if(phi_detect1){
		 	t1_stamp = time_stamp;
			return ;
		 }
		flag1 = 1;
		t1[index1] = time_stamp;
		// flag1 = !flag1;
		index1 = (index1 + 1) % 2;
		P1IFG &= ~BIT1;
		P1OUT ^= BIT3;

	}
	if (P1IFG & BIT2)
	{
		 if (phi_detect2)
		 {
		 	t2_stamp = time_stamp;
			return ;
		 }
		flag2 = 1;
		t2[index2] = time_stamp;
		// flag2 = !flag2;
		index2 = (index2 + 1) % 2;
		P1IFG &= ~BIT2;
		P1OUT ^= BIT4;
		P1IE &= ~(BIT2);
	//	P1OUT ^= BIT0;
 }
}

