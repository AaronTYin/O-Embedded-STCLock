#ifndef _AT24C02_H_
#define _AT24C02_H_

#include <reg52.h>
#include <index.h>
#include <intrins.h>	//用于_nop_();

sbit SCL = P1^6;			//24C02串行时钟
sbit SDA = P1^7;			//24C02串行数据

/*-----空操作-----*/
void Nop()
{
 	_nop_();	//一条大约1us
 	_nop_();
 	_nop_();
 	_nop_();
}
/*-----起始条件-----*/
void Start()
{
 	SDA=1;
 	SCL=1;
 	Nop();
 	SDA=0;
 	Nop();
}
/*-----停止条件-----*/
void Stop()
{
 	SDA=0;
 	SCL=1;
 	Nop();
 	SDA=1;
 	Nop();
}
/*-----应答位-----*/
void Ack()
{
	SDA=0;
	Nop();
	SCL=1;
	Nop();
	SCL=0;
}
/*-----反向应答位-----*/
void DeAck()
{
 	SDA=1;
 	Nop();
 	SCL=1;
 	Nop();
 	SCL=0;
}

/*-----发送数据子程序，Data为要发送的数据-----*/
void Send(uchar Data_Send)
{
   	uchar BitCounter=8;
   	uchar Temp;
   	do
   	{
   		Temp=Data_Send;	//将待发送的数据暂存Temp
   		SCL=0;
   		Nop();
   		if((Temp&0x80)==0x80)	//将读到的数据&0x80
				SDA=1;
   		else 
				SDA=0;
   		SCL=1;
   		Temp=Data_Send<<1;	//数据左移
   		Data_Send=Temp;	//数据左移重新赋值Data_Send
   		BitCounter--;	//当变量减到0，数据传送完成
   	}
   	while(BitCounter);	//判断是否传送完成
   	SCL=0;
}
/*-----读一字节的数据，并返该字节值-----*/
uchar Read()
{
  uchar Data_Read=0;
	uchar Temp=0;
	uchar BitCounter=8;
	SDA=1;
	do
	{
		SCL=0;
		Nop();
		SCL=1;
		Nop();
		if(SDA)	//数据位为1
			Data_Read=Data_Read|0x01;	//Data_Read的最低位为1（|0x01，就是将最低位变为1）
		else	//数据位为0
			Data_Read=Data_Read&0xfe;	//Data_Read最低位为0（&0xfe 11111110 就是将最低位变为0）
		if(BitCounter-1)	//BitCounter减1后是否为真
		{
			Temp=Data_Read<<1;	//Data_Read左移
			Data_Read=Temp;
		}
		BitCounter--;	//BitCounter减到0时，数据接收完毕
	}
	while(BitCounter);	//判断是否接受完成
	return Data_Read;
}

/*-----向ROM写数据-----*/
void Write_ROM(uchar Data_Write[],uchar Address,uchar Num)
{
  uchar i;
  uchar *PData;
  PData=Data_Write;
  for(i=0;i<Num;i++)
  {
		Start();
		Send(0xa0);
		Ack();
		Send(Address+i);
		Ack();
		Send(*(PData+i));
		Ack();
		Stop();
		DelayMs(20);
  }
}
/*-----从ROM读数据-----*/
void Read_ROM(uchar Data_Read[],uchar Address,uchar Num)
{
  uchar i;
  uchar *PData;
  PData=Data_Read;
  for(i=0;i<Num;i++)
  {
		Start();
		Send(0xa0);
		Ack();
		Send(Address+i);
		Ack();
		Start();
		Send(0xa1);
		Ack();
		*(PData+i)=Read();
		SCL=0;
		DeAck();
		Stop();
  }
}

/*
void main()
{
	uchar Password_Write[]={"262626"};
	uchar Password_Read[]={"      "};

	SP=0x5f;
  Init_Lcd();

	Write_ROM(Password_Write,0,6);
	Read_ROM(Password_Read,0,6);
	//Change_Screen(Password_Write);
	//DelayMs(3000);
	Change_Screen(Password_Read);
	DelayMs(3000);
}
*/

#endif