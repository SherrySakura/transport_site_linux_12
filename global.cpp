#include <pthread.h>

pthread_rwlock_t rwlock_tcp_flag_txt;		 //读写锁 （保护tcpData.txt和udpData.txt）  先读后写
pthread_rwlock_t rwlock_udp_flag_txt;		 //读写锁 （保护tcpData.txt和udpData.txt）


volatile bool tcpDateUpdate = 0;				 //udp线程检测该变量，用来检查tcp是否接收到新的命令，如果有则进行读取并解析发送给充电桩
volatile bool udpDateUpdate = 0;				 //tcp线程定时检测该变量，用来检查udp是否接收到新的命令，如果有则进行读取并解析发送给服务端，如果没有则发送心跳包给服务端

