SYSCALL_DEFINE2(mysyscall,char* ,sourcefile, char* ,targetfile)
{
	char addr[10];
	int x,stream1,stream2;
	mm_segment_t old_fs = get_fs();
	set_fs(KERNEL_DS);
	stream1 = sys_open(sourcefile,O_RDONLY,0);
	stream2 = sys_open(targetfile,O_WRONLY|O_CREAT|O_TRUNC,0600);
	if(stream1 == -1 || stream2 == -1)
		return -1;
	do{
		x = sys_read(stream1,addr,10);
		sys_write(stream2,addr,x);
		printk("+");
	}
	while(x>0);
	sys_close(stream1);
	sys_close(stream2);
	set_fs(old_fs);
	return 1;
}
