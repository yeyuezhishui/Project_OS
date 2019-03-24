#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
	long x;
	x = syscall(333,"/home/yyzs/os_design/2/t.txt","/home/yyzs/os_design/2/t5.txt");
	if(x==1)
	printf("succeed!\n");
	else if(x==-1)
	printf("failed!\n");
	return 0;
}
