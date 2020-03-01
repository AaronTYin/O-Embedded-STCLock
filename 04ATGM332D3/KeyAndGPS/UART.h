#ifndef __UART_H__
#define __UART_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GPS_Buffer_Length 80
#define latitude_Length 11
#define longitude_Length 12

typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//是否获取到GPS数据
	char isParseData;	//是否解析完成
	char latitude[latitude_Length];		//纬度
	char longitude[longitude_Length];		//经度
	char isUsefull;		//定位信息是否有效
	char isShowData;	//已经显示信息
} xdata _SaveData;


#define gpsRxBufferLength  76
char idata gpsRxBuffer[gpsRxBufferLength];
unsigned char RX_Count = 0;
_SaveData Save_Data;

void Uart_Init()					  			   
{
	SCON = 0X50;  //UART方式1；8位UART
	REN  = 1;     //允许串行口接收数据
	PCON = 0x00;  //SMOD=0;波特率不加倍
	TMOD = 0x20;  //T1方式2，用于产生波特率
	TH1  = 0xFD;  //装初值
	TL1  = 0xFD;
	TR1  = 1;     //启动定时器1
	EA   = 1;     //打开全局中断控制
	ES   = 1;     //打开串行口中断	
}
void Uart_End()					  			   
{
	REN  = 0;     //取消串行口接收数据
	TR1  = 0;     //关闭定时器1
	ES   = 0;     //关闭串行口中断
	EA   = 0;     //关闭全局中断控制
}

void UartPrintf(unsigned char *p)	//发送字符串
{	
 	while(*p)
 	{
	 	SBUF=*(p++);
		while(TI==0) {};
		TI=0;
	}   
}

void UartPrintASCII(unsigned char c)	//发送一个字符
{ 
    TI=0;   
    SBUF=c;   
    while(TI==0) {};   
    TI=0;   
}


void RECEIVE_DATA(void) interrupt 4 using 3	  		
{ 
	unsigned char temp = 0;
	char i = 0;
	ES=0;
	temp = SBUF;
	RI = 0;
	
	if(temp == '$')
	{
		RX_Count = 0;	
	}
		
	if(RX_Count <= 5)
	{
	   gpsRxBuffer[RX_Count++] = temp;
	}
	else if(gpsRxBuffer[0] == '$' &gpsRxBuffer[4] == 'M' && gpsRxBuffer[5] == 'C')	//确定是否收到"GPRMC/GNRMC"这一帧数据
	{
		gpsRxBuffer[RX_Count++] = temp;
		if(temp == '\n')									   
		{
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);	//清空
			memcpy(Save_Data.GPS_Buffer, gpsRxBuffer, RX_Count);	//保存数据
			Save_Data.isGetData = 1;
			RX_Count = 0;
			memset(gpsRxBuffer, 0, gpsRxBufferLength);	//清空				
		}
		
		if(RX_Count >= 75)
		{
			RX_Count = 75;
			gpsRxBuffer[RX_Count] = '\0';//添加结束符
		}			
	}
	ES=1; 
}

void clrStruct()
{
	Save_Data.isGetData = 0;
	Save_Data.isParseData = 0;
	Save_Data.isUsefull = 0;
	Save_Data.isShowData = 0;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
}
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}

void errorLog(int num)
{
	
	while (1)
	{
	  UartPrintf("ERROR");
		UartPrintASCII(num+0x30);
		UartPrintf("\r\n");
	}
}

#endif