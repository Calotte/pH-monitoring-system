#include"Sensor.h"
#include"ioCC2530.h"
 #include "OnBoard.h" 
#include "ad.h"

  


#define uint unsigned int 
#define uchar unsigned char



 

float getpH(void);
void pHDelay(uint k);


//时钟频率为32M
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
    ADCCON3 = 0x70;   //外部参考电压、512抽取率、AIN0  70
    ADCCON1 |= 0x30;
    ADCCON1 |= 0x40; //开启单通道ADC
    while(!(ADCCON1 & 0x80)); //等待AD转换完成
    value = ADCL >> 4; //ADCL寄存器低2位无效
    value |= ADCH << 4; 
    AdcValue += value; //AdcValue被赋值为4次AD值之和
  }
     value = AdcValue >> 2; //累加除以4，得到平均值
     return (5.00*value/2047); //根据AD值，计算出实际的pH
}

/*
float getpH(void)
{
  uchar i = 0;
  uint value = 0;
  long uint AdcValue = 0;//防止溢出
  float vol = 0.0;
  SET_IO_PORT_DIR(0,6,IO_IN);//设置P0.6为输入模式
SET_IO_PORT_DIR(0,7,IO_IN)
  ADC_ENABLE_CHANNEL(6);    //使能通道6作为ADC的采样通道
  for(i=0; i<4; i++)
  {
    ADC_SINGLE_CONVERSION(ADC_REF_P0_7 | ADC_12_BIT | ADC_AIN6);//外部AIN7参考电压,12位，6通道
    ADC_SAMPLE_SINGLE(); 
    while(!(ADCCON1 & 0x80));              //等待AD转换完成
    value = ADCL>>4;                      
    value |= (((uint)ADCH) << 4);         
    AdcValue += value;  
  }
  AdcValue = AdcValue >> 2;                 //累加除以4，得到平均值
  vol = (float)(5.0*AdcValue/2047);  //换算成电压值
  return vol;
}
*/