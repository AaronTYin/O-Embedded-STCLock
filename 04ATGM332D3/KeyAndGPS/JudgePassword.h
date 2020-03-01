#ifndef _JUDGEPASSWORD_H_
#define _JUDGEPASSWORD_H_

#include "KeyControl.h"
#include "AT24C02.h"
#include "Index.h"
#include <string.h>

sbit CloseLock = P1^6;
sbit OpenLock = P1^7;

uchar Lat[]={0};
uchar Long[]={0};

/*-----判断键盘密码-----*/
void Judge_Password()
{
	uint N=0;
	uint i;
	uchar Init_ROM_Flag[]={"0"};
	uchar Password_ROM[]={"26262626"};
	uchar Password_Input[]={"------"};
	uint Right_Wrong=0;
	uint Success_Flag=0;
	uint N_Reset=0;

	Read_ROM(Init_ROM_Flag,0,1);
	if(Init_ROM_Flag[0]!='1')
	{
		Init_ROM_Flag[0]='1';
		Write_ROM(Init_ROM_Flag,0,1);
		Write_ROM(Password_ROM,1,6);
		Clear_Ram();
		Show_Line(X2,"    00000000    ");
		DelayMs(1000);
	}
	else
	{
		Read_ROM(Password_ROM,1,6);
		Clear_Ram();
		Show_Line(X2,"    11111111    ");
		DelayMs(1000);
	}
	Show_Screen(Index);
	
	while(1)
	{
		while(Key_Check())
		{
			if(N<6&&(RowCol==0x11||RowCol==0x21||RowCol==0x41||RowCol==0x12||RowCol==0x22||RowCol==0x42||RowCol==0x14||RowCol==0x24||RowCol==0x44||RowCol==0x18||RowCol==0x28||RowCol==0x48))
			{
				Password_Input[N]=Coding_Key(RowCol);
				P0=RowCol;
				N++;
				Change_Screen(Password_Input);
			}
			else if(RowCol==0x88&&N==6)	//按下D确认且密码输入6位
			{
				Right_Wrong=1;
				for(i=0;i<N;i++)
				{
					if(Password_Input[i]!=Password_ROM[i])
						Right_Wrong=0;
				}
				if(Right_Wrong)
				{
					for(i=0;i<6;i++)	//清除刚输入的内容
						Password_Input[i]='-';
					N=0;
					Clear_Ram();
					Show_Line(X2,"    Welcome:    ");
					Show_Line(X3,"    Einstein    ");
					OneAlarm();
					CloseLock=0;
					OpenLock=1;
					DelayMs(3000);
					Show_Menu();
					
					while(1)
					{
						Key_Check();
						if(RowCol==0x11)	//按下1键复位密码
						{
							if(!Success_Flag)	//如果未曾复位
							{
								for(i=0;i<6;i=i+2)
									Password_ROM[i]='2';
								for(i=1;i<6;i=i+2)
									Password_ROM[i]='6';
								Write_ROM(Password_ROM,1,6);
								Clear_Ram();
								Show_Line(X2," Reset  Success ");
								Show_Line(X3,"  2 6 2  6 2 6  ");
								Success_Flag=1;
								OneAlarm();
								DelayMs(5000);
								Show_Menu();
								
							}
							else if(Success_Flag)	//如果已经复位
							{
								Show_Menu();
								//break;
							}
						}
						else if(RowCol==0x21)	//按下2键重设密码
						{
							Success_Flag=0;	//修改密码将复位信息置为未复位
							Change_Password(Password_Input);
							while(1)
							{
								Key_Check();
								if(RowCol==0x11||RowCol==0x21||RowCol==0x41||RowCol==0x12||RowCol==0x22||RowCol==0x42||RowCol==0x14||RowCol==0x24||RowCol==0x44||RowCol==0x18||RowCol==0x28||RowCol==0x48)
								{
									Password_ROM[N_Reset]=Coding_Key(RowCol);
									Password_Input[N_Reset]=Coding_Key(RowCol);
									N_Reset++;
									Change_Password(Password_Input);
								}
								else if(RowCol==0x88&&N_Reset==6)
								{
									Write_ROM(Password_ROM,1,6);
									N_Reset=0;
									Clear_Ram();
									Show_Line(X2," Change Success ");	//修改密码成功
									OneAlarm();
									DelayMs(2000);
									Show_Menu();
									break;
								}
								else if(RowCol==0x84)
								{
									N_Reset=0;
									for(i=0;i<6;i++)
										Password_Input[i]='-';
									Show_Menu();
									break;
								}
							}
						}
						else if(RowCol==0x81)	//按下A键进入指纹界面
						{
							Show_Finger();
							while(1)
							{
								Key_Check();
								if(RowCol==0x11)
								{
									Clear_Ram();
									Show_Line(X2,"     0000A1     ");
									DelayMs(2000);
								}
								else if(RowCol==0x21)
								{
									Clear_Ram();
									Show_Line(X2,"     0000A2     ");
									DelayMs(2000);
								}
								else if(RowCol==0x84)
								{
									Show_Menu();
									break;
								}
							}
						}
						else if(RowCol==0x82)	//按下B键进入GPS界面
						{
							Show_GPS();
							while(1)
							{
								Key_Check();
								if(RowCol==0x11)
								{
									Clear_Ram();
									Show_LatLong();
								}
								if(RowCol==0x21)
								{
									Clear_Ram();
									Show_Line(X2,"     0000B2     ");
									DelayMs(2000);
								}
								else if(RowCol==0x84)
								{
									Show_Menu();
									break;
								}
							}
						}
						else if(RowCol==0x84)	//按下C键跳到结尾C
							goto Key_Cancel;
					}
				}
				else
				{
					Clear_Ram();
					Show_Line(X2,"Password  Wrong!");
					TwoAlarm();
					DelayMs(3000);
					for(i=0;i<6;i++)
						Password_Input[i]='-';
					N=0;
					Clear_Ram();
					Change_Screen(Password_Input);

				}
			}
			else if(RowCol==0x84)	//按下C键，取消输入或重新上锁
			{
				Key_Cancel: for(i=0;i<6;i++)
					Password_Input[i]='-';
				N=0;
				Clear_Ram();
				Change_Screen(Password_Input);
				CloseLock=1;
				OpenLock=0;
			}
		}
	}
}

#endif