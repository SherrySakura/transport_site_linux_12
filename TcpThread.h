#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include "XTcp.h"
#include <string>
#include <fstream>
#ifdef WIN32
#include <windows.h>
#define socklen_t int
#else
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#endif

class TcpThread
{
public:

	void Start();   //线程开始
	void Main();   //线程主函数

	//void Stop();   //停止线程的循环
	bool isexit = false;  //线程退出的标准
	XTcp tcpsock;	   //要操作的tcp client套接字

	bool ClearFile(const char *filename);  //清空指定文件

	TcpThread();
	virtual ~TcpThread();

private:

	char tcp_recv_buf[65535];     //缓存
	char tcp_send_buf[1460];     //缓存
	int recv_num = 0;		//记录已经收到的次数

	bool isonline = false;  	  //connect是否存在   true正常

	std::ofstream output;   //文件操作 写
	std::ifstream input;   //文件操作 读
	const char * tcpfilename = "./doc/tcpDataBackup.dat";
	const char * udpfilename = "./doc/udpDataBackup.dat";

	long last_pos = 0;   //存放上次读到的地址
	bool isEnd = false;
	int tcp_data_size_max = 1460;   //tcp传递一次报文的最大尺寸设置为1472，再打就要分片，更不安全

	int sendsize = 0;

	time_t lasttime;   //定时操作  计算时间差
	time_t nowtime;
};

#endif