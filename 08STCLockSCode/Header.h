#ifndef _HEADER_H_
#define _HEADER_H_

#include <reg52.h>
#include "90.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <intrins.h>	//_nop_()
#include "Delay.h"
#include "Alarm.h"
#include "Infrared.h"
#include "KeyControl.h"
#include "UART.h"
#include "AT24C02.h"
#include "Index.h"
#include "DHT11.h"
#include "JudgePassword.h"
#include "GPS.h"
#include "Infrared.h"

void ATY_Init()
{
	P42=0;
	Relay=1;
	Uart_Init();
	Delay_ms(10);
	clrStruct();
	//SP=0x5f;
	Init_Lcd();
	CloseLock=1;
	OpenLock=0;
	RR_DATA=0;
	F_Scan=1;
	F_Input=1;
	F_Success=1;
	F_Fail=1;
}


/*sbit Alarm = P1^0;	//Alarm

sbit CS   = P1^1;			//LCD
sbit STD  = P1^2;
sbit SCLK = P1^3;
sbit PSB  = P0^0;
sbit RST  = P0^1;

sbit SCL = P1^4;			//24C02 Serial clock
sbit SDA = P1^5;			//24C02 Serial data

sbit CloseLock = P1^6;
sbit OpenLock = P1^7;

Keyboard P2						//Keyboard

GPS UART P3^0;				//GPS

sbit Relay = P3^1;		//Relay

sbit RR_DATA = P3^2;	//Infrared

sbit Data = P3^3;			//DHT11

sbit F_Scan = P3^4;		//Finger
sbit F_Input = P3^5;
sbit F_Success = P3^6;
sbit F_Fail = P3^7;

*/

#endif