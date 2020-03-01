#ifndef __UART_H__
#define __UART_H__

#include <string.h>
#define uchar unsigned char

uchar GPS_Buffer[]={0};
uchar GPS_Data[]={0};
uchar isGetData=0;		//是否获取到GPS数据
uchar beginGetData=0;	//开始获取数据
uchar isRightData=0;	//是否正确数据
uchar isParseData=0;	//是否解析完成
uchar isUsefull=0;		//定位信息是否有效
uchar RX_Count=0;
uchar GPS_Length=0; 

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

void UartPrintf(uchar *p)	//发送字符串
{	
 	while(*p)
 	{
	 	SBUF=*(p++);
		while(TI==0) {};
		TI=0;
	}   
}
void UartPrintASCII(uchar c)	//发送一个字符
{ 
    TI=0;   
    SBUF=c;   
    while(TI==0) {};   
    TI=0;   
}
void Clear_GPS()
{
	isGetData = 0;
	beginGetData = 0;
	isRightData = 0;
	isParseData = 0;
	isUsefull = 0;
	memset(GPS_Buffer, 0, 29);
}
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}

void Receive_Data(void) interrupt 4 using 3
{ 
	uchar GPS_Temp = 0;
	ES=0;
	GPS_Temp = SBUF;
	RI = 0;
	
	//UartPrintf("Receive...\r\n");
	if(GPS_Temp == '$' && !beginGetData)
	{
		beginGetData = 1;
		memset(GPS_Buffer, 0, 29);
		UartPrintf("beginGetData..\r\n");
	}
	if(GPS_Temp == 'M' && beginGetData)
	{
		isRightData=1;
		UartPrintf("isRightData..\r\n");
	}
	if(GPS_Temp == 'A' && isRightData)
	{
		UartPrintf("GetA..\r\n");
		RX_Count = 0;
		GPS_Buffer[RX_Count++] = GPS_Temp;
	}
	if(GPS_Buffer[0] == 'A')
	{
		//UartPrintf("Good..\r\n");
		GPS_Buffer[RX_Count] = GPS_Temp;
		if(GPS_Buffer[RX_Count] == 'E')									   
		{
			UartPrintf("----------GetE..\r\n");
			memset(GPS_Data, 0, RX_Count);
			memcpy(GPS_Data, GPS_Buffer, RX_Count);
			isGetData = 1;
			GPS_Buffer[RX_Count+1] = '\0';//添加结束符
			GPS_Length = RX_Count+1;
			memset(GPS_Buffer, 0, GPS_Length);
			RX_Count = 0;
			beginGetData = 0;
			isRightData = 0;
		}
		else
			//UartPrintf("NoE..\r\n");
		RX_Count++;
	}
	ES=1;
}

void Print_GPSData()
{
	if (isGetData)
	{
		isGetData = 0;
		UartPrintf("**************\r\n");
		UartPrintf(GPS_Data);
	}
	else
	{
		//UartPrintf("NoUse\r\n");
	}
}

#endif