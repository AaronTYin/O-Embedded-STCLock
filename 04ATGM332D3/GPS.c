#include <reg52.h>
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
} xdata _SaveData;


//函数或者变量声明
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
void UartPrintf(unsigned char *p)				//发送字符串
{	
 	while(*p)
 	{
	 	SBUF=*(p++);
		while(TI==0)
		{

		};
		TI=0;
	}   
}
void UartPrintASCII(unsigned char c)				//发送一个字符
{ 
    TI=0;   
    SBUF=c;   
    while(TI==0);   
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
	else if(gpsRxBuffer[0] == '$' &gpsRxBuffer[4] == 'M' && gpsRxBuffer[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
	{
		gpsRxBuffer[RX_Count++] = temp;
		if(temp == '\n')									   
		{
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
			memcpy(Save_Data.GPS_Buffer, gpsRxBuffer, RX_Count); 	//保存数据
			Save_Data.isGetData = 1;
			RX_Count = 0;
			memset(gpsRxBuffer, 0, gpsRxBufferLength);      //清空				
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
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
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

void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = 0;
		UartPrintf("**************\r\n");
		UartPrintf(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						//case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						//case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						//case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W
						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = 1;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = 1;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = 0;
				}
				else
				{
					errorLog(2);	//解析错误
				}
			}


		}
	}
}

void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = 0;
		
		UartPrintf("Save_Data.UTCTime = ");
		UartPrintf(Save_Data.UTCTime);
		UartPrintf("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = 0;
			UartPrintf("Save_Data.latitude = ");
			UartPrintf(Save_Data.latitude);
			UartPrintf("\r\n");


			UartPrintf("Save_Data.N_S = ");
			UartPrintf(Save_Data.N_S);
			UartPrintf("\r\n");

			UartPrintf("Save_Data.longitude = ");
			UartPrintf(Save_Data.longitude);
			UartPrintf("\r\n");

			UartPrintf("Save_Data.E_W = ");
			UartPrintf(Save_Data.E_W);
			UartPrintf("\r\n");
		}
		else
		{
			UartPrintf("GPS DATA is not usefull!\r\n");
		}
		
	}
}

void main()
{
	unsigned char i = 0;
	Uart_Init();
	Delay_ms(10);

	clrStruct();	//清空缓存数组

	while(1)
	{
		parseGpsBuffer();
		printGpsBuffer();
	}
}


