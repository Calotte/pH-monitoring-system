#ifndef SENSOR_H
#define SENSOR_H
#include <hal_types.h>



extern float getpH(void);
/*
#define SET_IO_PORT_DIR(port,pin,dir) \
  do                              \
{                                 \
  if(dir==IO_OUT)                 \
    P##port##DIR |=(0X01<<pin);   \
  else                           \
    P##port##DIR &= ~(0X01<<pin);\
}while(0)
//  port={0,1,2}, pin={0,..,7} and dir is one of them:
#define IO_IN   0
#define IO_OUT  1

#define ADC_ENABLE_CHANNEL(ch)   ADCCFG |=  (0x01 << ch)
#define ADC_REF_AVDD        0x80     // AVDD_SOC pin
#define ADC_REF_P0_7        0x40     // External reference on AIN7 pin
#define ADC_SINGLE_CONVERSION(settings) st( ADCCON3 = (settings); )
#define ADC_12_BIT          0x30     // 512 decimation rate
#define ADC_AIN6            0x06     // single ended P0_6

#define ADC_STOP()  st( ADCCON1 |= 0x30; )

#define ADC_SAMPLE_SINGLE()       \
  st( ADC_STOP();                 \
      ADCCON1 |= 0x40; )

*/

 

#endif