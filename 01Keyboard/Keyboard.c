#include <reg52.h>

#define uchar unsigned char
#define uint unsigned int

uchar code KC[]={0xFE,0xFD,0xFB,0xF7};	//键盘扫描控制列表

/*-----10ms延迟-----*/
void Delay_10Ms()
{
	uint TempCyc=11104;
	while(TempCyc--);
}
/*-----按键检测并返回按键值-----*/
uchar Key_Check()
{
 	uchar Row,Col,i;
 	P2=0xf0;	//P2接口初始化
 	if((P2&0xf0)!=0xf0)
 	{
		Delay_10Ms();
		if((P2&0xf0)!=0xf0)
		{
			Row=P2^0xf0;	//确定行线
			i=0;
			P2=KC[i];	//精确定位
			while(i<4)
			{
	 			if((P2&0xf0)!=0xf0)
				{
					Col=~(P2&0xff);	//确定列线
					break;	//已定位后提前退出
	  		}
				else 
				{
					i++;
					P2=KC[i];
				}
			}
		}
		else 
		{
			return 0;
		}
		while((P2&0xf0)!=0xf0);
		P0=Row|Col;	//P0接8个LED显示行列值
		return Row|Col;	//行线与列线组合后返回
 	}
 	else return 0;	//无键按下时返回0
}

void main()
{
	while(1) 
	{ 
		Key_Check();
	}
}
