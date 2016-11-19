#ifndef __MAIN_H__
#define __MAIN_H__

#define uchar unsigned char
#define uint unsigned int

sbit beep = P1^4;
sbit key_up=P1^3;
sbit key_down=P1^2;
sbit key_add=P1^0;
sbit key_delete=P1^1;

sbit dept = P1^5;	  //¼ÌµçÆ÷¿ØÖÆ¶Ë

#define nop() _nop_()

#define  CLR_BEEP  beep=1
#define  SET_BEEP  beep=0

#define  N_1  1
#define  N_2  2
#define  N_3  3
#define  N_4  4
#define  N_NOP  255
#define NUM 2

#define  READCARD   0xA1
#define  WRITECARD  0xA2
#define  KEYCARD    0xA3
#define  SETTIME    0xA4
#define  SENDID     0xA5

#define  RELOAD_COUNT  0xfb    //18.432M  9600  0xfb

#define ID_NUMBER 10
#define ID_NUMBER_SIZE 4

void disnum(void);
void card_op(void);
void initdesp(void);

#endif