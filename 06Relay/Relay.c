#include <reg52.h>
#include <Delay.h>

sbit R = P1^0;

void main()
{
	R=0;
	DelayMs(2000);
	R=1;
	DelayMs(2000);
}
