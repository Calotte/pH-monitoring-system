#include"iocc2530.h"
#include"OnBoard.h"

#define uint unsigned int 
#define uchar unsigned char

 

 

#define ON 0x01  //读取成功返回0x00，失败返回0x01
#define OFF 0x00


//unsigned char temp; //储存pH信息
float AvgTemp;

void pHDelay(uint k);
float getpH(void);
//void InitUART0(void);

//时钟频率为32M
void pHDelay(uint k)
{
 /* uint i,j;
  for(i=0;i<k;i++)
  for(j=0;j<2;j++);*/
  MicroWait(k);

}


float getpH(void){
  short int i;
  int AdcValue;
 int value;

  AdcValue = 0;
  for( i = 0; i < 4; i++ )
  {
    ADCCON3 = 0x70;
    ADCCON1 |= 0x30;
    ADCCON1 |= 0x40; //开启单通道ADC
    while(!(ADCCON1 & 0x80)); //等待AD转换完成
    value = ADCL >> 4; //ADCL寄存器低2位无效
   value |= ADCH <<4; 
    AdcValue += value; //AdcValue被赋值为4次AD值之和
  }
     value = AdcValue >> 2; //累加除以4，得到平均值
  return value;
}
/*void InitUART0(void)
{
     PERCFG = 0x00; //位置1 P0口
     P0SEL = 0x3c; //P0用作串口
     
     P2DIR &= ~0XC0; //P0优先作为UART0 
        U0CSR |= 0x80; //串口设置为UART方式
        U0GCR |= 9; 
        U0BAUD |= 59; //波特率设为19200

     UTX0IF = 1; //UART0 TX中断标志初始置位1 
        U0CSR |= 0X40; //允许接收
        IEN0 |= 0x84; //开总中断，接收中断

}*/