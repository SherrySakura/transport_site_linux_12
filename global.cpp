#include <pthread.h>

pthread_rwlock_t rwlock_tcp_flag_txt;		 //��д�� ������tcpData.txt��udpData.txt��  �ȶ���д
pthread_rwlock_t rwlock_udp_flag_txt;		 //��д�� ������tcpData.txt��udpData.txt��


volatile bool tcpDateUpdate = 0;				 //udp�̼߳��ñ������������tcp�Ƿ���յ��µ�������������ж�ȡ���������͸����׮
volatile bool udpDateUpdate = 0;				 //tcp�̶߳�ʱ���ñ������������udp�Ƿ���յ��µ�������������ж�ȡ���������͸�����ˣ����û�������������������

