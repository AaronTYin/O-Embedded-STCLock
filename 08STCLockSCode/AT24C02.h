#ifndef _AT24C02_H_
#define _AT24C02_H_

#include "Header.h"

sbit SCL = P1^4;			//24C02 Serial clock
sbit SDA = P1^5;			//24C02 Serial data

/*-----Do Nop-----*/
void Nop()
{
 	_nop_();	//About 1us per command
 	_nop_();
 	_nop_();
 	_nop_();
}
/*-----Start condition-----*/
void Start()
{
 	SDA=1;
 	SCL=1;
 	Nop();
 	SDA=0;
 	Nop();
}
/*-----Stop condition-----*/
void Stop()
{
 	SDA=0;
 	SCL=1;
 	Nop();
 	SDA=1;
 	Nop();
}
/*-----Acknowledgement-----*/
void Ack()
{
	SDA=0;
	Nop();
	SCL=1;
	Nop();
	SCL=0;
}
/*-----DeAcknowledgement-----*/
void DeAck()
{
 	SDA=1;
 	Nop();
 	SCL=1;
 	Nop();
 	SCL=0;
}

/*-----Send data-----*/
void Send(uchar Data_Send)
{
   	uchar BitCounter=8;
   	uchar Temp;
   	do
   	{
   		Temp=Data_Send;	//Save getted data to 'Temp' temporary
   		SCL=0;
   		Nop();
   		if((Temp&0x80)==0x80)
				SDA=1;
   		else 
				SDA=0;
   		SCL=1;
   		Temp=Data_Send<<1;
   		Data_Send=Temp;
   		BitCounter--;	//Data send over when Bitcounter subtracted to 0
   	}
   	while(BitCounter);	//Judge whether it is completed or not
   	SCL=0;
}
/*-----Read one byte data and return-----*/
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
		if(SDA)
			Data_Read=Data_Read|0x01;	//Data_Read's minimum bit is 1£¨|0x01,is change the minimum bit to 1£©
		else	//If SDA = 0
			Data_Read=Data_Read&0xfe;	//Data_Read's minimum bit is 0£¨&0xfe 11111110 is change the bit position to 0£©
		if(BitCounter-1)	//Judge the BitCounter is true or not after subtracted 1
		{
			Temp=Data_Read<<1;
			Data_Read=Temp;
		}
		BitCounter--;	//Getting data over while BitCounter subtract to 0
	}
	while(BitCounter);	//Judge whether it is completed or not
	return Data_Read;
}

/*-----Write data to ROM-----*/
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
/*-----Read data from ROM-----*/
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

#endif