#include "adc/adc.h"
#include <msp430.h>
#include <stdint.h>



void ADC_init_intref_repeating(
    uint16_t *buffer, uint16_t samples_cnt) // Init ADC single channel oneshot
                                            // with internal reference 2.5V
{
  ADC10CTL0 &= ~ENC;
  // ADC10CTL0 = MSC + // multiple samples
  //             SREF_1 +  // internal reference
  //             ADC10SHT_2 + // sample and hold time
  // REF2_5V + // VREF = 2.5V
  // REFON + // internal reference on
  // ADC10ON; // ADC on
  ADC10CTL0 = MSC +        // multiple samples
              SREF_0 +     // internal reference
              ADC10SHT_2 + // sample and hold time
              REF2_5V +    // VREF = 2.5V
              REFON +      // internal reference on
              ADC10ON;     // ADC on
  ADC10CTL1 = INCH_0 +     // channel 0 = P1.0
              SHS_0 +
              ADC10SSEL_3 + // ADC clock source = SMCLK 1MHz   ADC10OCS 5MHz
              ADC10DIV_7 +  //   8 No prescaler (1:1)
              CONSEQ_2;     // repeating single channel mode
  ADC10AE0 = BIT0;          // Enable P1.0 as analog input
  // __delay_cycles(1000);
  ADC10DTC0 = ADC10CT;      // Contioniously transfer
  ADC10DTC1 = samples_cnt;  // Number of conversion
  while (ADC10CTL1 & BUSY)
    ;
  ADC10SA = (int)buffer;      // Start address of buffer
  ADC10CTL0 |= ENC + ADC10SC; // Enable conversion
}

void ADC_init_intref(
    void) // Init ADC single channel oneshot with internal reference 2.5V
{
  ADC10CTL0 &= ~ENC;
  ADC10CTL0 = SREF_0 +              // internal reference
              ADC10SHT_3 +          // sample and hold time
              REF2_5V +             // VREF = 2.5V
              REFON +               // internal reference on
              ADC10ON;              // ADC on
  ADC10CTL1 = INCH_0 +              // channel 0 = P1.0
              SHS_0 + ADC10SSEL_0 + // ADC clock source = ADC10OCS 5MHz
              ADC10DIV_0 +          // No prescaler (1:1)
              CONSEQ_0;             // single channel mode
  ADC10AE0 = BIT0;                  // Enable P1.0 as analog input
  ADC10CTL0 |= ENC;                 // Enable conversion

  // ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE + MSC;  // Set
  // ADC module ADC10CTL1 = CONSEQ_0 + INCH_10 + ADC10DIV_7; // Set ADC module
  // ADC10DTC0 = ADC10CT;                                                // Set
  // ADC module

  // ADC10CTL0 |= ENC + ADC10SC;                                         //
  // Start ADC + sampling

  // __enable_interrupt();
}

uint16_t getADC(void) {
  ADC10CTL0 |= ADC10SC; // Strat conversion
  while (ADC10CTL1 & ADC10BUSY)
    ;              // Wait while ADc is not ready
  return ADC10MEM; // return ADC result
}

