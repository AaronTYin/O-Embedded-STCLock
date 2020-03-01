#include <reg52.h>
#include "90.h"

#define TRUE  	0x01
#define FALSE 	0x00

#define DATA_START			0xf5	//数据包开始
#define DATA_END				0xf5	//数据包结束

#define CMD_ENROLL1  		0x01	//添加指纹步骤一
#define CMD_ENROLL2  		0x02	//添加指纹步骤二
#define CMD_ENROLL3  		0x03	//添加指纹步骤三
#define CMD_DELETE  		0x04	//删除指定编号指纹
#define CMD_CLEAR  			0x05	//清空所有指纹
#define CMD_USERNUMB  	0x09	//取用户总数
#define CMD_IDENTIFY  	0x0b	//1:1比对
#define CMD_SEARCH  		0x0c	//1:N比对

#define ACK_SUCCESS  		0x00	//操作成功
#define ACK_FAIL	  		0x01	//操作失败
#define ACK_FULL	  		0x04	//指纹数据库已满
#define ACK_NOUSER   		0x05	//无此用户
#define ACK_USER_EXIST 	0x07 	//用户已存在
#define ACK_TIMEOUT  		0x08	//采集超时

#define UART_BUF_LEN		8
#define BUF_N 					8

#define uint unsigned int
#define uchar unsigned char

#define BEEP  P42
sbit F_Scan = P0^0;
sbit F_Input = P0^1;
sbit F_Success = P0^2;
sbit F_Fail = P0^3;
sbit O=P1^1;

uchar rBuf[UART_BUF_LEN];          //接收返回信息
uchar tBuf[UART_BUF_LEN];          //发送命令或者数据
uchar g_ucUartRxEnd ;              //接收返回信息结束标志
uchar g_ucUartRxLen ;       	     //接收返回信息长度
uchar l_ucFPID;             	     //用户编号

uchar p;

