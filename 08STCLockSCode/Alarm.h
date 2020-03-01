#ifndef _ALARM_H_
#define _ALARM_H_

#include "Header.h"

sbit Alarm = P1^0;	//Alarm

uchar Alarm_Switch=0;

/*-----One Buzz-----*/
void OneAlarm()
{
	Alarm=0;
	DelayMs(Alarm_Switch);
  Alarm=1;
}
/*-----Two Buzz-----*/
void TwoAlarm()
{
	Alarm=0;
	DelayMs(Alarm_Switch);
  Alarm=1;
  DelayMs(120);
	Alarm=0;
	DelayMs(Alarm_Switch);
  Alarm=1;
}

#endif
