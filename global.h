//����ȫ�ֱ���������������̼�ͨ��
#ifndef GLOBAL_H
#define GLOBAL_H

extern pthread_rwlock_t rwlock_tcp_flag_txt;		 //��д�� ������tcpData.txt��udpData.txt��  �ȶ���д
extern pthread_rwlock_t rwlock_udp_flag_txt;		 //��д�� ������tcpData.txt��udpData.txt��


extern volatile bool tcpDateUpdate;				 //udp�̼߳��ñ������������tcp�Ƿ���յ��µ�������������ж�ȡ���������͸����׮
extern volatile bool udpDateUpdate;				 //tcp�̶߳�ʱ���ñ������������udp�Ƿ���յ��µ�������������ж�ȡ���������͸�����ˣ����û�������������������


#endif
