#ifndef _INFRARED_H_
#define _INFRARED_H_

#include "Header.h"

sbit RR_DATA = P3^2;
uchar RR_Value[4]=0;
uchar RR_Time=0;
uchar RR_Check=0;

void UART_Send_Byte(uchar c)
{
	SBUF=c;
	while(TI==0);
	TI=0;
}
void UART_Send_String(uchar *str)
{
	while(*str!='\0')
	{
		UART_Send_Byte(*str++);
	}
}

void RR_Init(void)
{
	IT0=1;
	EX0=1;
	EA=1;
	RR_DATA=1;
}
void RR_Read() interrupt 0
{
	uchar j,k;
	uint err;
	RR_Time=0;                  
	Delay_10Us(700);
	if(RR_DATA==0)
	{
		err=1000;
		while((RR_DATA==0)&&(err>0))
		{           
			Delay_10Us(1);
			err--;
		} 
		if(RR_DATA==1) 
		{
			err=500;
			while((RR_DATA==1)&&(err>0))   
			{
				Delay_10Us(1);
				err--;
			}
			for(k=0;k<4;k++)      
			{               
				for(j=0;j<8;j++)   
				{
					err=60; 
					while((RR_DATA==0)&&(err>0))
					{
						Delay_10Us(1);
						err--;
					}
					err=500;
					while((RR_DATA==1)&&(err>0))  
					{
						Delay_10Us(10); 
						RR_Time++;
						err--;
						if(RR_Time>30)
						{
							return;
						}
					}
					RR_Value[k]>>=1; 
					if(RR_Time>=8) 
					{
						RR_Value[k]|=0x80;
					}
					RR_Time=0;
				}
			}
		}
		if(RR_Value[2]==~RR_Value[3])
		{
			RR_Check=1;
			UART_Send_String(RR_Value);	//Show uart data at hex, don't use text type
			UART_Send_String("\r\n\r\n");
		}
		else
		{
			RR_Check=0;
			UART_Send_String("Error\r\n");
		}
	}
}


#endif