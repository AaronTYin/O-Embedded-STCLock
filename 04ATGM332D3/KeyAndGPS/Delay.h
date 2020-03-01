#ifndef _DELAY_H_
#define _DELAY_H_

#include <reg52.h>
#define uint unsigned int
#define uchar unsigned char

/*-----—” ±-----*/
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

#endif