#include "LCD1602.h"

void write_cmd(uchar b);
void init_lcd1602();
void write_data(uchar c);

/////////////////////////////////////////////////////////////////////
//lcd延时函数
/////////////////////////////////////////////////////////////////////
void delay_lcd(uint a)			  
{
	uint x,y;
	for(x=a;x>0;x--)
		for(y=110;y>0;y--);
} 
              
/////////////////////////////////////////////////////////////////////
//向LCD1602发送指令
/////////////////////////////////////////////////////////////////////
void write_cmd(uchar b)
{
	  rs=0;
	  rw=0;
	  e=0;
	  Data_LCD=b;
	  delay_lcd(5);
	  e=1;
	  delay_lcd(5);
	  e=0;
}
		
/////////////////////////////////////////////////////////////////////
//初始化LCD1602
/////////////////////////////////////////////////////////////////////
void init_lcd1602()
{
	write_cmd(0x38);
	write_cmd(0xc);
	write_cmd(0x6);
	write_cmd(0x01);
}
				
/////////////////////////////////////////////////////////////////////
//向LCD1602发送数据
/////////////////////////////////////////////////////////////////////
void write_data(uchar c)
{
	rs=1;
	rw=0;
	e=0;
	Data_LCD=c;
	delay_lcd(5);
	e=1;
	delay_lcd(5);
	e=0;
}		