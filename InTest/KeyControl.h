#ifndef _KEYCONTROL_H_
#define _KEYCONTROL_H_

#include "Header.h"

uchar code Keyboard_Control[]={0xfe,0xfd,0xfb,0xf7};	//List of keyboard scanning
uchar RowCol=0;

/*-----Scan the key and return the value-----*/
uchar Key_Check()
{
	uchar Row,Col,i;
	P2=0xf0;
	RR_Init();
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
	else if(RR_Check)
	{
		RR_Check=0;
		switch(RR_Value[2])
		{
			case 0x01: RowCol=0x11;break;
			case 0x02: RowCol=0x21;break;
			case 0x03: RowCol=0x41;break;
			case 0x04: RowCol=0x12;break;
			case 0x05: RowCol=0x22;break;
			case 0x06: RowCol=0x42;break;
			case 0x07: RowCol=0x14;break;
			case 0x08: RowCol=0x24;break;
			case 0x09: RowCol=0x44;break;
			case 0x00: RowCol=0x28;break;
			case 0x40: RowCol=0x84;break;	//Cancel
			case 0x0A: RowCol=0x88;break;	//OK
			case 0x19: RowCol=0x81;break;	//A	up
			case 0x1D: RowCol=0x82;break;	//B	down
			case 0x46: RowCol=0x18;break;	//*	left
			case 0x47: RowCol=0x48;break;	//#	right
			case 0x10: 
				if(Alarm_Switch)
					Alarm_Switch=0;
				else
					Alarm_Switch=200;
				break;
		}
		OneAlarm();
		//RR_DATA=0;
		return RowCol;
	}
	else
	{
		RowCol=0;
		return 0;
	}
}
/*-----Coding the key value to number-----*/
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

#endif