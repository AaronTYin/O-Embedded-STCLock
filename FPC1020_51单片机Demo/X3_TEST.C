/************************************************************************************************************************************
 硬件接口：---K1----------P2^0---
           ---K2----------P2^1--- 
           ---K3----------P2^2---
           ---K4----------P2^3---
           ---K5----------P2^4---
           ---BEEP--------P2^5---
					 
硬件环境：单片机：STC90C516RD+
          晶振  ：11.0592M
					波特率：19200
					
****************************************************************************************************************************************

 功能按键：K1 添加指纹  ；K2 删除单个指纹  ；K3 搜索指纹  ：K4 读指纹用户总数   k5 清空指纹库
 
 注释；1、每个操作成功都是亮4个灯加蜂鸣器滴1声，操作失败时8个灯全亮，蜂鸣器滴3声。
       2、搜索指纹成功后，匹配到的用户ID号通过8个LED灯以二进制的形式显示。
       3、读取到的指纹用户总数也是通过8个LED灯以二进制的形式显示。
       4、模块上电后进行添加指纹时， 指纹膜板存放地址从0开始依次增加（已经录入的指纹掉电后也能保存）			 
************************************************************************************************************************************/#include<reg52.h>
#include "config.h"

extern unsigned char l_ucFPID;                  //用户编号

void BBP(unsigned char times) ;
uchar code Keyboard_Control[]={0xfe,0xfd,0xfb,0xf7};	//List of keyboard scanning
uchar RowCol=0;
uchar Key_Check()
{
	uchar Row,Col,i;
	P2=0xf0;
	if((P2&0xf0)!=0xf0)
	{
		Delay_ms(10);
		if((P2&0xf0)!=0xf0)
		{
			Row=P2^0xf0;
			i=0;
			P2=Keyboard_Control[i];
			while(i<4)
			{
				if((P2&0xf0)!=0xf0)
				{
					Col=~(P2&0xff);
					break;
				}
				else
				{
					i++;
					P2=Keyboard_Control[i];
				}
			}
		}
		else
		{
			RowCol=0;
			return 0;
		}
		while((P2&0xf0)!=0xf0);
		BBP(1);
		//P0=Row|Col;
		RowCol=Row|Col;
		return RowCol;
	}
	/*else if(RR_Check)
	{
		RR_Check=0;
		switch(RR_Value[2])
		{
			case 0x12: RowCol=0x84;
			
		}
		return RowCol;
	}*/
	else
	{
		RowCol=0;
		return 0;
	}
}

void main()
{   
	unsigned char MODE ,User_ID;
	UartInit();
	while(1)
	{
		Key_Check();
		switch(RowCol)
		{
			case (0x11): MODE=1;BBP(1);break;
			case (0x21): MODE=2;BBP(2);break;
			case (0x41): MODE=3;BBP(3);break;
		}
		switch ( MODE )
		{                                                                     
			case 0:   SS = 1 ;					                        //无操作 
								break;
			
			case 1:   MODE = 0 ;                                    // 增加指纹用户
								if(Finger_Enroll(++User_ID)) {SS=1;BBP(1);}        				
								else {SS = 0;BBP(3);}
								Delay_ms(2000)  ;
								break;
								
			case 2:   MODE = 0 ;                                   // 删除单个指纹用户
								if( Finger_Search()) {SS = 1;}
								else {SS = 0;BBP(3); break ;}
								if(Finger_Delete(l_ucFPID)) {SS=1;BBP(1);}   
								else {SS = 0;BBP(3);}
								Delay_ms(1000)  ;
								break;
								
			case 3:   MODE = 0 ;                                   //1：N搜索指纹库
								if( Finger_Search()) {SS = 1;BBP(1);}
								else {SS = 0;BBP(3);} 								
								Delay_ms(1000)  ;
								break;
			case 4:   MODE = 0 ;                                   // 读指纹用户总数
								if(Finger_Read()) {SS=1;BBP(1);}   
								else {SS = 0;BBP(3);}
								Delay_ms(1000)  ;
								break;		
			case 5:   MODE = 0 ;                                   // 清除指纹库
								if( Finger_Clear() )  {SS=1;BBP(1);} 
								else {SS = 0;BBP(3);}
								Delay_ms(1000)  ;
								break;														
			default : 
								break ;
			
		}
	}
}
void BBP(unsigned char times)
{
    uchar i;  
			 switch (times)
		{
				case 1: 	for(i=0;i<100;i++){	BEEP=~BEEP;	Delay_ms(1);}	
						 break;
				case 2:   for(i=0;i<100;i++){	BEEP=~BEEP;	Delay_ms(1);}
									Delay_ms(200);
									for(i=0;i<100;i++){	BEEP=~BEEP;	Delay_ms(1);}
						 break;
				case 3:   for(i=0;i<100;i++){	BEEP=~BEEP;	Delay_ms(1);}
									Delay_ms(200);
									for(i=0;i<100;i++){	BEEP=~BEEP;	Delay_ms(1);}
									Delay_ms(200);
									for(i=0;i<100;i++){	BEEP=~BEEP;	Delay_ms(1);}
						 break;
				default:
						 break;
		}
}