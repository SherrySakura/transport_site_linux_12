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

struct clientstruct //�洢client��ַ�Ľṹ��
{
	std::string ip;
	unsigned short port;
	char uid[2];  //�ʹ������ֽ�
};

class UdpThread
{
public:
	void Start();   //�߳̿�ʼ
	void Main();   //�߳�������

	bool ClearFile(const char *filename);
	//void Stop();   //ֹͣ�̵߳�ѭ��
	bool isexit = false;  //�߳��˳��ı�׼
	XUdp udpsock;	   //Ҫ������udpserver�׽���

	bool isonline = false;  	  //connect�Ƿ����   true����

	UdpThread();
	virtual ~UdpThread();

private:

	std::vector<clientstruct> udp_client;//����client��ַ��



	char udp_recv_buf[65535];     //����
	char udp_send_buf[1472];     //����

	sockaddr_in udp_client_addr;      //���յ�ʱ�򻺴������ַ
	int client_num = 0;
	int recv_num = 0;		//��¼�Ѿ��յ��Ĵ���
	bool isUnique = true;   //ip�Ƿ��Ѿ�����

	std::ofstream output;   //�ļ�����
	std::ifstream input;   //�ļ����� ��
	const char * tcpfilename = "./doc/tcpDataBackup.dat";
	const char * udpfilename = "./doc/udpDataBackup.dat";

	int sendsize = 0;
	bool uidmatchok = false;  //�Ƿ�ɹ�ƥ��udi
};

#endif