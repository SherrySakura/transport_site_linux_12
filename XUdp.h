#ifndef XUDP_H
#define XUDP_H

#ifdef WIN32
#include <windows.h>
#define socklen_t int
#else
#include <arpa/inet.h>
#define closesocket close    //�滻close����
#include <unistd.h>
#endif

class XUdp
{
public:
	int CreateSocket();						 //�����׽���
	void Close();						     //�ر�����
	bool Bind();							 //�󶨲������˿ں�
	int Send(const char *buf, int size, const char *ip, unsigned short port);	 //��������
	int Recv(char *buf, int bufsize, sockaddr_in *from);				//��������

	int SetRecvTimeout(int sec);			 //����udp���ճ�ʱ
	int SetSendTimeout(int sec);		     //����udp���ͳ�ʱ

	XUdp(unsigned short port = 9000);
	virtual ~XUdp();

private:
	int usock = 0;  //udp����˵�socket create��Ա�����Լ�����
	unsigned short uport = 0;   //���캯�������ȡ

};
#endif
