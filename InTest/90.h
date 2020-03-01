#ifndef __STC89C5xRC_RDP_H_
#define __STC89C5xRC_RDP_H_

/////////////////////////////////////////////////

/* The following is STC additional SFR */

/* sfr  AUXR  = 0x8e; */
/* sfr  AUXR1 = 0xa2; */
/* sfr  IPH   = 0xb7; */

sfr  P4  = 0xe8;
sbit P46 = P4^6;
sbit P45 = P4^5;       //ISP下载需勾选"ALE脚用作P4.5口"
sbit P44 = P4^4;
sbit P43 = P4^3;
sbit P42 = P4^2;
sbit P41 = P4^1;
sbit P40 = P4^0;

/* Above is STC additional SFR */

/*--------------------------------------------------------------------------
REG51F.H

Header file for 8xC31/51, 80C51Fx, 80C51Rx+
Copyright (c) 1988-1999 Keil Elektronik GmbH and Keil Software, Inc.
All rights reserved.

Modification according to DataSheet from April 1999
 - SFR's AUXR and AUXR1 added for 80C51Rx+ derivatives
--------------------------------------------------------------------------*/

/////////////////////////////////////////////////

#endif

