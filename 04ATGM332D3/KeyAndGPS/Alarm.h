#ifndef _ALARM_H_
#define _ALARM_H_

#include "Delay.h"

sbit Alarm = P1^0;	//报警

/*-----一声提示-----*/
void OneAlarm()
{
	Alarm=0;
	DelayMs(200);
  Alarm=1;
}
/*-----两声提示-----*/
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

#endif
