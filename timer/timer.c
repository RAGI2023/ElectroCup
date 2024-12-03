#include "msp430g2553.h"
#include "stdint.h"
// SMCLK = 1MHz

uint16_t capt1 = 0;


#define uint unsigned int

#define uchar unsigned char


uint PerVal;
uint PerVal2;
unsigned long int val;
unsigned long int val2;

// ʱ��� 65535max
uint16_t time_stamp = 0;

void timer_init(void)
{
    //Timer0_A3 ��ʼ��

////    P3SEL |= BIT0;
////    P3REN |= BIT0;
////    P3OUT &= ~BIT0;
////    P3SEL2 &= ~BIT0;
//
////    TA0CCTL0 = CAP + CM_1 + CCIE + SCS + CCIS_0; // TA0CCR0 Capture mode; CCIxA;
////    TA0CCTL1 = CAP + CM_3 + CCIE + SCS + CCIS_0;
//    TA0CCTL2 = CAP + CM_1 + CCIE + SCS + CCIS_0;
////    // Rising edge; interrupt enable
//    TA0CTL |= TASSEL_2 + MC_2 + TACLR + TAIE + ID_0; // SMCLK, Cont Mode; start timer



//    P1DIR&=~BIT1;  //��׽P1.1��
//    P1REN|=BIT1;
//    P1OUT&=~BIT1;
//    P1SEL |= BIT1;
    TA0CCTL0 |= CAP + CM_1 + CCIS_0 + SCS + CCIE + TAIE;//����ģʽ���������½�������ѡ��CCI0A��ͬ���������жϿ�
    TA0CTL |= TASSEL_2 + MC_2 + TACLR; //SMCLK=1M,��������ģʽ

//    P1DIR&=~BIT2; //��׽P1.2��
//    P1REN|=BIT2;
//    P1OUT&=~BIT2;
//    P1SEL |= BIT2;
//    TA0CCTL1 |= CAP + CM_3 + CCIS_0 + SCS + CCIE;//����ģʽ���������½�������ѡ��CCI1A��ͬ���������жϿ�
//    TA0CTL |= TASSEL_2 + MC_2 + ID_0 + TACLR; //SMCLK=1M,��������ģʽ
//    _EINT();//�ж���ʼλ

    P1DIR |= BIT0;

    // Timer1_A3��ʱ����ʼ�� ʱ���
    TA1CCR0 = 2 - 1;                //voir texte   1/1mhz * 50000  0.5s
    TA1CCTL0 = CCIE; // enable compare interrupt
    TA1CTL |= (TASSEL_2 | ID_0 | MC_1 + TACLR);   //source SMCLK, pas de predivision ID_0
//    TA1CTL |= TAIE;                 //autorisation interruption TAIE
    __enable_interrupt();



    // Timer

}

// Timer A1 CCR0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A(void)
{
    time_stamp++;
//    P1OUT ^= BIT1;
    TA1CTL&=~(TAIFG); //Reset the interrupt flag
}

//#pragma vector=TIMER0_A0_VECTOR
//__interrupt void freq_capture(void)
//{
//    capt1 = TA0CCR2;
//    P1OUT |= BIT0;
//}



#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)

{
    P1OUT |= BIT0;
  switch(TA0IV)
  {
    case 2:
      val = TA0CCR1 - PerVal;
      PerVal = TA0CCR1;

      break;
    case 4:
      val2 = TA0CCR1 - PerVal2;
      PerVal2 = TA0CCR1;
      break;
  }
}
