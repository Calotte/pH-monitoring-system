#include"iocc2530.h"
#include"OnBoard.h"

#define uint unsigned int 
#define uchar unsigned char

 

 

#define ON 0x01  //��ȡ�ɹ�����0x00��ʧ�ܷ���0x01
#define OFF 0x00


//unsigned char temp; //����pH��Ϣ
float AvgTemp;

void pHDelay(uint k);
float getpH(void);
//void InitUART0(void);

//ʱ��Ƶ��Ϊ32M
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
    ADCCON1 |= 0x40; //������ͨ��ADC
    while(!(ADCCON1 & 0x80)); //�ȴ�ADת�����
    value = ADCL >> 4; //ADCL�Ĵ�����2λ��Ч
   value |= ADCH <<4; 
    AdcValue += value; //AdcValue����ֵΪ4��ADֵ֮��
  }
     value = AdcValue >> 2; //�ۼӳ���4���õ�ƽ��ֵ
  return value;
}
/*void InitUART0(void)
{
     PERCFG = 0x00; //λ��1 P0��
     P0SEL = 0x3c; //P0��������
     
     P2DIR &= ~0XC0; //P0������ΪUART0 
        U0CSR |= 0x80; //��������ΪUART��ʽ
        U0GCR |= 9; 
        U0BAUD |= 59; //��������Ϊ19200

     UTX0IF = 1; //UART0 TX�жϱ�־��ʼ��λ1 
        U0CSR |= 0X40; //�������
        IEN0 |= 0x84; //�����жϣ������ж�

}*/