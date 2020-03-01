#ifndef _DELAY_H_
#define _DELAY_H_

#include "Header.h"
#define uint unsigned int
#define uchar unsigned char

/*-----Delay-----*/
void DelayUs(uint us)		//Delay time us
{
  while(us--);
}
void DelayMs(uint ms)		//Delay time ms
{
  uint i,j;
  for(i=0;i<ms;i++)
		for(j=0;j<15;j++)
			DelayUs(1);
}

void DHT11_delay_us(uchar n)
{
    while(--n);
}
void DHT11_delay_ms(uint z)
{
   uint i,j;
   for(i=z;i>0;i--)
      for(j=110;j>0;j--);
}

void Delay_10Us(uchar us)			//Delay for infrared
{
	while(us--);
}

#endif