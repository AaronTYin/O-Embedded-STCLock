#ifndef _ALARM_H_
#define _ALARM_H_

#include <reg52.h>
#include <Delay.h>
#define uint unsigned int

sbit Alarm = P1^4;	//报警

/*---------一声提示-----------*/
void OneAlarm()
{
	Alarm=0;
	DelayMs(200);
  Alarm=1;
}
/*----------两声提示----------*/
void TwoAlarm()
{
	Alarm=0;
	DelayMs(200);
  Alarm=1;
  DelayMs(120);
	Alarm=0;
	DelayMs(200);
  Alarm=1;
}

/*
void main()
{
	//uint i=10;
	while(1)
	{
		OneAlarm();
		DelayMs(4000);
		TwoAlarm();
		DelayMs(4000);
		ThreeAlarm();
		DelayMs(4000);
	}
}
*/

#endif
