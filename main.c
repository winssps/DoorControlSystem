#include "reg52.h"
#include "intrins.h"
#include "rc522.h"
#include "LCD1602.h"
#include "main.h"
#include <string.h>


char code IDs[][4]=	//四个字节的序列号
{
  	{0xb4,0x5e,0xd2,0x5f},//万能卡，
	{0xd1,0x4c,0xdf,0xa3},//韦铭
	{0x21,0xcb,0xdc,0x6b},//江旭华
	{0x90,0xe6,0x20,0xcb}, //韦明慧
	{0x30,0xa9,0x1c,0x44},
	{0x50,0xd2,0xc3,0x02},
	{0x71,0xed,0x74,0x6c},
	{0x55,0x8c,0x90,0x30},
	{0xb7,0xd4,0x81,0x3c},
	{0x1d,0xb1,0x65,0xfb},
	{0xa4,0xab,0xd1,0x5f},
	{0x90,0x4e,0xbe,0x65},
	{0x34,0x27,0x8d,0x45}
};

				
uchar idata Read_Data[16] = {0x00};
uchar idata PassWd[6] = {0x00};
uchar idata WriteData[16];
uchar idata RevBuffer[30];
uchar data MLastSelectedSnr[4];

uchar code tab[] = "0123456789ABCDEF";
uchar code welc[] = "Welcome";
uchar code kahao[] = "Card: "; 
uchar xdata show[8];                             //空数组，要显示的8位

uchar code descardnum[] = "Cards Number: ";

uint KuaiN;
uchar oprationcard;

bit flag = 0;                         //有卡标志
uchar j = 0;
uchar card_num = 0;                   //有效卡片数量

/////////////////////////////////////////////////////////////////////
//延时函数
/////////////////////////////////////////////////////////////////////
void delay_ms(uint data tms)
{
	uint ii;
	while(tms)
	{
		tms--;
		for(ii = 0; ii < 400; ii++)
		{
			nop(); nop(); nop(); nop(); nop(); nop();
		}
	}
}

void initCom(void)//初始化串口
{
	TMOD |= 0x20;
	TH1 = 0xf3;
	TL1 = 0Xf3;	  //2400
	SCON |= 0x50;
//	PCON |= 0X80;
	TR1 = 1;

	ES = 1;
	EA = 1;
}

void sendBit(char dat)//发送一个字节数据
{
	ACC = dat;
	SBUF = ACC;
	while(!TI);
		TI = 0;
}

void sendString(char* p)//发送字符串
{
	while(*p)
	{
		sendBit(*p++);
	}
}




/////////////////////////////////////////////////////////////////////
//验证通过提示函数
/////////////////////////////////////////////////////////////////////
void Pass(void)
{
	SET_BEEP;
	delay_ms(15);
	CLR_BEEP;
}

/////////////////////////////////////////////////////////////////////
//初始化RC522模块
/////////////////////////////////////////////////////////////////////
void InitRc522(void)
{
//	key_add = 1;   //电源引脚
	key_down = 1;
	PcdReset();	 //复位
	PcdAntennaOff(); //关闭天线 
	PcdAntennaOn();	 //开启天线 
	M500PcdConfigISOType( 'A' ); //设置RC632的工作方式  //ISO14443_A
}

/////////////////////////////////////////////////////////////////////
//初始化所有模块
/////////////////////////////////////////////////////////////////////
void InitAll(void)
{
	InitRc522();   //初始化读卡器
//	init_lcd1602();	  //初始化1602
	
	KuaiN = 0;		   //
	oprationcard = 0;
	
	Pass();		 //验证通过提示函数
}