void UartInit(void)		//19200bps@11.0592MHz
{
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;			//设定定时初值
	TH1 = 0xFD;			//设定定时器重装值
	ET1 = 0;				//禁止定时器1中断
	TR1 = 1;				//启动定时器1
	ES = 1 ;
	EA = 1 ;
}
void SendData(uchar dat)
{
	SBUF=dat;
	while(!TI);
	TI=0;
}
void UartSend(uchar *Datar,uchar cLength)
{
	do{
		SendData(*(Datar++));
	}while(--cLength != 0);
}
void Uart_Isr() interrupt 4 using 1
{    
	if(RI)
	{
		RI = 0;		//Clear receive interrupt flag
		rBuf[p++] = SBUF;
		if(p==8)
			g_ucUartRxEnd = 0xff;
	}
}
void Delay_ms(uint ms)                                                         
{                                                                          
	uint i=0,j=0;
	for(i=0;i<ms;i++)
		for(j=0;j<123;j++);
}
/*******************************************************************************
**功能：等待数据包发送完成
*******************************************************************************/
uchar WaitFpData(void)
{
  uchar i;
  
  for(i=200; i>0; i--)	//等待指纹芯片返回
  {
    Delay_ms(40);
    if(g_ucUartRxEnd)	break;
  }  
  if(i==0)
		return FALSE;	//指纹芯片没有返回
  else
		return TRUE;
}
/*******************************************************************************
**功能: 计算校验值
**参数: 要发送的数据指针地址
**返回: 校验值
*******************************************************************************/
uchar CmdGenCHK(uchar wLen,uchar *ptr)
{
	uchar i,temp = 0;
	
	for(i = 0; i < wLen; i++)
	{
		temp ^= *(ptr + i);
	}
	return temp;
}
/*******************************************************************************
**功能: 发送控制指纹芯片指令
**参数: wLen 数据长度
        cpPara 发送的数据
**返回：void
*******************************************************************************/
void UART_SendPackage(uchar wLen,uchar *ptr)
{
  uint i=0,len=0;
 
  tBuf[0] = DATA_START;	//指令包
  for(i = 0; i < wLen; i++)	// data in packet 
  {
    tBuf[1+i] = *(ptr+i);
  } 
  
  tBuf[wLen + 1] = CmdGenCHK(wLen, ptr);	//Generate checkout data
  tBuf[wLen + 2] = DATA_END;
  len = wLen + 3;
	
  g_ucUartRxEnd = 0;
  g_ucUartRxLen = len ;
	
	UartSend(tBuf,len);
} 
/*******************************************************************************
**功能：返回信息处理
**参数： cmd 不同命令不同处理
**返回：处理结果
*******************************************************************************/
uchar Check_Package(uchar cmd)
{
	uchar flag = FALSE;
	if(!WaitFpData())
		return flag; //等待接收返回信息
	p = 0 ;
	if(g_ucUartRxEnd)
		g_ucUartRxEnd = 0;	//清数据包接收标志
	else 
		return flag;

  if(rBuf[0] != DATA_START)
		return flag;
	if(rBuf[1] != cmd)
		return flag;
	if(rBuf[6] != CmdGenCHK(g_ucUartRxLen - 3, &rBuf[1]))
		return flag;
	switch(cmd)
	{
		case CMD_ENROLL1:
		case CMD_ENROLL2:
		case CMD_ENROLL3:
			if(ACK_SUCCESS == rBuf[4])
				flag = TRUE;
			else if(ACK_USER_EXIST == rBuf[4])
			{
				//Spk_HaveUser();
				Delay_ms(1500);
			}
			break;
		case CMD_DELETE:  //删除指定编号指纹
		case CMD_CLEAR:    //清空所有指纹
		case CMD_IDENTIFY:  //1:1比对
			if(ACK_SUCCESS == rBuf[4])
				flag = TRUE;
			break;
		case CMD_USERNUMB:  //取用户总数
			if(ACK_SUCCESS == rBuf[4])
			{
				flag = TRUE;
				l_ucFPID = rBuf[3];
			}
			break;
		case CMD_SEARCH:   //1:N比对
			if((1 == rBuf[4])||(2 == rBuf[4])||(3 == rBuf[4]))
			{
				flag = TRUE;
				l_ucFPID = rBuf[3];
			}
			break;
		default:
			break;
	}
    return flag;
}
/*******************************************************************************
**功能：以CharBuffer1 或CharBuffer2 中的特征文件搜索整个或部分指纹库
*******************************************************************************/
void FP_Search(void)
{
  uchar buf[BUF_N];
  
  *buf = CMD_SEARCH;          //1:N比对
  *(buf+1) = 0x00;
  *(buf+2) = 0x00;
  *(buf+3) = 0x00;
  *(buf+4) = 0x00;

  UART_SendPackage(5, buf);
}
/*******************************************************************************
**功能：清空 flash 指纹库
*******************************************************************************/
void FP_Clear(void)
{
  uchar buf[BUF_N];
  
  *buf = CMD_CLEAR;
  *(buf+1) = 0x00;
  *(buf+2) = 0x00;
  *(buf+3) = 0x00;
  *(buf+4) = 0x00;

  UART_SendPackage(5, buf);
}
/*******************************************************************************
**功能：删除指定编号指纹
**参数：u_id
**返回：void
*******************************************************************************/
void FP_Delete(uint u_id)
{
  uchar buf[BUF_N];
  
  *buf = CMD_DELETE;
  *(buf+1) = u_id>>8;
  *(buf+2) = u_id&0xff;
  *(buf+3) = 0x00;
  *(buf+4) = 0x00;
  UART_SendPackage(5, buf);
}
/*******************************************************************************
**功能：1:1比对
**参数：u_id
**返回：void
*******************************************************************************/
void FP_Identify(uint u_id)
{
  uchar buf[BUF_N];
  
  *buf = CMD_IDENTIFY;
  *(buf+1) = u_id>>8;
  *(buf+2) = u_id&0xff;
  *(buf+3) = 0x00;
  *(buf+4) = 0x00;
  UART_SendPackage(5, buf);
}
/*******************************************************************************
**注册指纹
**输入两次指纹注册一个指纹模板
**参数：UserID 指纹号
*******************************************************************************/
void Enroll_Step1(uint u_id)
{
  uchar buf[BUF_N];
  
  *buf = CMD_ENROLL1;
  *(buf+1) = u_id>>8;
  *(buf+2) = u_id&0xff;
  *(buf+3) = 1;
  *(buf+4) = 0x00;

  UART_SendPackage(5, buf);
}
/*******************************************************************************
**注册指纹
**输入两次指纹注册一个指纹模板
**参数：UserID 指纹号
*******************************************************************************/
void Enroll_Step2(uint u_id)
{
  uchar buf[BUF_N];
  
  *buf = CMD_ENROLL2;
  *(buf+1) = u_id>>8;
  *(buf+2) = u_id&0xff;
  *(buf+3) = 1;
  *(buf+4) = 0x00;

  UART_SendPackage(5, buf);
}
/*******************************************************************************
**注册指纹
**输入三次指纹注册一个指纹模板
**参数：UserID 指纹号
*******************************************************************************/
void Enroll_Step3(uint u_id)
{
  uchar buf[BUF_N];
  
  *buf = CMD_ENROLL3;
  *(buf+1) = u_id>>8;
  *(buf+2) = u_id&0xff;
  *(buf+3) = 1;
  *(buf+4) = 0x00;

  UART_SendPackage(5, buf);
}
/*******************************************************************************
**注册指纹
**输入三次指纹注册一个指纹模板
**参数：UserID 指纹号
*******************************************************************************/
uchar Finger_Enroll(uint u_id)
{
	Enroll_Step1(u_id);
	if(FALSE == Check_Package(CMD_ENROLL1))
		return FALSE;
	Delay_ms(100);
	
	Enroll_Step2(u_id);
	if(FALSE == Check_Package(CMD_ENROLL2))
		return FALSE;
	Delay_ms(100);
	
	Enroll_Step3(u_id);
	return Check_Package(CMD_ENROLL3);
}
/*******************************************************************************
**清空指纹
**参数：UserID 指纹号
*******************************************************************************/
uchar Finger_Clear(void)
{
	FP_Clear();
	//if(FALSE == WaitFpData())return FALSE;
	return Check_Package(CMD_CLEAR);
}
/*******************************************************************************
**删除指定指纹
**参数：UserID 指纹号
*******************************************************************************/
uchar Finger_Delete(uint u_id)
{
	FP_Delete(u_id);
	//if(FALSE == WaitFpData())return FALSE;
	return Check_Package(CMD_DELETE);
}
/*******************************************************************************
**读取用户总数
*******************************************************************************/
uchar Finger_Read(void)
{
	 uchar buf[BUF_N];
  
  *buf = CMD_USERNUMB;
  *(buf+1) = 0x00;
  *(buf+2) = 0x00;
  *(buf+3) = 0x00;
  *(buf+4) = 0x00;
  UART_SendPackage(5, buf);
	return Check_Package(CMD_USERNUMB);
}
/*******************************************************************************
**读取用户总数
*******************************************************************************/
uchar Finger_Search(void)
{
	FP_Search();
	return Check_Package(CMD_SEARCH);
}

