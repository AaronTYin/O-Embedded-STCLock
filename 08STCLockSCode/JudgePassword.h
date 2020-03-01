#ifndef _JUDGEPASSWORD_H_
#define _JUDGEPASSWORD_H_

#include "Header.h"

sbit CloseLock = P1^6;
sbit OpenLock = P1^7;
sbit Relay = P3^1;

sbit F_Scan = P3^4;		//Finger
sbit F_Input = P3^5;
sbit F_Success = P3^6;
sbit F_Fail = P3^7;

uchar Lat[]={0};
uchar Long[]={0};

/*-----Judge password from keyboard-----*/
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
			else if(RowCol==0x88&&N==6)	//Got 6 bit number and the button has been pressed
			{
				Right_Wrong=1;
				for(i=0;i<N;i++)
				{
					if(Password_Input[i]!=Password_ROM[i])
						Right_Wrong=0;
				}
				if(Right_Wrong)
				{
					for(i=0;i<6;i++)	//Clear up the value just entered
						Password_Input[i]='-';
					N=0;
					Clear_Ram();
					Show_Line(X2,"    Welcome:    ");
					Show_Line(X3,"    Einstein    ");
					OneAlarm();
					CloseLock=0;
					OpenLock=1;
					Relay=0;
					DelayMs(3000);
					Show_Menu();
					
					while(1)
					{
						if(DHT11_receive())
						{
							TwoAlarm();
							goto Key_Cancel;
						}
						Key_Check();
						if(RowCol==0x11)	//Press 1 to reset the password
						{
							if(!Success_Flag)	//If never reset
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
							else if(Success_Flag)	//If reset
							{
								Show_Menu();
								//break;
							}
						}
						else if(RowCol==0x21)	//Press 2 to change the password
						{
							Success_Flag=0;	//Set reset flag to unreset
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
									Show_Line(X2," Change Success ");	//Change password success
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
						else if(RowCol==0x81)	//Press A to enter the fingerprint page
						{
							Show_Finger();
							while(1)
							{
								if(DHT11_receive())
								{
									TwoAlarm();
									goto Key_Cancel;
								}
								Key_Check();
								if(RowCol==0x11)
								{
									Clear_Ram();
									Show_Line(X2,"    Scanning    ");
									F_Scan=0;
									DelayMs(200);
									F_Scan=1;
									while(F_Success && F_Fail);
									OneAlarm();
									
									if(!F_Success && F_Fail)
									{
										Clear_Ram();
										Show_Line(X2,"    Success!    ");
										DelayMs(2000);
										Clear_Ram();
										Show_Finger();
									}
									else if(F_Success && !F_Fail)
									{
										Clear_Ram();
										Show_Line(X2,"     Fail!!     ");
										DelayMs(2000);
										Clear_Ram();
										Show_Finger();

									}
								}
								else if(RowCol==0x21)
								{
									Clear_Ram();
									Show_Line(X2,"     InputF     ");
									F_Input=0;
									DelayMs(200);
									F_Input=1;
									while(F_Success && F_Fail);
									OneAlarm();

									if(!F_Success && F_Fail)
									{
										Clear_Ram();
										Show_Line(X2,"    Success!    ");
										DelayMs(2000);
										Clear_Ram();
										Show_Finger();
									}
									else if(F_Success && !F_Fail)
									{
										Clear_Ram();
										Show_Line(X2,"     Fail!!     ");
										DelayMs(2000);
										Clear_Ram();
										Show_Finger();
									}
								}
								else if(RowCol==0x84)
								{
									Show_Menu();
									break;
								}
							}
						}
						else if(RowCol==0x82)	//Press B to enter the GPS page
						{
							if(DHT11_receive())
							{
								TwoAlarm();
								goto Key_Cancel;
							}
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
									Clear_Ram();
									Show_GPS();
								}
								else if(RowCol==0x84)
								{
									Show_Menu();
									break;
								}
							}
						}
						else if(RowCol==0x84)	//Press C to jump to the end C
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
			else if(RowCol==0x84)	//Press C to cancel input or reclock
			{
				Key_Cancel: for(i=0;i<6;i++)
					Password_Input[i]='-';
				N=0;
				Clear_Ram();
				Change_Screen(Password_Input);
				CloseLock=1;
				OpenLock=0;
				Relay=1;
			}
		}
	}
}

#endif