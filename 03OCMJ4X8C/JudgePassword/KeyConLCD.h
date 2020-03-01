#ifndef _KeyConLED_H_
#define _KeyConLED_H_

#include <reg52.h>
#include <index.h>
#include <Alarm.h>

uchar code Keyboard_Control[]={0xfe,0xfd,0xfb,0xf7};	//键盘扫描控制表
uchar RowCol=0;
uchar Num[]={"------"};

/*-----检测按键并返回键值-----*/
uchar Key_Check()
{
	uchar Row,Col,i;
	P2=0xf0;
	if((P2&0xf0)!=0xf0)
	{
		DelayMs(10);
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
		OneAlarm();
		//P0=Row|Col;
		RowCol=Row|Col;
		return RowCol;
	}
	else
	{
		RowCol=0;
		return 0;
	}
}
/*-----将按键编码为数值-----*/
uchar Coding_Key(uchar RowCol)	 
{
	uchar Coded_Key;
	switch(RowCol)
	{
		case (0x11): Coded_Key='1';break;
		case (0x21): Coded_Key='2';break;
		case (0x41): Coded_Key='3';break;
		case (0x81): Coded_Key='A';break;
		case (0x12): Coded_Key='4';break;
		case (0x22): Coded_Key='5';break;
		case (0x42): Coded_Key='6';break;
		case (0x82): Coded_Key='B';break;
		case (0x14): Coded_Key='7';break;
		case (0x24): Coded_Key='8';break;
		case (0x44): Coded_Key='9';break;
		case (0x84): Coded_Key='C';break;
		case (0x18): Coded_Key='*';break;
		case (0x28): Coded_Key='0';break;
		case (0x48): Coded_Key='#';break;
		case (0x88): Coded_Key='D';break;
	}
	return Coded_Key;
}


/*
void main()
{
	uint N=0;
	SP=0x5f;
  Init_Lcd();
	
	Show_Screen(Index);
	
	while(1)
	{
		while(Key_Check())
		{
			if(RowCol==0x11||RowCol==0x21||RowCol==0x41||RowCol==0x12||RowCol==0x22||RowCol==0x42||RowCol==0x14||RowCol==0x24||RowCol==0x44||RowCol==0x18||RowCol==0x28||RowCol==0x48)
			{
				Num[N]=Coding_Key(RowCol);
				P0=RowCol;
				N++;
				Change_Screen(Num);
			}
			else if(RowCol==0x88&&N==6)
			{
				Clear_Ram();
				Show_Line(X2,"    Welcome:    ");
				Show_Line(X3,"    Einstein    ");
			}
		}
		
	}
}
*/

#endif