/////////////////////////////////////////////////////////////////////
//寻卡，防冲撞，读取卡序列号
/////////////////////////////////////////////////////////////////////
void ctrlprocess(void)
{
	char data status;
	int i=0,j=0,okNumber=0;
	bit stopBuff=1;
	PcdReset();	   
	status = PcdRequest(PICC_REQIDL, &RevBuffer[0]);//寻天线区内未进入休眠状态的卡，返回卡片类型 2字节
	if(status != MI_OK)//成功返回MI_OK	 error
	{
		flag = 0;
		return;
	}
	else    //寻卡ok  天线区有卡
		{flag=1;} 	 

	status = PcdAnticoll(&RevBuffer[2]);//防冲撞，返回卡的序列号 4字节
							/*
								RevBuffer[2]
								RevBuffer[3]
								RevBuffer[4]
								RevBuffer[5]
							*/
	if(status != MI_OK)
	{
		return;
	}

		memcpy(MLastSelectedSnr, &RevBuffer[2], 4);//cpy读到的卡号 
		status = PcdSelect(MLastSelectedSnr);//选卡  成功返回 0
	if(status != MI_OK)
	{
		return;
	}
	else
	{
	//	sendString(MLastSelectedSnr);
		for(i=0;i<13;i++)
		{
		  	for(j=0;j<4;j++)
			{
				if(MLastSelectedSnr[j] == IDs[i][j])okNumber++;
			}	
			if(okNumber == 4)//ok
				{okNumber = 0;dept=0;delay_ms(200);break;}
			else	//error
			   okNumber = 0;
		}  
	}

	if(oprationcard == READCARD)//读卡	   0xA1
	{
		oprationcard = 0;//操作卡
		status = PcdAuthState(PICC_AUTHENT1A, KuaiN, PassWd, MLastSelectedSnr);//验证卡片密码
		if(status != MI_OK)//成功返回 0
			return;
		status = PcdRead(KuaiN, Read_Data);//读取M1卡一块数据 KuaiN =0;
		if(status != MI_OK)//成功返回 0
			return;
		
		PcdHalt();//命令卡片进入休眠状态 成功返回0	
	}     					
}

void convert(uchar *num_ID)	 //显示卡号
{
	for(j = 0; j < 4; j++)
	{
		show[2*j] = (num_ID[j] & 0xf0) >> 4; 
		show[2*j+1] = num_ID[j] & 0x0f;
	}		 
}	
  /*
void disnum(void)
{
	uchar equal = 0;
	uchar i = 0;

	write_cmd(0x01);
	write_cmd(0x84);

	for(j = 0; welc[j] != '\0'; j++)
	{
	  	write_data(welc[j]);	
	}

	write_cmd(0xc0);
	for(j=0; kahao[j]!='\0'; j++)
	{
	  	write_data(kahao[j]);	
	}

	
	for(i=0; i<= NUM; i++)
	{
		equal = 0;

		for(j = 0; j <= 7; j++)
		{
			if (tab[show[j]] != ID[i][j])
			{
				equal = 1;
				break;
			}
		}

		if (equal == 0)
		{
			beep = 0;
			break;
		}
		else
			beep = 1;
	}  

	if (equal == 0)	   
	{
		for(j=0; j<=7; j++)
		{
			write_data(tab[show[j]]);
			show[j] = 0;
		} 
	} 
} */

void initdesp(void)
{
	write_cmd(0x01);
	write_cmd(0x84);

	for(j = 0; welc[j] != '\0'; j++)
	{
	  	write_data(welc[j]);	
	}

	write_cmd(0xc0);
	
	for(j=0; descardnum[j]!='\0'; j++)
	{
	  	write_data(descardnum[j]);	
	}

	write_data(tab[card_num/10]);
 	write_data(tab[card_num%10]);		   
}




bit sdl=1;

void main(void)		 //主函数
{
//	initCom();
 	InitAll();//初始化模块
	while(1)
	{	
				 	 
		if(flag == 1)//失败	
		{
			while(flag)
			  ctrlprocess();
	  	}	 
						
		else//ok	
		{
			dept = 1;
			while(!flag)
				ctrlprocess();
		}	 
	}  
}
