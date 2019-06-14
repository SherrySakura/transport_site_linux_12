#ifndef XUDP_H
#define XUDP_H

#ifdef WIN32
#include <windows.h>
#define socklen_t int
#else
#include <arpa/inet.h>
#define closesocket close    //替换close函数
#include <unistd.h>
#endif

class XUdp
{
public:
	int CreateSocket();						 //创建套接字
	void Close();						     //关闭连接
	bool Bind();							 //绑定并监听端口号
	int Send(const char *buf, int size, const char *ip, unsigned short port);	 //发送数据
	int Recv(char *buf, int bufsize, sockaddr_in *from);				//接收数据

	int SetRecvTimeout(int sec);			 //设置udp接收超时
	int SetSendTimeout(int sec);		     //设置udp发送超时

	XUdp(unsigned short port = 9000);
	virtual ~XUdp();

private:
	int usock = 0;  //udp服务端的socket create成员函数自己生成
	unsigned short uport = 0;   //构造函数从外获取

};
#endif
