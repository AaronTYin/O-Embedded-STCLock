#include <reg52.h>

#define uint  unsigned int
#define uchar unsigned char
#define x     0x80
#define y     0x80
#define comm  0
#define dat   1
//|-----------------|			 |-----------------|
//|              VDD|--------------------|VCC		   |
//|             LED+|--------------------|VCC		   |
//|             LED-|--------------------|GND		   |
//|              VSS|--------------------|GND		   |
//|                 |			 |	89C52	   |
//|              DB0|--------------------|P1.0		   |
//|              DB1|--------------------|P1.1		   |
//|              DB2|--------------------|P1.2		   |
//|   OCMJ2X4C   DB3|--------------------|P1.3		   |
//|              DB4|--------------------|P1.4		   |
//|              DB5|--------------------|P1.5		   |
//|              DB6|--------------------|P1.6		   |
//|        DB7(busy)|--------------------|P1.7		   |
//|               rs|--------------------|P3.0		   |
//|               rw|--------------------|P3.1		   |
//|                e|--------------------|P3.2		   |
//|              psb|--------------------|P3.3		   |
//|              rst|--------------------|P3.4		   |
//|-----------------|			 |-----------------|

sbit rs = P3^0;   //H=data; L=command;
sbit rw = P3^1;   //H=read; L=write;
sbit  e = P3^2;   //input enable;
sbit psb= P3^3;   //H=并口; L=串口;
sbit rst= P3^4;   //Reset Signal 低电平有效

sbit busy=P1^7;   //lcd busy bit

void wr_lcd (uchar dat_comm,uchar content);
void chk_busy (void);
void delay (uint us);

uchar code tab1[]={
"  阀门  "
"  80％  "
};

/*------------------初始化-----------------*/
void init_lcd (void)
{
  rst=1;
  psb=1;
  wr_lcd (comm,0x30);  /*30---基本指令动作*/   
  wr_lcd (comm,0x01);  /*清屏，地址指针指向00H*/
  wr_lcd (comm,0x06);  /*光标的移动方向*/
  wr_lcd (comm,0x0c);  /*开显示，关游标*/
}
/*---------------显示汉字或字符----------------*/
void chn_disp (uchar code *chn)
{
  uchar i,j;
  wr_lcd (comm,0x30);
  wr_lcd (comm,0x80);
  j=0;
  for (i=0;i<8;i++)
  wr_lcd (dat,chn[j*8+i]);
  wr_lcd (comm,0x90);
  j=1;
  for (i=0;i<8;i++)
  wr_lcd (dat,chn[j*8+i]);
}
/*--------------清DDRAM------------------*/
void clrram (void)
{
  wr_lcd (comm,0x30);
  wr_lcd (comm,0x01);
}
/*---------------------------------------*/
void wr_lcd (uchar dat_comm,uchar content)
{
  chk_busy ();
  if(dat_comm)
   {
    rs=1;   //data
    rw=0;   //write
   }
  else 
   {
    rs=0;   //command
    rw=0;   //write
   }
  P1=content;   //output data or comm
  e=1;
  ;
  e=0;
}

void chk_busy (void)
{
  P1=0xff;
  rs=0;
  rw=1;
  e =1;
  while(busy==1);
  e =0;
}
/*---------------------------------------*/
void delay (uint us)   //delay time
{
  while(us--);
}
/*------------------主程序--------------------*/
void main ()
{
	  SP=0x5f;
	  init_lcd ();
	  clrram();
	  chn_disp (tab1);
	  while (1);
}
