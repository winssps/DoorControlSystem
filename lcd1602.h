#include <reg52.H>
#ifndef __LCD1602_H__
#define __LCD1602_H__

#define uchar unsigned char
#define uint unsigned int

sbit rs = P3^5;
sbit rw = P3^6;
sbit e = P3^7;
#define Data_LCD P0

void write_cmd(uchar b);
void init_lcd1602();
void write_data(uchar c);
void delay_lcd(uint a);

#endif