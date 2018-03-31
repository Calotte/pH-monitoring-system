//initUART_Timer.h
//2010-09-09 PM
//By Hiber From  FEIBIT  Electronics LTD.
// www.feibit.com


#include <ioCC2530.h>
#define uint unsigned int
#define uchar unsigned char
#define LED1   P1_0	        //定义LED1为P10口控制
#define LED2   P1_1	        //定义LED2为P11口控制
#define LED3   P1_4	        //定义LED3为P14口控制
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

#define DISABLE_ALL_INTERRUPTS() (IEN0 = IEN1 = IEN2 = 0x00)//三个



void InitT3(void)
{
  T3CCTL0 = 0X44;        // T3CCTL0 (0xCC)，CH0 中断使能，CH0 比较模式
  T3CC0 = 0xFA;          // T3CC0设置为250
  T3CTL |= 0x9A;         // 启动T3计数器，计数时钟为16分频。使用MODULO模式
  IEN1 |= 0X08;     
  IEN0 |= 0X80;          //开总中断，开T3中断
}


void InitUART0(void)
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

}






