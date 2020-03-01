#include <reg52.h>

#define uint unsigned int
#define uchar unsigned char
#define X1 0x80	//X第一行
#define X2 0x90	//X第二行
#define X3 0x88	//X第三行
#define X4 0x98	//X第四行
#define Y 0x80
#define COMMAND 0
#define DATA 1

sbit CS   = P1^0;	//
sbit STD  = P1^1;	//数据口
sbit SCLK = P1^2;
sbit PSB  = P1^3;	//H=并口; L=串口;
sbit RST  = P1^4;	//Reset Signal 低电平有效

void WriteRead_Lcd(uchar DATA_COMMAND,uchar Content);

uchar code Index[]=
{
	"               :"	//第一行
	"  - - -  - - -  "	//第三行
	"    Einstein    "	//第二行
	"----------------"	//第四行
};

/*----------------延时------------------*/
void DelayUs(uint us)   //delay time us
{
  while(us--);
}
void DelayMs(uint ms)	//delay time ms
{
  uint i,j;
  for(i=0;i<ms;i++)
		for(j=0;j<15;j++)
			DelayUs(1);
}

/*----------------初始化------------------*/
void Init_Lcd()
{
  RST=1;
  PSB=0;
  WriteRead_Lcd(COMMAND,0x30);	//30---基本指令动作
  WriteRead_Lcd(COMMAND,0x01);	//清屏，地址指针指向00H
  DelayUs(100);
  WriteRead_Lcd(COMMAND,0x06);	//光标的移动方向
  WriteRead_Lcd(COMMAND,0x0c);	//开显示，关游标
}
/*--------------清DDRAM------------------*/
void Clear_Ram()
{
  WriteRead_Lcd(COMMAND,0x30);
  WriteRead_Lcd(COMMAND,0x01);
  DelayUs(180);
}
/*-----------显示汉字或字符------------*/
void Show_Screen(uchar *Content)
{
  uchar i,j;
	Clear_Ram();
  WriteRead_Lcd(COMMAND,X1);	//指定DDRAM首地址
  for(j=0;j<4;j++)
    for(i=0;i<16;i++)
			WriteRead_Lcd(DATA,Content[j*16+i]);
}
/*-----------显示指定行汉字或字符------------*/
void Show_Line(uchar Line,uchar *Content)
{
	uchar i;
  //Clear_Ram();
  WriteRead_Lcd(COMMAND,Line);
  for(i=0;i<16;i++)
		WriteRead_Lcd(DATA,Content[i]);
}
/*------------DDRAM读写-----------------*/
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
    STD=0;   //write - COMMANDand
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

void Change_Screen(uchar *Num)
{
	uchar Changed_Index[]=
	{
		"C              :"
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


/*------------------主程序--------------------*
void main()
{
	uchar Num[]={"123456"};
	//uchar Line_Word[]={" 这 是 第 N 行 "};

	SP=0x5f;	//堆栈指针
  Init_Lcd();
	
	while(1)
	{
		DelayMs(2000);
		Show_Screen(Index);
		DelayMs(2000);
		Change_Screen(Num);
		DelayMs(2000);
		Show_Line(X1,Num);
		DelayMs(2000);
		Show_Line(X2,Num);
		DelayMs(2000);
		Show_Line(X3,Num);
		DelayMs(2000);
		Show_Line(X4,Num);
		//DelayMs(2000);
		//Show_Line(X1,Line_Word);
		//DelayMs(2000);
		//Show_Line(X2,Line_Word);
		//DelayMs(2000);
		//Show_Line(X3,Line_Word);
		//DelayMs(2000);
		//Show_Line(X4,Line_Word);
	}
}
*/
