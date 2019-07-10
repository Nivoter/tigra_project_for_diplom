
#include <ADC.h>


//static adcsample_t samples1[ADC1_NUM_CHANNELS * ADC1_BUF_DEPTH];


static const GPTConfig gpt4cfg1 = {

                                   .frequency = 100000,

                                   .callback = NULL,

                                   .cr2 = TIM_CR2_MMS_1,

                                   .dier = 0U

};

uint16_t value = 0;
uint16_t current_value = 0;
int16_t DinCur[2] = {0};
int16_t DinADC[2] = {0};
//static void adccallback(ADCDriver *adcp, adcsample_t *buffer, size_t n)
//{
//  (void)adcp;
//  value = buffer[0];
//  current_value = buffer[1];
//
//  (void)n;
//}
//
//
//
//static adcerrorcallback (ADCDriver *adcp, adcerror_t err)
//{
//  (void)adcp;
//  (void)err;
//}
//
//static const ADCConversionGroup adcgrpcfg1 =
//{
//   .circular = true,
//   .num_channels = ADC1_NUM_CHANNELS,
//   .end_cb = adccallback,
//   .error_cb = adcerrorcallback,
//   .cr1 = 0,
//   .cr2 = ADC_CR2_EXTEN_RISING | ADC_CR2_EXTSEL_SRC(0b1101),
//   .smpr1 = ADC_SMPR1_SMP_AN10(ADC_SAMPLE_144),
//   .smpr2 = ADC_SMPR2_SMP_AN9(ADC_SAMPLE_144),
//   .sqr1 = ADC_SQR1_NUM_CH(ADC1_NUM_CHANNELS),
//   .sqr2 = 0,
//   .sqr3 = ADC_SQR3_SQ1_N(ADC_CHANNEL_IN9)|
//           ADC_SQR3_SQ2_N(ADC_CHANNEL_IN13)
//};

void initADC(void)
{
//    gptStart(&GPTD6, &gpt4cfg1);
//    adcStart(&ADCD1, NULL);
    commonADC1UnitInit ();
//    palSetLineMode(LINE_ADC123_IN13,PAL_MODE_INPUT_ANALOG);
//    palSetLineMode(LINE_ADC12_IN9, PAL_MODE_INPUT_ANALOG);
//    adcStartConversion(&ADCD1, &adcgrpcfg1, samples1, ADC1_BUF_DEPTH);
//    gptStartContinuous(&GPTD6,gpt4cfg1.frequency/1000);
    DinADC[0] = (commonADC1UnitGetValue(1)-1100-1000)/10;
    DinCur[0] = (commonADC1UnitGetValue(2) - 1800)/70;
}



#define LPF_K_CUR   (float)0.6
#define LPF_K_POS   (float)0.25

int16_t get_current(void)
{
    DinCur[1] = abs(((commonADC1UnitGetValue(2) - 1875)*100)/7)-600;
    if(DinCur[1]<0)
    {
        DinCur[1] = 0;
    }

    DinCur[0] = DinCur[1]*LPF_K_CUR + (1 - LPF_K_CUR)*DinCur[0];
//    prev_dirCur = DinCur[0];
    return DinCur[0];
//    return DoutCur;
}



int16_t get_wheel_pos(void)        //WORKING
{
    DinADC[1] = (commonADC1UnitGetValue(1)-1100-1000)/10; //ñáèëñÿ ÀÖÏ
//    DinADC[1] = (value-3010)/10;
    DinADC[0] = DinADC[0] - (LPF_K_POS*(DinADC[0]-DinADC[1]));
    return DinADC[0];
//    return DoutADC;
}
