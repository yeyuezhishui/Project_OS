#include "design1_1.h"
#include <unistd.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//读取文件大小函数
int file_size(char * filename)
{
	struct stat statbuf;
	stat(filename,&statbuf);//读取文件状态信息
	int size = statbuf.st_size;
	return size;
}


int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		printf("the num of the parameters is wrong!\n");
		return 0;
	}
	else if(strcmp(argv[1],"copy") != 0)
	{
		printf("wrong instruction!\n");
		return 0;
	}
	strcpy(sourcefile,argv[2]);
	strcpy(targetfile,argv[3]);
	//创建信号灯
	int semID;
	semID = semget(semKey,2,IPC_CREAT|0666);
	union semun arg;
	arg.val = 10;//10个缓冲区初始都为可写状态
	semctl(semID,0,SETVAL,arg);//初始化可写信号灯
	arg.val = 0;//没有一个缓冲区初始可读
	semctl(semID,1,SETVAL,arg);//初始化可读信号灯
	
	//创建共享内存
	int shID;
	shID = shmget(shKey,sizeof(shbuf),IPC_CREAT|0666);
	shbuf * Loop_BUF = (shbuf *)shmat(shID,NULL,0);
	Loop_BUF->last_block = 0;//初始标记最后一个缓冲区不存在
	int pid1 = -1,pid2 = -1;
	pid1 = fork();
	if(pid1 != 0 && pid2 != 0)
	{
		pid2 = fork();
	}
	if(pid1==0)//子进程1
	{
		//get信号灯 
	int semID;
	semID = semget(semKey,2,IPC_CREAT|0666);//0666为权限
	
	//get共享内存
	int shID;
	shID = shmget(shKey,sizeof(shbuf),IPC_CREAT|0666);
	shbuf * Loop_BUF = (shbuf *)shmat(shID,NULL,0);
	
	
	//定义文件指针
	FILE * fp;
	if((fp = fopen(sourcefile,"rb"))==NULL)
	{
		printf("Can not open file\n");
	}
	

	int total_size = file_size(sourcefile);//文件总大小
	int Buf_Num = 0;//当前缓冲区编号
	double current_size = 0;//当前已读大小
	
	//准备进度条
	char buf[102];//进度条主体
	memset(buf,'\0',sizeof(buf));
	const char* lable="|/-\\";
	int i = 0;
	while(Loop_BUF->last_block!=1)
	{
		P(semID,0);//P写操作
		Loop_BUF->len_of_block[Buf_Num] = fread(Loop_BUF->buf_block[Buf_Num],sizeof(char),100,fp);//往一个缓冲区里写
		if(Loop_BUF->len_of_block[Buf_Num]<100)//当一个缓冲区没有被写满
		{
			
			Loop_BUF->last_block = 1;//标记遇为最后一个缓冲区出现
		}
		current_size += Loop_BUF->len_of_block[Buf_Num];
		printf("\033[?25l");
		
		//来做一个进度条
		printf("[%-100s][%.0f%%][%c]\r",buf,current_size/total_size*100,lable[i%4]);
		i++;
		int j = 0;
		for(j = 0;j<=(current_size*100/total_size);j++)
		{
			buf[j] = '=';
		}
		Buf_Num = (Buf_Num+1)%10;//环形读取下一个缓冲区
		V(semID,1);//V读操作
	}
	fclose(fp);
	exit(0);
	}
	else if(pid2==0)//子进程2
	{
		//获取信号灯
	int semID;
	semID = semget(semKey,2,IPC_CREAT|0666);
	
	//获取环形缓冲
	int shID;
	shID = shmget(shKey,sizeof(shbuf),IPC_CREAT|0666);
	shbuf * Loop_BUF = (shbuf *)shmat(shID,NULL,0);
	
	//打开要写入的文件
	FILE *fp2;
	if((fp2 = fopen(targetfile,"wb"))==NULL)
	{
		printf("Can not open the file\n");
	}
	
	int Buf_Num_2 = 0;
	while(Loop_BUF->last_block!=1 || Loop_BUF->len_of_block[Buf_Num_2]==100)
	{
		P(semID,1);//P读
		fwrite(Loop_BUF->buf_block[Buf_Num_2],sizeof(char),Loop_BUF->len_of_block[Buf_Num_2],fp2);//每次读取一个缓冲区的内容
		Buf_Num_2 = (Buf_Num_2+1)%10;//循环累加
		V(semID,0);//V写
	}
	if(Loop_BUF->len_of_block[Buf_Num_2]!=0)//当标记为最后一块的缓冲区内还有可读内容
	{
		fwrite(Loop_BUF->buf_block[Buf_Num_2],sizeof(char),Loop_BUF->len_of_block[Buf_Num_2],fp2);//读取最后一块缓冲区
	}
	fclose(fp2);
	exit(0);
	}
	else//主进程
	{
		wait(&pid1);
		wait(&pid2);
		printf("\n succeed!\n");
		//删除信号灯
		
		semctl(semID,1,IPC_RMID,0);
		//释放共享内存
		shmctl(shID,IPC_RMID,0);
	}
	return 0;
}
