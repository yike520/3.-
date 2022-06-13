#include <reg52.h> 
#include <intrins.h>
#include <stdio.h>
#include "delay.h"
//	操作方法
// 蓝牙发送字符 CX或cX 表示关闭x位灯， x取值1-8	 
// 蓝牙发送字符 OX或oX 表示关闭x位灯， x取值1-8

#define INIT 0xFF	   //定义常量
#define OPEN 0x22
#define CLOSE 0x33

unsigned long times_5ms=0;	   //定时计数
unsigned char Commd_Flag=INIT;		  //初始化命令接收标志
	  
sbit relay1=P1^0;	  	//初始化relay对应引脚
sbit relay2=P1^1;
sbit relay3=P1^2;
sbit relay4=P1^3;
sbit relay5=P1^4;
sbit relay6=P1^5;
sbit relay7=P1^6;
sbit relay8=P1^7;

void Init_Timer0(void);//定时器初始化

void UART_Init(void);			 //函数声明
void SendByte(unsigned char dat);
void SendStr(unsigned char *s,unsigned char length);

void main (void)
{

	Init_Timer0();        //定时器0初始化
	UART_Init();		   //蓝牙 串口 波特率9600

	P1=0x00;
	DelayMs(200);          //延时有助于稳定	 上电检测下硬件
	P1=0xff;
	
	while(1)         //主循环
	{
			;//蓝牙的接收处理 均在中断中处理 请查看串口中断
	}
}

void Init_Timer0(void)
{
//**All notes can be deleted and modified**//
  TMOD |= 0x10;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响		     
	TH0=(65536-10000)/256;		  //重新赋值 20ms
	TL0=(65536-10000)%256;
	EA=1;            //总中断打开
	ET0=1;           //定时器中断打开
	TR0=1;           //定时器开关打开
}
void UART_Init(void)
{
//**All notes can be deleted and modified**//
    SCON  = 0x05;		        // SCON: 模式 1, 8-bit UART, 使能接收  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit 重装
    TH1   = 0xFD;               // TH1:  重装值 9600 波特率 晶振 11.0592MHz
	TL1 = TH1;  
    TR1   = 1;                  // TR1:  timer 1 打开                         
    EA    = 1;                  //打开总中断
    ES    = 1;                  //打开串口中断
} 

void Timer0_isr(void) interrupt 1 
{
	TH0=(65536-5000)/256;		  //重新赋值 5ms
	TL0=(65536-5000)%256;
	times_5ms++;				  //定时5ms

}

void UART_SER (void) interrupt 4 	//串行中断服务程序
{
	unsigned char R_buf;
	if(RI)                        //判断是接收中断产生
	{
		RI=0;                      //标志位清零
		R_buf=SBUF;
		if((R_buf=='O')||(R_buf=='o'))
		{
			Commd_Flag=OPEN;	//接收到打开灯标志
		}
		else if((R_buf=='C')&&(R_buf=='c'))
		{
			Commd_Flag=CLOSE;	//接收到 关闭灯标志
		}
		else if((R_buf!='1')&&(R_buf!='2')&&(R_buf!='3')&&(R_buf!='4')&&(R_buf!='5')&&(R_buf!='6')&&(R_buf!='7')&&(R_buf!='8')&&(R_buf!='a')&&(R_buf!='A'))
		{
			Commd_Flag=INIT;	 //否则 初始化接受标志
		}

		if(Commd_Flag==OPEN)			//根据命令值进行打开相应的灯
		{
			switch(R_buf)
			{
				case '1':relay1=0;Commd_Flag=INIT;break;	//打开相应的灯 并恢复命令标志
				case '2':relay2=0;Commd_Flag=INIT;break;			
				case '3':relay3=0;Commd_Flag=INIT;break;			
				case '4':relay4=0;Commd_Flag=INIT;break;			
				case '5':relay5=0;Commd_Flag=INIT;break;	
				case '6':relay6=0;Commd_Flag=INIT;break;			
				case '7':relay7=0;Commd_Flag=INIT;break;			
				case '8':relay8=0;Commd_Flag=INIT;break;
				case 'A':relay1=0;relay2=0;relay3=0;relay4=0;relay5=0;relay6=0;relay7=0;relay8=0;Commd_Flag=INIT;break;
				case 'a':relay1=0;relay2=0;relay3=0;relay4=0;relay5=0;relay6=0;relay7=0;relay8=0;Commd_Flag=INIT;break;
				default:break;						 //此处错误判断 不可恢复命令标准
			}
		}
		else if(Commd_Flag==CLOSE)//根据命令值进行关闭相应的灯
		{
			switch(R_buf)
			{
				case '1':relay1=1;Commd_Flag=INIT;break;	//关闭相应的灯 并恢复命令标志
				case '2':relay2=1;Commd_Flag=INIT;break;			
				case '3':relay3=1;Commd_Flag=INIT;break;			
				case '4':relay4=1;Commd_Flag=INIT;break;			
				case '5':relay5=1;Commd_Flag=INIT;break;	
				case '6':relay6=1;Commd_Flag=INIT;break;			
				case '7':relay7=1;Commd_Flag=INIT;break;			
				case '8':relay8=1;Commd_Flag=INIT;break;
				case 'A':relay1=1;relay2=1;relay3=1;relay4=1;relay5=1;relay6=1;relay7=1;relay8=1;Commd_Flag=INIT;break;
				case 'a':relay1=1;relay2=1;relay3=1;relay4=1;relay5=1;relay6=1;relay7=1;relay8=1;Commd_Flag=INIT;break;					
				default:break;						 //此处错误判断 不可恢复命令标准
			}
		}
		SBUF=R_buf;				   //返回接收到的数据
	}
	if(TI)  //如果是发送标志位，清零
	TI=0;
} 

