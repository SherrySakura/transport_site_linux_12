#include "XUdp.h"
#include <iostream>	//IO������
#include <string>   //string��׼��
#ifdef WIN32
#include <windows.h>
#define socklen_t int

#else
#include <arpa/inet.h>
#define closesocket close    //�滻close����
#include <unistd.h>
#include <pthread.h>
#endif

XUdp::XUdp(unsigned short port)
{
	//��ʼ����̬���ӿ�
	//����lib��
#ifdef WIN32  //linux�²��ó�ʼ�������
	static bool first = true;
	if (first)
	{
		first = false;						 //ֻ���״ν���ʱ��ʼ�������
		WSADATA ws;					         //����Socket��   ��Ŀ����-������-������� ws2_32.lib
		WSAStartup(MAKEWORD(2, 2), &ws);     //��̬�����ü�1    
	}
#endif
	uport = port;
}

XUdp::~XUdp()
{
	delete this;
}

int XUdp::CreateSocket()						 //�����׽���
{
	usock = socket(AF_INET, SOCK_DGRAM, 0);   //TCP/IP  UDP  ����udp �׽���
	if (usock == -1)
	{
		printf("create udp socket failed.\n");
		return -1;
	}
	printf("create udp socket success.\n");
	return 0;
}

void XUdp::Close()					  //�ر�����
{
	if (usock <= 0) return;  //socket����
	closesocket(usock);		//�Ѻ궨��

	usock = 0;
	uport = 0;
}

bool XUdp::Bind()			 //�󶨲������˿ں�
{
	sockaddr_in saddr;              //���ݽṹ
	saddr.sin_family = AF_INET;     //Э��
	saddr.sin_port = htons(uport);   //�˿ڣ������ֽ���С�˷�ʽ��ת���������ֽ��򣨴�˷�ʽ��
									 //  saddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);                  
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);   //��IP���㲥��ַINADDR_ANY 0.0.0.0  Ϊ�˼���linux   

	if (bind(usock, (sockaddr*)&saddr, sizeof(saddr)) != 0)   //��װsockaddr_in���ݽṹ���׽���
	{
		printf("udp bind port %d failed.\n", uport);
		return false;
	}
	printf("udp bind port %d success.\n", uport);
	return true;
}

int XUdp::Send(const char *buf, int size, const char *ip, unsigned short port)	 //��������(ǿ��ȫ������)
{
	int sendedSize = 0;   //�ѷ��ͳɹ��ĳ���
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;  //tcp/ipЭ��
	saddr.sin_port = htons(port);//����˵Ķ˿�  �����ֽ���ת���������ֽ���
	saddr.sin_addr.s_addr = inet_addr(ip);  //������ip��ַ  �ַ���ip��ַת������
	while (sendedSize != size)   //��û������ɣ���Ӷϵ㿪ʼ�������� ֱ�����
	{
		int len = sendto(usock, buf + sendedSize, size - sendedSize, 0, (sockaddr*)&saddr, sizeof(saddr));
		if (len <= 0)break;
		sendedSize += len;
	}
	return sendedSize;
}


int XUdp::Recv(char *buf, int bufsize, sockaddr_in *from)	 //��������
{
	socklen_t len = sizeof(sockaddr_in);
	int re = recvfrom(usock, buf, bufsize, 0, (sockaddr*)from, &len);   //���ؽ��յĿͻ��˵������ַ�������ڵ�ַ��
	return re;
}

int XUdp::SetRecvTimeout(int sec = 1)   //����udp���ճ�ʱ
{
#ifdef WIN32
	int udp_rev_time = sec * 1000;
	if (setsockopt(usock, SOL_SOCKET, SO_RCVTIMEO, (char *)&udp_rev_time, sizeof(int))<0)
	{
		printf("set udp receive failed.\n");
		return -1;
	}
	printf("set udp recv timeout success. %d seconds\n", sec);
	return 0;
#else
	struct timeval udp_rev_time;
	udp_rev_time.tv_sec = sec;
	udp_rev_time.tv_usec = 0;
	if (setsockopt(usock, SOL_SOCKET, SO_RCVTIMEO, (char *)&udp_rev_time, sizeof(udp_rev_time))<0)
	{
		printf("set udp receive failed.\n");
		return -1;
	}
	printf("set udp recv timeout success. %d seconds\n", sec);
	return 0;
#endif
}


int XUdp::SetSendTimeout(int sec = 1)   //����udp���ͳ�ʱ
{
#ifdef WIN32
	int udp_rev_time = sec;
	if (setsockopt(usock, SOL_SOCKET, SO_SNDTIMEO, (char *)&udp_rev_time, sizeof(int))<0)
	{
		printf("set udp send failed.");
		return -1;
	}
	return 0;
	printf("set udp send timeout success. %d seconds\n", sec);
#else
	struct timeval udp_rev_time;
	udp_rev_time.tv_sec = sec;
	udp_rev_time.tv_usec = 0;
	if (setsockopt(usock, SOL_SOCKET, SO_SNDTIMEO, (char *)&udp_rev_time, sizeof(udp_rev_time))<0)
	{
		printf("set udp send failed.");
		return -1;
	}
	printf("set udp send timeout success. %d seconds\n", sec);
	return 0;
#endif
}