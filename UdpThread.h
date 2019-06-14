#ifndef UDPTHREAD_H
#define UDPTHREAD_H

#include "XUdp.h"
#include <string>
#include <fstream>
#include <vector>
#ifdef WIN32
#include <windows.h>
#define socklen_t int
#else
#endif

struct clientstruct //存储client地址的结构体
{
	std::string ip;
	unsigned short port;
	char uid[2];  //就存两个字节
};

class UdpThread
{
public:
	void Start();   //线程开始
	void Main();   //线程主函数

	bool ClearFile(const char *filename);
	//void Stop();   //停止线程的循环
	bool isexit = false;  //线程退出的标准
	XUdp udpsock;	   //要操作的udpserver套接字

	bool isonline = false;  	  //connect是否存在   true正常

	UdpThread();
	virtual ~UdpThread();

private:

	std::vector<clientstruct> udp_client;//缓存client地址表



	char udp_recv_buf[65535];     //缓存
	char udp_send_buf[1472];     //缓存

	sockaddr_in udp_client_addr;      //接收的时候缓存网络地址
	int client_num = 0;
	int recv_num = 0;		//记录已经收到的次数
	bool isUnique = true;   //ip是否已经存在

	std::ofstream output;   //文件操作
	std::ifstream input;   //文件操作 读
	const char * tcpfilename = "./doc/tcpDataBackup.dat";
	const char * udpfilename = "./doc/udpDataBackup.dat";

	int sendsize = 0;
	bool uidmatchok = false;  //是否成功匹配udi
};

#endif