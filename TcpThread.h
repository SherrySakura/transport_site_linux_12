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

	void Start();   //�߳̿�ʼ
	void Main();   //�߳�������

	//void Stop();   //ֹͣ�̵߳�ѭ��
	bool isexit = false;  //�߳��˳��ı�׼
	XTcp tcpsock;	   //Ҫ������tcp client�׽���

	bool ClearFile(const char *filename);  //���ָ���ļ�

	TcpThread();
	virtual ~TcpThread();

private:

	char tcp_recv_buf[65535];     //����
	char tcp_send_buf[1460];     //����
	int recv_num = 0;		//��¼�Ѿ��յ��Ĵ���

	bool isonline = false;  	  //connect�Ƿ����   true����

	std::ofstream output;   //�ļ����� д
	std::ifstream input;   //�ļ����� ��
	const char * tcpfilename = "./doc/tcpDataBackup.dat";
	const char * udpfilename = "./doc/udpDataBackup.dat";

	long last_pos = 0;   //����ϴζ����ĵ�ַ
	bool isEnd = false;
	int tcp_data_size_max = 1460;   //tcp����һ�α��ĵ����ߴ�����Ϊ1472���ٴ��Ҫ��Ƭ��������ȫ

	int sendsize = 0;

	time_t lasttime;   //��ʱ����  ����ʱ���
	time_t nowtime;
};

#endif