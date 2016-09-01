# DoorControlSystem
学校基地电子部门禁系统

使用方法：
  1、把下载器的TXD RXD 串口数据线连接对应的板子上的RXD,TXD 不需要下载器的电源，
  2、在主函数打开串口初始化，把main.c文件里的ctrlprocess(void)里的//	sendString(MLastSelectedSnr); //把读到的卡号发送到串口去掉
  3、把新的程序烧录进去之后，打开电脑的串口助手，把需要识别的RFID卡放在识别模块，电脑接收到ID卡号，
  4、把卡号复制到main.c文件里的IDs数组，
  5、修改//	sendString(MLastSelectedSnr); //把读到的卡号发送到串口   语句下的for循环上限
