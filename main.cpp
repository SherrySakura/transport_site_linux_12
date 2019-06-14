#include <iostream>	//IO流操作
#include <string>   //string标准库
#include "string.h"   //string标准库
#include <fstream>   //文件操作

#include "global.h"
#include "XUdp.h"
#include "XTcp.h"
#include "UdpThread.h"
#include "TcpThread.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#endif



int main(int argc, char *argv[])
{
#ifdef WIN32
	static bool first = true;
	if (first)
	{
		first = false; //只在首次进入时初始化网络库
		WSADATA ws; //加载Socket库   项目属性-链接器-输入加上 ws2_32.lib
		WSAStartup(MAKEWORD(2, 2), &ws); //动态库引用加1  
	}
#endif
	//初始化端口
	char tcp_serverip[16] = "192.168.0.123";  //tcp_serverip 服务端ip
	unsigned short tcp_port = 8000; //tcp_port 对服务端默认端口8000
	unsigned short udp_port = 9000; //udp_port 对充电桩默认端口9000
	if (argc > 1)
	{
		if (argc == 2)
		{
			strcpy(tcp_serverip,argv[1]);   //第一个参数给tcp
		}
		if (argc == 3)
		{
			strcpy(tcp_serverip,argv[1]);   //第一个参数给tcp
			tcp_port = atoi(argv[2]);   //第一个参数给udp
		}
		if (argc == 4)
		{
			strcpy(tcp_serverip,argv[1]);   //第一个参数给tcp
			tcp_port = atoi(argv[2]);   //第二个参数给tcp
			udp_port = atoi(argv[3]);   //第三个参数给udp
		}
	}

	//初始化文件(建立文件并清空)	
	std::ofstream output;  
	const char * udpfilename = "./doc/udpDataBackup.dat";
	const char * tcpfilename = "./doc/tcpDataBackup.dat";

	output.open(udpfilename, std::ofstream::out | std::ios::binary| std::ios::trunc);   //关联输出流对象和文件(输出|二进制|定位到末尾)
	if (output.is_open())  //关联的文件是否打开
	{
		std::cout << "udpDataBackup file open sucess." << std::endl;
		std::cout << "udpDataBackup file clear sucess." << std::endl;
		output.close();
	}
	else
	{
		std::cout << "udpDataBackup file open failed." << std::endl;
	}
	output.open(tcpfilename, std::ofstream::out | std::ios::binary| std::ios::trunc);   //关联输出流对象和文件(输出|定位到末尾)
	if (output.is_open())  //关联的文件是否打开
	{
		std::cout << "tcpDataBackup file open sucess." << std::endl;
		std::cout << "tcpDataBackup file clear sucess." << std::endl;
		output.close();
	}
	else
	{
		std::cout << "tcpDataBackup file open failed." << std::endl;
	}

	//初始化读写锁
	pthread_rwlock_init(&rwlock_tcp_flag_txt,NULL);//读写锁 （保护tcpData.txt和udpData.txt）  先读后写
	pthread_rwlock_init(&rwlock_udp_flag_txt,NULL);//读写锁 （保护tcpData.txt和udpData.txt）


	XUdp udp_sock = XUdp(udp_port);   //外部的XUdp只用来接收main函数的参数，其余的操作都在线程中进行
	UdpThread *th_udp = new UdpThread();
	th_udp->udpsock = udp_sock;       //接收XUdp，即接收main函数的参数
	th_udp->Start();

	XTcp tcp_sock = XTcp(tcp_serverip,tcp_port);    //外部的XTcp只用来接收main函数的参数，其余的操作都在线程中进行
	TcpThread *th_tcp = new TcpThread();
	th_tcp->tcpsock = tcp_sock;
	th_tcp->Start();

	while (true)
	{

					//sleep(1);

					//pthread_rwlock_wrlock(&rwlock_udp_flag_txt);  
					//sleep(3);  //睡眠3秒
					//pthread_rwlock_unlock(&rwlock_udp_flag_txt);  //解锁
					//sleep(3);  //睡眠3秒
	}
	
	pthread_rwlock_destroy(&rwlock_tcp_flag_txt);//销毁读写锁  
	pthread_rwlock_destroy(&rwlock_udp_flag_txt);//销毁读写锁 
}