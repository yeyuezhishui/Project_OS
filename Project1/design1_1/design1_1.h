#ifndef EX3_HEADER
#define EX3_HEADER

#include <pthread.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/stat.h>

//定义p操作
void P(int semid,int index)
{
	struct sembuf sem;
	sem.sem_num  = index;
	sem.sem_op = -1;
	sem.sem_flg = 0;
	semop(semid,&sem,1);
	return;
}

//定义V操作
void V(int semid,int index)
{
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op = 1;
	sem.sem_flg = 0;
	semop(semid,&sem,1);
	return;
}

union semun
{
	int val;
	struct semid_ds *buf;
 	unsigned short int *array;
 	struct seminfo *__buf;
};


//定义环形缓冲结构体
typedef struct ShBuf
{
	char buf_block[10][100];//10个缓冲区 每个100字节
	int len_of_block[10];//记录每个缓冲区当前容纳字节大小
	int last_block;//标记是否出现最后一个缓冲区
}shbuf;

const key_t semKey = 1;//生成信号灯ID
const key_t shKey = 0x1234;//生成shmID
char  sourcefile[100];
char  targetfile[100];

#endif

