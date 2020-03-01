#include <reg52.h>
#define uint unsigned int

sbit Alarm = P1^5;	//报警

/*------延时-----*/
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

void main()
{
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
		

