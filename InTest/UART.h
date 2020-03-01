#ifndef __UART_H__
#define __UART_H__

#include "Header.h"

#define GPS_Buffer_Length 80
#define latitude_Length 11
#define longitude_Length 12

typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//If got the GPS data
	char isParseData;	//If the data has analysis success
	char latitude[latitude_Length];		//WeiDu
	char longitude[longitude_Length];		//JingDu
	char isUsefull;		//If the position info is useful
	char isShowData;	//Has shown the data 
} xdata _SaveData;


#define gpsRxBufferLength  76
char idata gpsRxBuffer[gpsRxBufferLength];
unsigned char RX_Count = 0;
_SaveData Save_Data;

void Uart_Init()					  			   
{
	SCON = 0X50;  //UART type 1, 8 bit UART
	REN  = 1;     //Allow the serial port get data
	PCON = 0x00;  //SMOD=0; No doubling of baud rate
	TMOD = 0x20;  //T1 type 2, to generate baud rate
	TH1  = 0xFD;  //Filling initial value
	TL1  = 0xFD;
	TR1  = 1;     //Open timer 1
	EA   = 1;     //Open global interrupt control
	ES   = 1;     //Open serial port interrupt
}
void Uart_End()					  			   
{
	REN  = 0;     //Forbid the serial pin get data
	TR1  = 0;     //Close timer 1
	ES   = 0;     //Close serial port interrupt
	EA   = 0;     //Close global interrupt control
}

void UartPrintf(unsigned char *p)	//Send a string
{	
 	while(*p)
 	{
	 	SBUF=*(p++);
		while(TI==0);
		TI=0;
	}   
}

void UartPrintASCII(unsigned char c)	//Send a character
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
	else if(gpsRxBuffer[0] == '$' &gpsRxBuffer[4] == 'M' && gpsRxBuffer[5] == 'C')	//Make sure got the frame:"GPRMC/GNRMC"
	{
		gpsRxBuffer[RX_Count++] = temp;
		if(temp == '\n')									   
		{
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);		//Clear buf
			memcpy(Save_Data.GPS_Buffer, gpsRxBuffer, RX_Count);	//Save data
			Save_Data.isGetData = 1;
			RX_Count = 0;
			memset(gpsRxBuffer, 0, gpsRxBufferLength);	//Clear buf
		}
		
		if(RX_Count >= 75)
		{
			RX_Count = 75;
			gpsRxBuffer[RX_Count] = '\0';		//Adding an End Character
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