#include "Header.h"

uchar Lat_ROM[]={"3614.3948"};
uchar Long_ROM[]={"12026.3911"};

void main()
{
	ATY_Init();	
	
	Clear_Ram();
	Show_Line(X2,"Searching GPS...");
	//Show_Line(X2,Lat_ROM);
	
	while(1)
	{
		parseGpsBuffer();
		printGpsBuffer();
		if(Save_Data.isShowData)
		{
			UartPrintf("\r\nSuccess!");
			//Uart_End();
			break;
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
