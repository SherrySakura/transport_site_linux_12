#ifndef XTCP_H
#define XTCP_H

#include <string>

class XTcp
{
public:
	int CreateSocket();				 //�����׽���
	bool Bind(unsigned short port);  //�󶨶˿ں�
	bool Listen(unsigned short num); //�����˿ں�
	bool SetBlock(bool isblock);  //��������ģʽ  ��ϣ��ֻ����connect��ʱ���Ƿ������ģ�����������ʱ���������ģ�
	bool Connect(const char *ip = "192.168.0.123", unsigned short port = 8000, int sec = 3);
	bool Connect(int sec = 3);
	XTcp Accept();                   //����XTcp���󣬽�������
	void Close();							//�ر�����
	int Recv(char *buf, int size);          //��������
	int Send(const char *buf, int size);	//��������
	int SetRecvTimeout(int sec = 1);			 //����udp���ճ�ʱ
	int SetSendTimeout(int sec = 1);		     //����udp���ͳ�ʱ


	char clientip[16];						//��Ž��յ���client ip
	unsigned short clientport = 0;			//��Ž��յ���client port

	XTcp(unsigned short port = 8000);
    XTcp(char *ip,unsigned short port);
	virtual ~XTcp();


private:
	char tcp_serverip[16] = "";   //tcp_serverip �����ip
	int tsock = 0;							//tcp�ͻ��˵�socket��create�Զ�����
	unsigned short tport = 0;				//����main�����еĲ���  Ҫ����Connect
};

#endif