#ifndef _INDEX_H_
#define _INDEX_H_

#include "Header.h"

#define X1 0x80	//X first line
#define X2 0x90	//X second line
#define X3 0x88	//X third line
#define X4 0x98	//X fourth line
#define Y 0x80
#define COMMAND 0
#define DATA 1

sbit CS   = P1^1;
sbit STD  = P1^2;	//Data port
sbit SCLK = P1^3;
sbit PSB  = P0^0;	//H=Parallel; L=Serial;
sbit RST  = P0^1;	//Reset Signal Low effective

void WriteRead_Lcd (uchar DATA_COMMAND,uchar Content);

uchar code Index[]=
{
	"I              :"	//The first line
	"  - - -  - - -  "	//The third line
	"    Einstein    "	//The second line
	"----------------"	//The fource line
};


/*-----Initialization-----*/
void Init_Lcd()
{
  RST=1;
  PSB=0;
  WriteRead_Lcd(COMMAND,0x30);	//Basic command action
  WriteRead_Lcd(COMMAND,0x01);	//Clear screen, address pointer points to 00H
  DelayUs(100);
  WriteRead_Lcd(COMMAND,0x06);	//Moving direction of cursor
  WriteRead_Lcd(COMMAND,0x0c);	//Open display, close cursor
}
/*-----Clear DDRAM-----*/
void Clear_Ram ()
{
  WriteRead_Lcd(COMMAND,0x30);
  WriteRead_Lcd(COMMAND,0x01);
  DelayUs(180);
}
/*-----Show characters on the whole screen-----*/
void Show_Screen(uchar *Content)
{
  uchar i,j;
	Clear_Ram();
  WriteRead_Lcd(COMMAND,X1);	//Specify the first address of DDRAM
  for(j=0;j<4;j++)
    for(i=0;i<16;i++)
			WriteRead_Lcd(DATA,Content[j*16+i]);
}
/*-----Show characters on designated line-----*/
void Show_Line(uchar Line,uchar *Content)
{
	uchar i;
  //Clear_Ram();
  WriteRead_Lcd(COMMAND,Line);
  for(i=0;i<16;i++)
		WriteRead_Lcd(DATA,Content[i]);
}
/*-----Write or read DDRAM-----*/
void WriteRead_Lcd(uchar DATA_COMMAND,uchar Content)
{
  uchar a,i,j;
  DelayUs(50);
  a=Content;
  CS=1;
  SCLK=0;
  STD=1;
  for(i=0;i<5;i++)
  {
    SCLK=1;
    SCLK=0;
  }
  STD=0;
  SCLK=1;
  SCLK=0;
  if(DATA_COMMAND)
    STD=1;   //read from DDRAM - data
  else
    STD=0;   //write - COMMAND
  SCLK=1;
  SCLK=0;
  STD=0;
  SCLK=1;
  SCLK=0;
  for(j=0;j<2;j++)
  {
    for(i=0;i<4;i++)
    {
      a=a<<1;
      STD=CY;	//CY->Carry
      SCLK=1;
      SCLK=0;
    }
    STD=0;
    for(i=0;i<4;i++)
    {
      SCLK=1;
      SCLK=0;
    }
  }
}

void Change_Screen(uchar *Num)
{
	uchar Changed_Index[]=
	{
		"I              :"
		"  - - -  - - -  "
		"    Einstein    "
		"----------------"
	};
	Clear_Ram();
	Changed_Index[18]=Num[0];
	Changed_Index[20]=Num[1];
	Changed_Index[22]=Num[2];
	Changed_Index[25]=Num[3];
	Changed_Index[27]=Num[4];
	Changed_Index[29]=Num[5];
	Show_Screen(Changed_Index);
}
void Change_Password(uchar *Num)
{
	uchar Changed_Password[]=
	{
		"C              :"
		"  - - -  - - -  "
		"    Einstein    "
		"----------------"
	};
	Clear_Ram();
	Changed_Password[18]=Num[0];
	Changed_Password[20]=Num[1];
	Changed_Password[22]=Num[2];
	Changed_Password[25]=Num[3];
	Changed_Password[27]=Num[4];
	Changed_Password[29]=Num[5];
	Show_Screen(Changed_Password);
}
void Show_Menu()
{
	Clear_Ram();
	Show_Line(X1,"Menu            ");
	Show_Line(X2,"1:Reset A:Finger");
	Show_Line(X3,"2:Alter B:GPS   ");
	Show_Line(X4,"    C:ReLock    ");
}
void Show_Finger()
{
	Clear_Ram();
	Show_Line(X1,"Finger          ");
	Show_Line(X2,"    1:Scan      ");
	Show_Line(X3,"    2:Input     ");
	Show_Line(X4,"    C:Cancel    ");
}
void Show_GPS()
{
	Clear_Ram();
	Show_Line(X1,"GPS             ");
	Show_Line(X2,"  1:CurrentGPS  ");
	Show_Line(X3,"  2:SetCurrent  ");
	Show_Line(X4,"  C:Cancel      ");
}
void Show_LatLong()
{
	uchar LatLong[]=
	{
		"Latitude:       "
		"Longitude:      "
		"                "
		"                "
	};
	Clear_Ram();
	LatLong[39]=Save_Data.latitude[0];
	LatLong[40]=Save_Data.latitude[1];
	LatLong[41]=Save_Data.latitude[2];
	LatLong[42]=Save_Data.latitude[3];
	LatLong[43]=Save_Data.latitude[4];
	LatLong[44]=Save_Data.latitude[5];
	LatLong[45]=Save_Data.latitude[6];
	LatLong[46]=Save_Data.latitude[7];
	LatLong[47]=Save_Data.latitude[8];
	LatLong[54]=Save_Data.longitude[0];
	LatLong[55]=Save_Data.longitude[1];
	LatLong[56]=Save_Data.longitude[2];
	LatLong[57]=Save_Data.longitude[3];
	LatLong[58]=Save_Data.longitude[4];
	LatLong[59]=Save_Data.longitude[5];
	LatLong[60]=Save_Data.longitude[6];
	LatLong[61]=Save_Data.longitude[7];
	LatLong[62]=Save_Data.longitude[8];
	LatLong[63]=Save_Data.longitude[9];
	Show_Screen(LatLong);
}

#endif