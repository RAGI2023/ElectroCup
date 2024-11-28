#include "msp430g2553.h"
#include <msp430.h>


int TXByteCtr = 1;
unsigned char PRxData;                     // Pointer to RX data
int Rx = 0;
unsigned char WHO_AM_I = 40;

void init_I2C(void);
void Transmit(void);
void Receive(void); 
// int main(void)
// {
  
//   init_I2C();
//   while(1){
//   //Transmit process
//   Rx = 0;
//   Transmit();
//   while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent

//   //Receive process
//   Rx = 1;
//   Receive();
//   while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
//   }
// }

//-------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C slave
// to the MSP430 memory. It is structured such that it can be used to receive
//-------------------------------------------------------------------------------
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
  if(Rx == 1){                              // Master Recieve?
  PRxData = UCB0RXBUF;                       // Get RX data
  __bic_SR_register_on_exit(CPUOFF);        // Exit LPM0
  }

  else{                                     // Master Transmit
  if (TXByteCtr)                            // Check TX byte counter
  {
      UCB0TXBUF = WHO_AM_I;                     // Load TX buffer
      TXByteCtr--;                            // Decrement TX byte counter
  }
  else
  {
      UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
      IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
      __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
  }
 }

}

void init_I2C(void) {
  P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  _DINT();
  IE2 |= UCB0RXIE;             //Enable RX interrupt
  IE2 |= UCB0TXIE;                          // Enable TX interrupt
  while (UCB0CTL1 & UCTXSTP);               // Ensure stop condition got sent
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB0CTL1 |= UCTXNACK;  // …Ë÷√∑¢ÀÕ NACK

  UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = 0x2F;                         // Slave Address is 069h
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
}

void Transmit(void){
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
}
void Receive(void){
   UCB0CTL1 &= ~UCTR ;                  // Clear UCTR
   while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    UCB0CTL1 |= UCTXSTT;                    // I2C start condition
    while (UCB0CTL1 & UCTXSTT);             // Start condition sent?
    UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
}
