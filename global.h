//定义全局变量，用来处理进程间通信
#ifndef GLOBAL_H
#define GLOBAL_H

extern pthread_rwlock_t rwlock_tcp_flag_txt;		 //读写锁 （保护tcpData.txt和udpData.txt）  先读后写
extern pthread_rwlock_t rwlock_udp_flag_txt;		 //读写锁 （保护tcpData.txt和udpData.txt）


extern volatile bool tcpDateUpdate;				 //udp线程检测该变量，用来检查tcp是否接收到新的命令，如果有则进行读取并解析发送给充电桩
extern volatile bool udpDateUpdate;				 //tcp线程定时检测该变量，用来检查udp是否接收到新的命令，如果有则进行读取并解析发送给服务端，如果没有则发送心跳包给服务端


#endif
