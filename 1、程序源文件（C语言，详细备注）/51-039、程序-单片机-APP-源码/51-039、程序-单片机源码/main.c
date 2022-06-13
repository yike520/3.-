#include <reg52.h> 
#include <intrins.h>
#include <stdio.h>
#include "delay.h"
//	��������
// ���������ַ� CX��cX ��ʾ�ر�xλ�ƣ� xȡֵ1-8	 
// ���������ַ� OX��oX ��ʾ�ر�xλ�ƣ� xȡֵ1-8

#define INIT 0xFF	   //���峣��
#define OPEN 0x22
#define CLOSE 0x33

unsigned long times_5ms=0;	   //��ʱ����
unsigned char Commd_Flag=INIT;		  //��ʼ��������ձ�־
	  
sbit relay1=P1^0;	  	//��ʼ��relay��Ӧ����
sbit relay2=P1^1;
sbit relay3=P1^2;
sbit relay4=P1^3;
sbit relay5=P1^4;
sbit relay6=P1^5;
sbit relay7=P1^6;
sbit relay8=P1^7;

void Init_Timer0(void);//��ʱ����ʼ��

void UART_Init(void);			 //��������
void SendByte(unsigned char dat);
void SendStr(unsigned char *s,unsigned char length);

void main (void)
{

	Init_Timer0();        //��ʱ��0��ʼ��
	UART_Init();		   //���� ���� ������9600

	P1=0x00;
	DelayMs(200);          //��ʱ�������ȶ�	 �ϵ�����Ӳ��
	P1=0xff;
	
	while(1)         //��ѭ��
	{
			;//�����Ľ��մ��� �����ж��д��� ��鿴�����ж�
	}
}

void Init_Timer0(void)
{
//**All notes can be deleted and modified**//
  TMOD |= 0x10;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��		     
	TH0=(65536-10000)/256;		  //���¸�ֵ 20ms
	TL0=(65536-10000)%256;
	EA=1;            //���жϴ�
	ET0=1;           //��ʱ���жϴ�
	TR0=1;           //��ʱ�����ش�
}
void UART_Init(void)
{
//**All notes can be deleted and modified**//
    SCON  = 0x05;		        // SCON: ģʽ 1, 8-bit UART, ʹ�ܽ���  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit ��װ
    TH1   = 0xFD;               // TH1:  ��װֵ 9600 ������ ���� 11.0592MHz
	TL1 = TH1;  
    TR1   = 1;                  // TR1:  timer 1 ��                         
    EA    = 1;                  //�����ж�
    ES    = 1;                  //�򿪴����ж�
} 

void Timer0_isr(void) interrupt 1 
{
	TH0=(65536-5000)/256;		  //���¸�ֵ 5ms
	TL0=(65536-5000)%256;
	times_5ms++;				  //��ʱ5ms

}

void UART_SER (void) interrupt 4 	//�����жϷ������
{
	unsigned char R_buf;
	if(RI)                        //�ж��ǽ����жϲ���
	{
		RI=0;                      //��־λ����
		R_buf=SBUF;
		if((R_buf=='O')||(R_buf=='o'))
		{
			Commd_Flag=OPEN;	//���յ��򿪵Ʊ�־
		}
		else if((R_buf=='C')&&(R_buf=='c'))
		{
			Commd_Flag=CLOSE;	//���յ� �رյƱ�־
		}
		else if((R_buf!='1')&&(R_buf!='2')&&(R_buf!='3')&&(R_buf!='4')&&(R_buf!='5')&&(R_buf!='6')&&(R_buf!='7')&&(R_buf!='8')&&(R_buf!='a')&&(R_buf!='A'))
		{
			Commd_Flag=INIT;	 //���� ��ʼ�����ܱ�־
		}

		if(Commd_Flag==OPEN)			//��������ֵ���д���Ӧ�ĵ�
		{
			switch(R_buf)
			{
				case '1':relay1=0;Commd_Flag=INIT;break;	//����Ӧ�ĵ� ���ָ������־
				case '2':relay2=0;Commd_Flag=INIT;break;			
				case '3':relay3=0;Commd_Flag=INIT;break;			
				case '4':relay4=0;Commd_Flag=INIT;break;			
				case '5':relay5=0;Commd_Flag=INIT;break;	
				case '6':relay6=0;Commd_Flag=INIT;break;			
				case '7':relay7=0;Commd_Flag=INIT;break;			
				case '8':relay8=0;Commd_Flag=INIT;break;
				case 'A':relay1=0;relay2=0;relay3=0;relay4=0;relay5=0;relay6=0;relay7=0;relay8=0;Commd_Flag=INIT;break;
				case 'a':relay1=0;relay2=0;relay3=0;relay4=0;relay5=0;relay6=0;relay7=0;relay8=0;Commd_Flag=INIT;break;
				default:break;						 //�˴������ж� ���ɻָ������׼
			}
		}
		else if(Commd_Flag==CLOSE)//��������ֵ���йر���Ӧ�ĵ�
		{
			switch(R_buf)
			{
				case '1':relay1=1;Commd_Flag=INIT;break;	//�ر���Ӧ�ĵ� ���ָ������־
				case '2':relay2=1;Commd_Flag=INIT;break;			
				case '3':relay3=1;Commd_Flag=INIT;break;			
				case '4':relay4=1;Commd_Flag=INIT;break;			
				case '5':relay5=1;Commd_Flag=INIT;break;	
				case '6':relay6=1;Commd_Flag=INIT;break;			
				case '7':relay7=1;Commd_Flag=INIT;break;			
				case '8':relay8=1;Commd_Flag=INIT;break;
				case 'A':relay1=1;relay2=1;relay3=1;relay4=1;relay5=1;relay6=1;relay7=1;relay8=1;Commd_Flag=INIT;break;
				case 'a':relay1=1;relay2=1;relay3=1;relay4=1;relay5=1;relay6=1;relay7=1;relay8=1;Commd_Flag=INIT;break;					
				default:break;						 //�˴������ж� ���ɻָ������׼
			}
		}
		SBUF=R_buf;				   //���ؽ��յ�������
	}
	if(TI)  //����Ƿ��ͱ�־λ������
	TI=0;
} 

