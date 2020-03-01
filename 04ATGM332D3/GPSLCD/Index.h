#ifndef _INDEX_H_
#define _INDEX_H_

#include "Delay.h"

#define X1 0x80	//X第一行
#define X2 0x90	//X第二行
#define X3 0x88	//X第三行
#define X4 0x98	//X第四行
#define Y 0x80
#define COMMAND 0
#define DATA 1

sbit CS   = P1^1;	//
sbit STD  = P1^2;	//数据口
sbit SCLK = P1^3;
sbit PSB  = P0^0;	//H=并口; L=串口;
sbit RST  = P0^1;	//Reset Signal 低电平有效

void WriteRead_Lcd (uchar DATA_COMMAND,uchar Content);
void DelayUs (uint us);

uchar code Index[]=
{
	"I              :"	//第一行
	"  - - -  - - -  "	//第三行
	"    Einstein    "	//第二行
	"----------------"	//第四行
};


/*-----初始化-----*/
void Init_Lcd (void)
{
  RST=1;
  PSB=0;
  WriteRead_Lcd (COMMAND,0x30);	//30---基本指令动作
  WriteRead_Lcd (COMMAND,0x01);	//清屏，地址指针指向00H
  DelayUs (100);
  WriteRead_Lcd (COMMAND,0x06);	//光标的移动方向
  WriteRead_Lcd (COMMAND,0x0c);	//开显示，关游标
}
/*-----清DDRAM-----*/
void Clear_Ram (void)
{
  WriteRead_Lcd (COMMAND,0x30);
  WriteRead_Lcd (COMMAND,0x01);
  DelayUs (180);
}
/*-----显示整屏汉字或字符-----*/
void Show_Screen (uchar *Content)
{
  uchar i,j;
	Clear_Ram();
  WriteRead_Lcd (COMMAND,X1);	//指定DDRAM首地址
  for (j=0;j<4;j++)
    for (i=0;i<16;i++)
			WriteRead_Lcd(DATA,Content[j*16+i]);
}
/*-----显示指定行汉字或字符-----*/
void Show_Line (uchar Line,uchar *Content)
{
	uchar i;
  //Clear_Ram();
  WriteRead_Lcd(COMMAND,Line);
  for(i=0;i<16;i++)
		WriteRead_Lcd(DATA,Content[i]);
}
/*-----DDRAM读写-----*/
void WriteRead_Lcd (uchar DATA_COMMAND,uchar Content)
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
      STD=CY;	//CY->Carry位
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

void Change_Screen (uchar *Num)
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
void Change_Password (uchar *Num)
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
void Show_Menu ()
{
	Clear_Ram();
	Show_Line(X1,"Menu            ");
	Show_Line(X2,"1:Reset A:Finger");
	Show_Line(X3,"2:Alter B:GPS   ");
	Show_Line(X4,"    C:ReLock    ");
}
void Show_Finger ()
{
	Clear_Ram();
	Show_Line(X1,"Finger          ");
	Show_Line(X2,"    1:Scan      ");
	Show_Line(X3,"    2:Input     ");
	Show_Line(X4,"    C:Cancel    ");
}
void Show_GPS ()
{
	Clear_Ram();
	Show_Line(X1,"GPS             ");
	Show_Line(X2,"  1:CurrentGPS  ");
	Show_Line(X3,"  2:SetCurrent  ");
	Show_Line(X4,"  C:Cancel      ");
}

#endif