void BBP(uchar times)
{
	uchar i;  
	switch (times)
	{
		case 1:
			F_Success=0;
			F_Fail=1;
			BEEP=~BEEP;
			Delay_ms(200);
			BEEP=~BEEP;
			break;
			
		case 2:
			BEEP=~BEEP;
			Delay_ms(200);
			BEEP=~BEEP;
			Delay_ms(200);
			BEEP=~BEEP;
			Delay_ms(200);
			BEEP=~BEEP;
			break;
			
		case 3:
			F_Success=1;
			F_Fail=0;
			BEEP=~BEEP;
			Delay_ms(200);
			BEEP=~BEEP;
			Delay_ms(200);
			BEEP=~BEEP;
			Delay_ms(200);
			BEEP=~BEEP;
			Delay_ms(200);
			BEEP=~BEEP;
			Delay_ms(200);
			BEEP=~BEEP;
			break;
			
		default:
			break;
	}
}
void Mode(uchar MODE)
{
	uchar User_ID;
	switch ( MODE )
	{                                                                     
		case 0:
			break;

		case 1:
			MODE = 0;	//增加指纹用户
			if(Finger_Enroll(++User_ID))
				BBP(1);
			else
				BBP(3);
			Delay_ms(2000);
			break;

		case 2:
			MODE = 0;	//删除单个指纹用户
			if(Finger_Search())
				BBP(2);
			else
			{
				BBP(3);
				break;
			}
			if(Finger_Delete(l_ucFPID))
				BBP(1);
			else
				BBP(3);
			Delay_ms(1000);
			break;

		case 3:
			MODE = 0;	//1:N搜索指纹库
			if(Finger_Search())
				BBP(1);
			else
				BBP(3);
			Delay_ms(1000);
			break;
		
		case 4:
			MODE = 0;	//读指纹用户总数
			if(Finger_Read())
				BBP(1);
			else
				BBP(3);
			Delay_ms(1000);
			break;

		case 5:
			MODE = 0;	//清除指纹库
			if(Finger_Clear())
				BBP(1);
			else
				BBP(3);
			Delay_ms(1000);
			break;
		
		default:
			break;
	}
}

void main()
{
	F_Scan=1;
	F_Input=1;
	F_Success=1;
	F_Fail=1;
	O=0;
	P42=0;
	

	UartInit();
	BBP(2);
	while(1)
	{
		F_Success=1;
		F_Fail=1;
		if(!F_Scan)
		{
			while(!F_Scan);
			BBP(2);
			Mode(3);
		}
		else if(!F_Input)
		{
			while(!F_Input);
			BBP(2);
			Mode(1);
		}
	}
}
