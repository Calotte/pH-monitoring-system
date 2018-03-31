//initUART_Timer.h
//2010-09-09 PM
//By Hiber From  FEIBIT  Electronics LTD.
// www.feibit.com


#include <ioCC2530.h>
#define uint unsigned int
#define uchar unsigned char
#define LED1   P1_0	        //����LED1ΪP10�ڿ���
#define LED2   P1_1	        //����LED2ΪP11�ڿ���
#define LED3   P1_4	        //����LED3ΪP14�ڿ���
// Data
typedef unsigned char       BYTE;

// Unsigned numbers
typedef unsigned char       UINT8;
typedef unsigned char       INT8U;

typedef unsigned short      INT16U;
typedef unsigned long       UINT32;
typedef unsigned long       INT32U;

// Signed numbers
typedef signed char         INT8;

typedef signed long         INT32;

#define DISABLE_ALL_INTERRUPTS() (IEN0 = IEN1 = IEN2 = 0x00)//����



void InitT3(void)
{
  T3CCTL0 = 0X44;        // T3CCTL0 (0xCC)��CH0 �ж�ʹ�ܣ�CH0 �Ƚ�ģʽ
  T3CC0 = 0xFA;          // T3CC0����Ϊ250
  T3CTL |= 0x9A;         // ����T3������������ʱ��Ϊ16��Ƶ��ʹ��MODULOģʽ
  IEN1 |= 0X08;     
  IEN0 |= 0X80;          //�����жϣ���T3�ж�
}


void InitUART0(void)
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

}






