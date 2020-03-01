/*GPS未调试好，无法获取经纬度信息。*/

#include "Delay.h"
#include "Index.h"
#include "UART.h"

uchar WholeFlag[1]="0";

void Reset (void)
{
	((void (code *) (void)) 0x0000) ();
}

void main ()
{
	while(1)
	{		
		if(WholeFlag[0]=='1')
		{
			WholeFlag[0]='2';
			Uart_Init();
			Delay_ms(10);
			Clear_GPS();
			
			while(1)
			{
				Print_GPSData();
				//UartPrintf(WholeFlag);
				//UartPrintf("\r\n");
			}
		}//*/
		/*else if(WholeFlag[0]=='2')
		{
			SP=0x5f;
			Init_Lcd();
		
			Clear_Ram();
			Show_Line(X1,WholeFlag);
			DelayMs(2000);
			WholeFlag[0]='0';
			Clear_Ram();
			Show_Line(X2,WholeFlag);
			DelayMs(2000);
		}//*/
		else
		{
			WholeFlag[0]='1';
		}
		//Reset();
	}
}
