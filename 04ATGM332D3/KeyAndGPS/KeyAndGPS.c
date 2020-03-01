#include <stdlib.h>
#include "JudgePassword.h"
#include "GPS.h"

uchar Lat_ROM[]={"3614.3948"};
uchar Long_ROM[]={"12026.3911"};

void main()
{
	
	Uart_Init();
	Delay_ms(10);
	clrStruct();
	//SP=0x5f;
	Init_Lcd();
	CloseLock=1;
	OpenLock=0;
	
	Clear_Ram();
	Show_Line(X2,"Searching GPS...");
	//Show_Line(X2,Lat_ROM);
	
	//while(1)
	{
	while(1)
	{
		parseGpsBuffer();
		printGpsBuffer();
		if(Save_Data.isShowData)
		{
			UartPrintf("\r\nSuccess!");
			Uart_End();
			break;
		}
	}
	}
	
	if(((int)Lat_ROM[5]-(int)Save_Data.latitude[5])>1 || ((int)Long_ROM[6]-(int)Save_Data.longitude[6])>1)
	{
		Show_Line(X2,"Location  Wrong!");
		DelayMs(3000);
	}
	else
		Judge_Password();
}
