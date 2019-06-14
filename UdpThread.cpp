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
//实现string到unsigned int的转换  
unsigned int string_to_unsigned_int(std::string str)  
{  
    unsigned int result(0);//最大可表示值为4294967296（=2‘32-1）  
    //从字符串首位读取到末位（下标由0到str.size() - 1）  
    for (int i = str.size()-1;i >= 0;i--)  
    {  
        unsigned int temp(0),k = str.size() - i - 1;  
        //判断是否为数字  
        if (isdigit(str[i]))  
        {  
            //求出数字与零相对位置  
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

void UdpThread::Start()   //线程开始
{
	//启动线程
	isexit = false;
	std::thread sth(&UdpThread::Main, this);	//<函数指针，调用的对象>
	sth.detach();  //释放主线程的控制权（资源）
	printf("udp thread start.\n");
}
//udp_client
void UdpThread::Main()   //线程主函数
{
	//UDP初始化
	udpsock.CreateSocket();
	udpsock.Bind();			 //绑定端口号
	udpsock.SetRecvTimeout(1);

	int length = sizeof(udp_client_addr);
	while (!isexit)
	{
		int rev_len = udpsock.Recv(udp_recv_buf, sizeof(udp_recv_buf), &udp_client_addr);	 //接收数据
		if (rev_len > 0)  //接收成功没返回接收到的字节数
		{
			recv_num++;
			isUnique = true;
			printf("recv_num:%d\n", recv_num);
			printf("udp receive success. length:%d\n", rev_len);
			//获取客户端ip
			printf("udp client's ip is %s  port: %d\n", inet_ntoa(udp_client_addr.sin_addr), ntohs(udp_client_addr.sin_port));

			//缓存接收信息   
			udp_recv_buf[rev_len] = '\0';  //在接受字符串的尾部强加'\0'
			printf("udp_recv_buf: %s\n", udp_recv_buf);

			clientstruct udp_newclient; //缓存新client
			if (client_num>0)
			{
				for (auto &v : udp_client)  //范围遍历udp_client中的每一个元素
				{
					if (std::string(inet_ntoa(udp_client_addr.sin_addr)) == v.ip)  //只要ip相同
					{
						isUnique = false;  //放过这条ip记录
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

				udp_client.push_back(udp_newclient);  //保存新用户
	 			client_num++;
			}	
			// for (auto &v : udp_client)
			// {
			// 	printf("all ip %s\n", v.ip.c_str());
			// 	printf("all port %d\n", v.port);
			// 	printf("all uid %d\n", v.uid);
			// }

			//写文件
			
			//先获取rwlock_udp_flag_txt锁  获取写锁就行了，可写有可读
			
			pthread_rwlock_wrlock(&rwlock_udp_flag_txt);  


			output.open(udpfilename, std::ofstream::out | std::ios::binary | std::ofstream::app);   //关联输出流对象和文件(输出|二进制|定位到末尾)
			if (output.is_open())  //关联的文件是否打开
			{
				printf("file open sucess.\n");
				output.write(udp_recv_buf,rev_len);   //写入接收到的数据
				output.close();
			}
			else
			{
				printf("udpfilename open failed.\n");
			}

 			udpDateUpdate = true;   //标志置位
 			pthread_rwlock_unlock(&rwlock_udp_flag_txt);  //解锁


			printf("data backup success.\n");
			//回应
			udpsock.Send("ok", 3, inet_ntoa(udp_client_addr.sin_addr), ntohs(udp_client_addr.sin_port));
			//continue;
		}
		if (rev_len < 0)  //超时或错误
		{
			printf("udp receive timeout or error. code: %d\n", rev_len);
			//continue;
		}
		if (rev_len == 0)    //udp已经关闭
		{
			printf("udp connect has been closed. code: %d\n", rev_len);
			//continue;
		}


		//rwlock_tcp_flag_txt  获取写锁就行了，可写有可读
			
		pthread_rwlock_wrlock(&rwlock_tcp_flag_txt);  
		if(tcpDateUpdate)
		{
			printf("UDP thread had finded that tcp command.\n");
			tcpDateUpdate=false;

			//现在假设文件中只有一条tcp服务端命令
			input.open(tcpfilename, std::ofstream::in | std::ofstream::binary);   //关联输出流对象和文件(读|二进制)
			if (input.is_open())  		//关联的文件是否打开
			{	

				//计算当前的文件的大小
				input.seekg(0 , std::ifstream::end);
				int filesize = input.tellg();	
				//if(filesize == -1) filesize = 0;
				printf("file total size is %d\n", filesize);
				 
				input.seekg(0 , std::ifstream::beg);  //将读指针定位到顶部
				input.read(udp_send_buf,filesize);   //读取大尺寸整个文档
				sendsize = filesize;
				printf("read size : %d\n", filesize);

				filesize = 0;
			}  //if (input.is_open())
			else
			{
				printf("udpfilename open failed.\n");
			}
			input.close();

			ClearFile(tcpfilename);	//清空文件

		}
		pthread_rwlock_unlock(&rwlock_tcp_flag_txt);  //解锁

		if(sendsize > 0)  //确实有要下传的tcp命令
		{
			//解析出命令行中的uid
			clientstruct temp_client;
			temp_client.uid[0] = udp_send_buf[0];
			temp_client.uid[1] = udp_send_buf[1];

			for(auto &v : udp_client)
			{
				if(v.uid[0]==temp_client.uid[0]&&v.uid[1]==temp_client.uid[1])  //匹配已存的地址表
				{
					printf("uid match success.\n");
					temp_client.ip = v.ip;
					temp_client.port = v.port;
					uidmatchok = true;
					break;
				}
			}

			if(uidmatchok)   //uid已匹配成功,下传
			{
				udpsock.Send(udp_send_buf, sendsize, temp_client.ip.c_str(), temp_client.port);
				sendsize = 0;
				uidmatchok = 0;
				printf("udp send success.\n");
			}	
		}



	}
}

bool UdpThread::ClearFile(const char *filename)  //清空指定文件
{
	output.open(filename, std::ofstream::out | std::ios::trunc);   
	if (output.is_open())  //关联的文件是否打开
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
