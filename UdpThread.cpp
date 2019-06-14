#include "UdpThread.h"
#include <iostream>
#include <thread>
#include <cstring>
#include "global.h"

#ifdef WIN32
#else
#include <pthread.h>
#endif

UdpThread::UdpThread()
{
}


UdpThread::~UdpThread()
{
}
//ʵ��string��unsigned int��ת��  
unsigned int string_to_unsigned_int(std::string str)  
{  
    unsigned int result(0);//���ɱ�ʾֵΪ4294967296��=2��32-1��  
    //���ַ�����λ��ȡ��ĩλ���±���0��str.size() - 1��  
    for (int i = str.size()-1;i >= 0;i--)  
    {  
        unsigned int temp(0),k = str.size() - i - 1;  
        //�ж��Ƿ�Ϊ����  
        if (isdigit(str[i]))  
        {  
            //��������������λ��  
            temp = str[i] - '0';  
            while (k--)  
                temp *= 10;  
            result += temp;  
        }  
        else  
            //exit(-1);  
            break;  
    }  
    return result;  
} 

void UdpThread::Start()   //�߳̿�ʼ
{
	//�����߳�
	isexit = false;
	std::thread sth(&UdpThread::Main, this);	//<����ָ�룬���õĶ���>
	sth.detach();  //�ͷ����̵߳Ŀ���Ȩ����Դ��
	printf("udp thread start.\n");
}
//udp_client
void UdpThread::Main()   //�߳�������
{
	//UDP��ʼ��
	udpsock.CreateSocket();
	udpsock.Bind();			 //�󶨶˿ں�
	udpsock.SetRecvTimeout(1);

	int length = sizeof(udp_client_addr);
	while (!isexit)
	{
		int rev_len = udpsock.Recv(udp_recv_buf, sizeof(udp_recv_buf), &udp_client_addr);	 //��������
		if (rev_len > 0)  //���ճɹ�û���ؽ��յ����ֽ���
		{
			recv_num++;
			isUnique = true;
			printf("recv_num:%d\n", recv_num);
			printf("udp receive success. length:%d\n", rev_len);
			//��ȡ�ͻ���ip
			printf("udp client's ip is %s  port: %d\n", inet_ntoa(udp_client_addr.sin_addr), ntohs(udp_client_addr.sin_port));

			//���������Ϣ   
			udp_recv_buf[rev_len] = '\0';  //�ڽ����ַ�����β��ǿ��'\0'
			printf("udp_recv_buf: %s\n", udp_recv_buf);

			clientstruct udp_newclient; //������client
			if (client_num>0)
			{
				for (auto &v : udp_client)  //��Χ����udp_client�е�ÿһ��Ԫ��
				{
					if (std::string(inet_ntoa(udp_client_addr.sin_addr)) == v.ip)  //ֻҪip��ͬ
					{
						isUnique = false;  //�Ź�����ip��¼
						break;
					}
				}
			}
			if (isUnique == true)
			{
				udp_newclient.ip = inet_ntoa(udp_client_addr.sin_addr);
				udp_newclient.port = ntohs(udp_client_addr.sin_port);
				udp_newclient.uid[0] = udp_recv_buf[0];
				udp_newclient.uid[1] = udp_recv_buf[1];

				printf("uid:--------------char[0]: %cchar[1]: %c  \n",udp_newclient.uid[0],udp_newclient.uid[1]);

				udp_client.push_back(udp_newclient);  //�������û�
	 			client_num++;
			}	
			// for (auto &v : udp_client)
			// {
			// 	printf("all ip %s\n", v.ip.c_str());
			// 	printf("all port %d\n", v.port);
			// 	printf("all uid %d\n", v.uid);
			// }

			//д�ļ�
			
			//�Ȼ�ȡrwlock_udp_flag_txt��  ��ȡд�������ˣ���д�пɶ�
			
			pthread_rwlock_wrlock(&rwlock_udp_flag_txt);  


			output.open(udpfilename, std::ofstream::out | std::ios::binary | std::ofstream::app);   //���������������ļ�(���|������|��λ��ĩβ)
			if (output.is_open())  //�������ļ��Ƿ��
			{
				printf("file open sucess.\n");
				output.write(udp_recv_buf,rev_len);   //д����յ�������
				output.close();
			}
			else
			{
				printf("udpfilename open failed.\n");
			}

 			udpDateUpdate = true;   //��־��λ
 			pthread_rwlock_unlock(&rwlock_udp_flag_txt);  //����


			printf("data backup success.\n");
			//��Ӧ
			udpsock.Send("ok", 3, inet_ntoa(udp_client_addr.sin_addr), ntohs(udp_client_addr.sin_port));
			//continue;
		}
		if (rev_len < 0)  //��ʱ�����
		{
			printf("udp receive timeout or error. code: %d\n", rev_len);
			//continue;
		}
		if (rev_len == 0)    //udp�Ѿ��ر�
		{
			printf("udp connect has been closed. code: %d\n", rev_len);
			//continue;
		}


		//rwlock_tcp_flag_txt  ��ȡд�������ˣ���д�пɶ�
			
		pthread_rwlock_wrlock(&rwlock_tcp_flag_txt);  
		if(tcpDateUpdate)
		{
			printf("UDP thread had finded that tcp command.\n");
			tcpDateUpdate=false;

			//���ڼ����ļ���ֻ��һ��tcp���������
			input.open(tcpfilename, std::ofstream::in | std::ofstream::binary);   //���������������ļ�(��|������)
			if (input.is_open())  		//�������ļ��Ƿ��
			{	

				//���㵱ǰ���ļ��Ĵ�С
				input.seekg(0 , std::ifstream::end);
				int filesize = input.tellg();	
				//if(filesize == -1) filesize = 0;
				printf("file total size is %d\n", filesize);
				 
				input.seekg(0 , std::ifstream::beg);  //����ָ�붨λ������
				input.read(udp_send_buf,filesize);   //��ȡ��ߴ������ĵ�
				sendsize = filesize;
				printf("read size : %d\n", filesize);

				filesize = 0;
			}  //if (input.is_open())
			else
			{
				printf("udpfilename open failed.\n");
			}
			input.close();

			ClearFile(tcpfilename);	//����ļ�

		}
		pthread_rwlock_unlock(&rwlock_tcp_flag_txt);  //����

		if(sendsize > 0)  //ȷʵ��Ҫ�´���tcp����
		{
			//�������������е�uid
			clientstruct temp_client;
			temp_client.uid[0] = udp_send_buf[0];
			temp_client.uid[1] = udp_send_buf[1];

			for(auto &v : udp_client)
			{
				if(v.uid[0]==temp_client.uid[0]&&v.uid[1]==temp_client.uid[1])  //ƥ���Ѵ�ĵ�ַ��
				{
					printf("uid match success.\n");
					temp_client.ip = v.ip;
					temp_client.port = v.port;
					uidmatchok = true;
					break;
				}
			}

			if(uidmatchok)   //uid��ƥ��ɹ�,�´�
			{
				udpsock.Send(udp_send_buf, sendsize, temp_client.ip.c_str(), temp_client.port);
				sendsize = 0;
				uidmatchok = 0;
				printf("udp send success.\n");
			}	
		}



	}
}

bool UdpThread::ClearFile(const char *filename)  //���ָ���ļ�
{
	output.open(filename, std::ofstream::out | std::ios::trunc);   
	if (output.is_open())  //�������ļ��Ƿ��
	{
		printf("clear %s success.\n",filename);
		output.close();
		return true;
	}
	else
	{
		printf("clear %s failed.\n",filename);
		return false;
	}
}
