#include "TcpThread.h"
#include <iostream>
#include <thread>
#include "global.h"
#include <cstring>

#ifdef WIN32
#else
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#endif

TcpThread::TcpThread()
{
}


TcpThread::~TcpThread()
{
}


void TcpThread::Start()   //线程开始
{
	//启动线程
	isexit = false;
	std::thread sth(&TcpThread::Main, this);	//<函数指针，调用的对象>
	sth.detach();  //释放主线程的控制权（资源）
	printf("tcp thread start.\n");
}

void TcpThread::Main()   //线程主函数
{
	//tcp socket初始化

	while (!isexit)
	{
		while (!isonline)    //连接   如果isonline == false
		{	
			tcpsock.CreateSocket();
			//int re = tcpsock.Connect("192.168.0.123", 8000, 3);    
			int re = tcpsock.Connect(5);
			if (re) 
			{
				isonline = true;	//更改连接标志
				break;
			}	
			else	
			{
				isonline = false;	//更改连接标志
				tcpsock.Close();   //关闭套接字，重连
			}
		}		


		tcpsock.SetRecvTimeout(1);  //设置tcp接收超时
		tcpsock.SetSendTimeout(2);	//设置tcp发送超时

		time(&lasttime);  //获取时间

		while (isonline)  //在线则执行
		{
			int rev_len = tcpsock.Recv(tcp_recv_buf, sizeof(tcp_recv_buf) - 1);
			if (rev_len > 0)
			{
				tcp_recv_buf[rev_len] = '\0';
				printf("%s %d\n", tcp_recv_buf, rev_len);

				//将受到的命令写入缓存  获取写锁就行了，可写有可读
				pthread_rwlock_wrlock(&rwlock_tcp_flag_txt);  

				output.open(tcpfilename, std::ofstream::out | std::ios::binary | std::ofstream::app);   //关联输出流对象和文件(输出|二进制|定位到末尾)
				if (output.is_open())  //关联的文件是否打开
				{
					printf("file open sucess.\n");
					output.write(tcp_recv_buf,rev_len);   //写入接收到的数据
					output.close();
				}
				else
				{
					printf("tcpfilename open failed.\n");
				}
	 			tcpDateUpdate=true;     //标志置位

				pthread_rwlock_unlock(&rwlock_tcp_flag_txt);  //解锁
				printf("tcp data backup success.\n");

			}
			if (rev_len == 0)
			{
				printf("tcp connect lost.\n");
				isonline = false;	//更改连接标志
				break;  //自动重连  这个得处理
			}
			if (rev_len < 0)
			{
				printf("tcp receive timeout or error. code: %d\n",rev_len);
			}


			/*每隔5秒执行一次查询、发送  */
			time(&nowtime);  			//获取当前时间
			if((nowtime - lasttime)>=5)   	
			{
				printf("=============the time diffrient: %d============== \n",nowtime - lasttime);  //执行事件
				time(&lasttime);  //更新本次时间
			
				//查询udp数据缓存是否有值，rwlock_udp_flag_txt  获取写锁就行了，可写有可读
				pthread_rwlock_wrlock(&rwlock_udp_flag_txt);  

				//if(udpDateUpdate)    //不用这个了，直接打开查看
				if(true)
				{
					printf("TCP thread had finded that udp data.\n");
					/*打开文件，计算剩余未读大小，读取缓存，确定当次读完则清空（缓存读完）*/
					input.open(udpfilename, std::ofstream::in | std::ofstream::binary);   //关联输出流对象和文件(读|二进制)
					if (input.is_open())  		//关联的文件是否打开
					{	

						//计算当前的文件的大小
						input.seekg(0 , std::ifstream::end);
						int filesize = input.tellg();	
						//if(filesize == -1) filesize = 0;
						printf("file total size is %d\n", filesize);
						 
						input.seekg(last_pos);  //将读指针定位到上次未读完的位置
						if((filesize-last_pos)>tcp_data_size_max)  //如果未读大小超过限制
						{
							printf("can't last file data by ones.\n");
							input.read(tcp_send_buf,tcp_data_size_max);   //读取大尺寸整个文档
							printf("read size : %d\n", tcp_data_size_max);
							sendsize = tcp_data_size_max;
							last_pos = input.tellg();				  //记录读指针
						}
						else
						{
							printf("read last file data by ones\n");
							int lastdatasize = filesize-last_pos;
							printf("last file size %d \n",lastdatasize);
							input.read(tcp_send_buf,lastdatasize);   //读取剩余整个文档
							printf("read size : %d\n", lastdatasize);
							sendsize = lastdatasize;
							isEnd = true;
						}
						filesize = 0;
					}  //if (input.is_open())
					else
					{
						printf("udpfilename open failed.\n");
					}
					input.close();
					if (isEnd)   //本文件已经读完
					{
						isEnd = false;
						printf("read to end =========================\n");
						ClearFile(udpfilename);	//清空文件
						last_pos = 0;			//get指针置零
					}
					udpDateUpdate=false;
				}//if(udpDateUpdate)

				pthread_rwlock_unlock(&rwlock_udp_flag_txt);  //解锁

				//发送
				if(sendsize)
				{
					int sendre = tcpsock.Send(tcp_send_buf,sendsize); 
					if(sendre>0)
					{
						printf("tcp send done-----------size : %d bytes.\n",sendre); 	
						printf("sendsize-----------size : %d bytes.\n",sendsize); 	
					}
					sendsize = 0;
				}
			} //if((nowtime - lasttime)>=5) 
		
		}
	}
}

bool TcpThread::ClearFile(const char *filename)  //清空指定文件
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
