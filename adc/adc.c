#include "adc/adc.h"
#include "fft/fft.h"
#include <msp430.h>
#include <math.h>

#define SAMPLE_RATE (1.0/8 * 1e6) //Hz
// samples_cnt 是2的整数次幂！
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
              ADC10SSEL_2 + // ADC clock source = MCLK 1MHz   ADC10OCS 5MHz
              ADC10DIV_7 +  //   8 No prescaler (1:1)
              CONSEQ_2;     // repeating single channel mode
  ADC10AE0 = BIT0;          // Enable P1.0 as analog input
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

sin_Data getLissaData(uint16_t *buffer, uint16_t samples_cnt) {
    sin_Data y;
  // Step 1: 将实数 ADC 数据转换为复数数组
    complex input_data[samples_cnt];
    int i;
    for (i = 0; i < samples_cnt; i++) {
        input_data[i].real = buffer[i];
        input_data[i].imag = 0;
    }

    // Step 2: 调用 FFT 计算频域数据
    fft(samples_cnt, input_data);

    // Step 3: 提取幅值、相位和频率
    float magnitude[samples_cnt];
    float phase[samples_cnt];
    float frequencies[samples_cnt / 2]; // 只需要前半部分频率
    int max_index = 0;        // 用于找到主频率
    float max_magnitude = 0;
    int k;
    for (k = 0; k < samples_cnt; k++) {
        // 幅值计算
        magnitude[k] = sqrt(input_data[k].real * input_data[k].real +
                            input_data[k].imag * input_data[k].imag);

        // 相位计算
        phase[k] = atan2(input_data[k].imag, input_data[k].real);

        // 频率计算（仅前半部分）
        if (k < samples_cnt / 2) {
            frequencies[k] = (float)k * SAMPLE_RATE / samples_cnt;
        }

        // 找到最大幅值对应的频率（主频）
        if (k < samples_cnt / 2 && magnitude[k] > max_magnitude) {
            max_magnitude = magnitude[k];
            max_index = k;
        }
    }

    y.amp = max_magnitude;
    y.omega = 2*PI*frequencies[max_index];
    y.delta_phi = phase[max_index];
    // Step 4: 主频参数
    // float A = max_magnitude;         // 振幅
    // float f = frequencies[max_index]; // 主频
    // float phi = phase[max_index];    // 相位
    return y;
}