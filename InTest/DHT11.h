#ifndef _DHT11_H_
#define _DHT11_H_

#include "Header.h"

sbit Data = P3^3;

void DHT11_start()
{
   Data=1;
   DHT11_delay_us(2);
   Data=0;
   DHT11_delay_ms(20);
   Data=1;
   DHT11_delay_us(30);
}

uchar DHT11_rec_byte()
{
	uchar i,dat=0;
  for(i=0;i<8;i++)
   {          
      while(!Data);
      DHT11_delay_us(8);
      dat<<=1;
      if(Data==1)
         dat+=1;
      while(Data);
    }  
    return dat;
}

uint DHT11_receive()
{
    uchar R_H,R_L,T_H,T_L,RH,RL,TH,TL,revise;
    DHT11_start();
    if(Data==0)
    {
        while(Data==0);
        DHT11_delay_us(40);
        R_H=DHT11_rec_byte();
        R_L=DHT11_rec_byte();
        T_H=DHT11_rec_byte();
        T_L=DHT11_rec_byte();
        revise=DHT11_rec_byte();
        DHT11_delay_us(25);

        if((R_H+R_L+T_H+T_L)==revise)
        {
            RH=R_H;
            RL=R_L;
            TH=T_H;
            TL=T_L;
        }
				RH='0'+(RH/10);
				if(RH>'7')
					return 1;
    }
		return 0;
}

#endif