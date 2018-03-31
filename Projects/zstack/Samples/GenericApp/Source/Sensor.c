#include"Sensor.h"
#include"ioCC2530.h"
 #include "OnBoard.h" 
#include "ad.h"

  


#define uint unsigned int 
#define uchar unsigned char



 

float getpH(void);
void pHDelay(uint k);


//ʱ��Ƶ��Ϊ32M
void pHDelay(uint k)
{
 
  MicroWait(k);

}


float getpH(void){
 
  short int i;
  int AdcValue;        
  int value;

  AdcValue = 0;
  for( i = 0; i < 4; i++ )
  {   
    ADCCON3 = 0x70;   //�ⲿ�ο���ѹ��512��ȡ�ʡ�AIN0  70
    ADCCON1 |= 0x30;
    ADCCON1 |= 0x40; //������ͨ��ADC
    while(!(ADCCON1 & 0x80)); //�ȴ�ADת�����
    value = ADCL >> 4; //ADCL�Ĵ�����2λ��Ч
    value |= ADCH << 4; 
    AdcValue += value; //AdcValue����ֵΪ4��ADֵ֮��
  }
     value = AdcValue >> 2; //�ۼӳ���4���õ�ƽ��ֵ
     return (5.00*value/2047); //����ADֵ�������ʵ�ʵ�pH
}

/*
float getpH(void)
{
  uchar i = 0;
  uint value = 0;
  long uint AdcValue = 0;//��ֹ���
  float vol = 0.0;
  SET_IO_PORT_DIR(0,6,IO_IN);//����P0.6Ϊ����ģʽ
SET_IO_PORT_DIR(0,7,IO_IN)
  ADC_ENABLE_CHANNEL(6);    //ʹ��ͨ��6��ΪADC�Ĳ���ͨ��
  for(i=0; i<4; i++)
  {
    ADC_SINGLE_CONVERSION(ADC_REF_P0_7 | ADC_12_BIT | ADC_AIN6);//�ⲿAIN7�ο���ѹ,12λ��6ͨ��
    ADC_SAMPLE_SINGLE(); 
    while(!(ADCCON1 & 0x80));              //�ȴ�ADת�����
    value = ADCL>>4;                      
    value |= (((uint)ADCH) << 4);         
    AdcValue += value;  
  }
  AdcValue = AdcValue >> 2;                 //�ۼӳ���4���õ�ƽ��ֵ
  vol = (float)(5.0*AdcValue/2047);  //����ɵ�ѹֵ
  return vol;
}
*/