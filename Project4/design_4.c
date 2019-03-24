#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/vfs.h>
#include <sys/time.h>
#include <dirent.h>

typedef struct Info
{
    int pid;
    char name[100];
    char state[1];
    int priority;
    int ram;
    int disk;
    int net;
    //int flag = 0;
}info;

info pro_info[1000];
int n_info  =0;
int p=0,q=0,r=0,s=0;
static float cpu_used_percent=0;	/*cpu使用率*/
static int cpu_start_position=15;	/*绘制cpu移动的线条*/
static float cpu_data[66];	/*cpu历史数据*/
static int flag2=0;		/*初始化cpu_data数组中数据的标志*/
static int cpu_first_data=0;	/*第一个数据，既最早的数据，下一个要淘汰的数据*/
static long idle,total;		/*计算cpu时的数据*/
static int flag=0;		/*计算cpu使用率时启动程序的标志*/
static int flag1=0;		/*计算单个进程cpu使用率时使用的标志*/
static char temp_cpu[50];	/*cpu使用率*/
static char temp_mem[50];	/*内存使用*/
static char temp_process[50];	/*进程数*/
static long mem_total;		/*内存总大小*/
static long mem_free;		/*空闲内存*/
static long long ustime[32768];	/*前一次记录的用户态和核心态的总时间*/
static long mtime[32768];	/*前一次记录的时刻*/
static float mem_data[66];	/*cpu历史数据*/
static int flag3=0;		/*初始化cpu_data数组中数据的标志*/
static int mem_first_data=0;	/*第一个数据，既最早的数据，下一个要淘汰的数据*/
static int mem_start_position=15;	/*绘制内存移动的线条*/
char *txt_pid = NULL;
char temp_disc[1024];
char temp_nett[1024];

void select_row_callback(GtkWidget *clist,gint row,gint column, GdkEventButton *event, gpointer data)
{
    gtk_clist_get_text(GTK_CLIST(clist),row,column,&txt_pid);
    printf("%s\n",txt_pid);
}



void start(GtkWidget* clist,gint co)
{
    int i = 0;
    int j = 0;
    int n;
    char text[7][100];
    gchar *txt[7];
    n = n_info;
    switch(co)
    {
        case 0:
            for(i = 0;i<n-1;i++)
            {
                for(j = 0;j<n-i-1;j++)
                {
                    if(pro_info[j].pid<pro_info[j+1].pid)
                    {
                        int temp_pid;
                        temp_pid=pro_info[j].pid;
                        pro_info[j].pid=pro_info[j+1].pid;
                        pro_info[j+1].pid=temp_pid;
                    }
                }
            }
            for(i = n-1;i>=0;i--)
            {
                sprintf(text[0],"%d",pro_info[i].pid);
                sprintf(text[1],"%s",pro_info[i].name);
                sprintf(text[2],"%s",pro_info[i].state);
                sprintf(text[3],"%d",pro_info[i].priority);
                sprintf(text[4],"%d",pro_info[i].ram);
                sprintf(text[5],"%d",pro_info[i].disk);
                sprintf(text[6],"%d",pro_info[i].net);
                txt[5]=text[5];
                txt[6]=text[6];
			    txt[0]=text[0];
			    txt[1]=text[1];
			    txt[2]=text[2];
			    txt[3]=text[3];
			    txt[4]=text[4];
                gtk_clist_append(GTK_CLIST(clist),txt);
            }
            break;
        case 1:
            for(i = 0;i<n-1;i++)
            {
                for(j = 0;j<n-i-1;j++)
                {
                    if(strcmp(pro_info[j].name,pro_info[j+1].name)<0)
                    {
                        char temp_name[100];
                        strcpy(temp_name,pro_info[j].name);
                        strcpy(pro_info[j].name,pro_info[j+1].name);
                        strcpy(pro_info[j+1].name,temp_name);
                    }
                }
            }
            for(i = n-1;i>=0;i--)
            {
                sprintf(text[0],"%d",pro_info[i].pid);
                sprintf(text[1],"%s",pro_info[i].name);
                sprintf(text[2],"%s",pro_info[i].state);
                sprintf(text[3],"%d",pro_info[i].priority);
                sprintf(text[4],"%d",pro_info[i].ram);
                sprintf(text[5],"%d",pro_info[i].disk);
                sprintf(text[6],"%d",pro_info[i].net);
                txt[5]=text[5];
                txt[6]=text[6];
			    txt[0]=text[0];
			    txt[1]=text[1];
			    txt[2]=text[2];
			    txt[3]=text[3];
			    txt[4]=text[4];
                gtk_clist_append(GTK_CLIST(clist),txt);
            }
            break;
        case 2:
            for(i = 0;i<n-1;i++)
            {
                for(j = 0;j<n-i-1;j++)
                {
                    if(strcmp(pro_info[j].state,pro_info[j+1].state)>0)
                    {
                        char temp_state[100];
                        strcpy(temp_state,pro_info[j].state);
                        strcpy(pro_info[j].state,pro_info[j+1].state);
                        strcpy(pro_info[j+1].state,temp_state);
                    }
                }
            }
            for(i = n-1;i>=0;i--)
            {
                sprintf(text[0],"%d",pro_info[i].pid);
                sprintf(text[1],"%s",pro_info[i].name);
                sprintf(text[2],"%s",pro_info[i].state);
                sprintf(text[3],"%d",pro_info[i].priority);
                sprintf(text[4],"%d",pro_info[i].ram);
                sprintf(text[5],"%d",pro_info[i].disk);
                sprintf(text[6],"%d",pro_info[i].net);
                txt[5]=text[5];
                txt[6]=text[6];
			    txt[0]=text[0];
			    txt[1]=text[1];
			    txt[2]=text[2];
			    txt[3]=text[3];
			    txt[4]=text[4];
                gtk_clist_append(GTK_CLIST(clist),txt);
            }
            break;
        case 3:
            for(i = 0;i<n-1;i++)
            {
                for(j = 0;j<n-i-1;j++)
                {
                    if(pro_info[j].priority>pro_info[j+1].priority)
                    {
                        int temp_pri;
                        temp_pri=pro_info[j].priority;
                        pro_info[j].priority=pro_info[j+1].priority;
                        pro_info[j+1].priority=temp_pri;
                    }
                }
            }
            for(i = 0;i<n;i++)
            {
                sprintf(text[0],"%d",pro_info[i].pid);
                sprintf(text[1],"%s",pro_info[i].name);
                sprintf(text[2],"%s",pro_info[i].state);
                sprintf(text[3],"%d",pro_info[i].priority);
                sprintf(text[4],"%d",pro_info[i].ram);
                sprintf(text[5],"%d",pro_info[i].disk);
                sprintf(text[6],"%d",pro_info[i].net);
                txt[5]=text[5];
                txt[6]=text[6];
			    txt[0]=text[0];
			    txt[1]=text[1];
			    txt[2]=text[2];
			    txt[3]=text[3];
			    txt[4]=text[4];
                gtk_clist_append(GTK_CLIST(clist),txt);
            }
            break;
        case 4:
            for(i = 0;i<n-1;i++)
            {
                for(j = 0;j<n-i-1;j++)
                {
                    if(pro_info[j].ram<pro_info[j+1].ram)
                    {
                        int temp_ram;
                        temp_ram=pro_info[j].ram;
                        pro_info[j].ram=pro_info[j+1].ram;
                        pro_info[j+1].ram=temp_ram;
                    }
                }
            }
            for(i = 0;i<n;i++)
            {
                sprintf(text[0],"%d",pro_info[i].pid);
                sprintf(text[1],"%s",pro_info[i].name);
                sprintf(text[2],"%s",pro_info[i].state);
                sprintf(text[3],"%d",pro_info[i].priority);
                sprintf(text[4],"%d",pro_info[i].ram);
                sprintf(text[5],"%d",pro_info[i].disk);
                sprintf(text[6],"%d",pro_info[i].net);
                txt[5]=text[5];
                txt[6]=text[6];
			    txt[0]=text[0];
			    txt[1]=text[1];
			    txt[2]=text[2];
			    txt[3]=text[3];
			    txt[4]=text[4];
                gtk_clist_append(GTK_CLIST(clist),txt);
            }
            break;
        case 5:
            for(i = 0;i<n-1;i++)
            {
                for(j = 0;j<n-i-1;j++)
                {
                    if(pro_info[j].disk<pro_info[j+1].disk)
                    {
                        int temp_disk;
                        temp_disk=pro_info[j].disk;
                        pro_info[j].disk=pro_info[j+1].disk;
                        pro_info[j+1].disk=temp_disk;
                    }
                }
            }
            for(i = 0;i<n;i++)
            {
                sprintf(text[0],"%d",pro_info[i].pid);
                sprintf(text[1],"%s",pro_info[i].name);
                sprintf(text[2],"%s",pro_info[i].state);
                sprintf(text[3],"%d",pro_info[i].priority);
                sprintf(text[4],"%d",pro_info[i].ram);
                sprintf(text[5],"%d",pro_info[i].disk);
                sprintf(text[6],"%d",pro_info[i].net);
                txt[5]=text[5];
                txt[6]=text[6];
			    txt[0]=text[0];
			    txt[1]=text[1];
			    txt[2]=text[2];
			    txt[3]=text[3];
			    txt[4]=text[4];
                gtk_clist_append(GTK_CLIST(clist),txt);
            }
            break;
        case 6:
            for(i = 0;i<n-1;i++)
            {
                for(j = 0;j<n-i-1;j++)
                {
                    if(pro_info[j].net<pro_info[j+1].net)
                    {
                        int temp_net;
                        temp_net=pro_info[j].net;
                        pro_info[j].net=pro_info[j+1].net;
                        pro_info[j+1].net=temp_net;
                    }
                }
            }
            for(i = 0;i<n;i++)
            {
                sprintf(text[0],"%d",pro_info[i].pid);
                sprintf(text[1],"%s",pro_info[i].name);
                sprintf(text[2],"%s",pro_info[i].state);
                sprintf(text[3],"%d",pro_info[i].priority);
                sprintf(text[4],"%d",pro_info[i].ram);
                sprintf(text[5],"%d",pro_info[i].disk);
                sprintf(text[6],"%d",pro_info[i].net);
                txt[5]=text[5];
                txt[6]=text[6];
			    txt[0]=text[0];
			    txt[1]=text[1];
			    txt[2]=text[2];
			    txt[3]=text[3];
			    txt[4]=text[4];
                gtk_clist_append(GTK_CLIST(clist),txt);
            }
            break;
        default: break;
    }
} 

void select_column_callback(GtkWidget *clist, gint column)
{
    gtk_clist_clear(GTK_CLIST(clist));
    start(clist, column);
}

gint delete_event( GtkWidget *widget, GdkEvent *event, gpointer data )
{
    gtk_main_quit ();
    return FALSE;
}

void kill_proc(void)
{
    char buf[20];
	sprintf(buf,"kill -9 %s",txt_pid);
    system(buf);
}

void refresh(GtkWidget *clist)
{
    p = 0;
    q = 0;
    r = 0;
    s = 0;
        DIR *dir;
        struct dirent *ptr;
        int i,j;
        FILE *fp;
        char buf[1024];
        
        char bufy[1024];
        
        char _buffer[1024];
        char _buffery[1024];
        
        char *buffery = _buffery;
        char *buffer=_buffer;
        char *buffer2;
        char proc_pid[1024];
        char proc_name[1024];
        char proc_stat[1024];
        char proc_pri[1024];
        char proc_takeup[1024];
        char disk_1[1024];
        char disk_2[1024];
        char net[1024];
        char text[7][1024];
        gchar *txt[7];
        int y = 0;
	gtk_clist_clear(GTK_CLIST(clist));
    
    dir=opendir("/proc");

        while(ptr=readdir(dir)){
                if((ptr->d_name)[0]>=48&&(ptr->d_name)[0]<=57){
                        p++;
                        sprintf(buf,"/proc/%s/stat",ptr->d_name);
				        sprintf(bufy,"/proc/%s/net/dev",ptr->d_name);
				        
				        
				        
				        FILE * fpy = fopen(bufy,"r");
				        int i3;
				        for(i3 = 0; i3<3;i3++)
				        {
				            fgets(buffery,1024,fpy);
				        }
				        fclose(fpy);
				        int j3 = 0,j4 = 0;
				        for(j3 = 0,j4 = 0;j4 != 2;j3++)
				        {
				            while(buffery[j3]==' ')
				            {
				                j3++;
				            }
				            j4+=1;
				            if(j4==2)
				            {
				                break;
				            }
				            while(buffery[j3]!=' ')
				            {
				                j3++;
				            }
				        }
				        for(j4=0;buffery[j3]!=' ';j3++)
				        {
				            net[j4]=buffery[j3];
				            j4+=1;
				        }
				        net[j4] = '\0';
				        
				        
				        
                	    fp=fopen(buf,"r");
                        fgets(buffer,1024,fp);
                        fclose(fp);
                        int i1,j1;
                        for(i1=0,j1=0;i1<1024&&j1<9;i1++)
                        {
                            if(buffer[i1]==' ') j1++;
                        }
                        i1++;
                        for(j1=0;;i1++)
                        {
                            if(buffer[i1]==' ')
                            {
                                disk_1[j1]='\0';
                                break;
                            }
                            else disk_1[j1] = buffer[i1];
                            j1++;
                        }
                        
                        
                        for(i1=0,j1=0;i1<1024&&j1<11;i1++)
                        {
                            if(buffer[i1]==' ') j1++;
                        }
                        i1++;
                        for(j1=0;;i1++)
                        {
                            if(buffer[i1]==' ')
                            {
                                disk_1[j1]='\0';
                                break;
                            }
                            else disk_2[j1] = buffer[i1];
                            j1++;
                        }                        
                        
                        for(i=0;i<1024;i++){
                        	if(buffer[i]==' ') break;
                        }
                        buffer[i]='\0';
                        strcpy(proc_pid,buffer);
                        i+=2;
                        buffer+=i;
                        for(i=0;i<1024;i++){
                        	if(buffer[i]==')') break;
                        }
                        buffer[i]='\0';
                        strcpy(proc_name,buffer);
                        i+=2;
                        buffer2=buffer+i;
                        buffer2[1]='\0';
                        strcpy(proc_stat,buffer2);
                        for(i=0,j=0;i<1024&&j<15;i++){
                        	if(buffer2[i]==' ') j++;
                        }
                        buffer2+=i;
                        for(i=0;i<1024;i++){
                        	if(buffer2[i]==' ') break;
                        }
                        buffer2[i]='\0';
                        strcpy(proc_pri,buffer2);
                        for(j=0;i<1024&&j<4;i++){
                        	if(buffer2[i]==' ') j++;
                        }
                        buffer2+=i;
                        for(i=0;i<1024;i++){
                        	if(buffer2[i]==' ') break;
                        }
                        buffer2[i]='\0';
                        strcpy(proc_takeup,buffer2);
                        
                        if(!strcmp(proc_stat,"R")) q++;
                        if(!strcmp(proc_stat,"S")) r++;
			            if(!strcmp(proc_stat,"Z")) s++;
                                
                        sprintf(text[0],"%s",proc_pid);
                        pro_info[y].pid = atoi(proc_pid);
                        sprintf(text[1],"%s",proc_name);
                        sprintf(pro_info[y].name,"%s",proc_name);
                        sprintf(text[2],"%s",proc_stat);
                        sprintf(pro_info[y].state,"%s",proc_stat);
                        sprintf(text[3],"%s",proc_pri);
                        pro_info[y].priority = atoi(proc_pri);

			            sprintf(text[4],"%s",proc_takeup);
			            pro_info[y].ram = atoi(proc_takeup);
			            int disk = atoi(disk_1)+atoi(disk_2);
			            pro_info[y].disk = disk;
			            sprintf(text[5],"%d",disk);
			            sprintf(text[6],"%s",net);
			            pro_info[y].net = atoi(net);
			            y+=1;

			            txt[0]=text[0];
		            	txt[1]=text[1];
			            txt[2]=text[2];
			            txt[3]=text[3];
		            	txt[4]=text[4];
		            	txt[5]=text[5];
		            	txt[6]=text[6];
		            

                        gtk_clist_append(GTK_CLIST(clist),txt);
                }
                n_info = y;
        } 
        closedir(dir);
}

void refresh_lable(GtkWidget *label)
{
    char bufff[1000]; 
        sprintf(bufff,"All process：%d\n\nRunning：%d\n\nSleeping：%d\n\nZombied：%d\n",p,q,r,s);
        gtk_label_set_text(GTK_LABEL(label),bufff);	
	    gtk_widget_show(label);
}

char *get_cpu_name(char *_buf1)
{

	FILE * fp;
	int i=0;
	char *buf1=_buf1;

    	fp=fopen("/proc/cpuinfo","r");
	for(i=0;i<5;i++){
   	fgets(buf1,256,fp);
	}
	for(i=0;i<256;i++){
   	if(buf1[i]==':') break;
	}
	i+=2;
	buf1+=i;
	buf1[31]='\0';
	fclose(fp);
   	return buf1;
}

char *get_cpu_type(char *_buf2)
{

	FILE * fp;
	int i=0;
	char *buf2=_buf2;

    	fp=fopen("/proc/cpuinfo","r");
	for(i=0;i<2;i++){
   	fgets(buf2,256,fp);
	}
	for(i=0;i<256;i++){
   	if(buf2[i]==':') break;
	}
	i+=2;
	buf2+=i;
	buf2[12]='\0';
	fclose(fp);
   	return buf2;
}

char *get_cpu_f(char *_buf3)
{

	FILE * fp;
	int i=0;
	char *buf3=_buf3;

    	fp=fopen("/proc/cpuinfo","r");
	for(i=0;i<8;i++){
   	fgets(buf3,256,fp);
	}
	for(i=0;i<256;i++){
   	if(buf3[i]==':') break;
	}
	i+=2;
	buf3+=i;
	buf3[8]='\0';
	fclose(fp);
   	return buf3;
}

char *get_cache_size(char *_buf4)
{

	FILE * fp;
	int i=0;
	char *buf4=_buf4;

    fp=fopen("/proc/cpuinfo","r");
	for(i=0;i<9;i++){
   	fgets(buf4,256,fp);
	}
	for(i=0;i<256;i++){
   	if(buf4[i]==':') break;
	}
	i+=2;
	buf4+=i;
	buf4[10]='\0';
	fclose(fp);
   	return buf4;
}

char *get_system_type(char *_buf1)
{
	FILE * fp;
        int i=0;
	char *buf1=_buf1;

        //fp=fopen("/proc/version","r");
        fp=fopen("/etc/issue","r");
        fgets(buf1,256,fp);
        for(i=0;i<256;i++){
   	if(buf1[i]=='\\') break;
	}
        buf1[i]='\0';
        fclose(fp);
        return buf1;
}

char *get_system_version(char *_buf2)
{
	FILE * fp;
        int i=0;
        int j=0;
	char *buf2=_buf2;

        fp=fopen("/proc/version","r");
        fgets(buf2,256,fp);
        for(i=0,j=0;i<256&&j<2;i++){
   	if(buf2[i]==' ') j++;
	}
        buf2+=i;
        for(i=0;i<256;i++){
   	if(buf2[i]==')') break;
	}
        buf2[i+1]='\0';
        fclose(fp);
        return buf2;
}

char *get_gcc_version(char *_buf3)
{
	FILE * fp;
        int i=0;
        int j=0;
	char *buf3=_buf3;

        fp=fopen("/proc/version","r");
        fgets(buf3,256,fp);
        for(i=0,j=0;i<256&&j<6;i++){
   	if(buf3[i]==' ') j++;
	}
        buf3+=i;
        for(i=0;i<256;i++){
   	if(buf3[i]==')') break;
	}
        buf3[i+1]='\0';
        fclose(fp);
        return buf3;
}

gint cpu_record_draw(GtkWidget *widget)/*cpu使用记录绘图函数*/
{
	int i;
	int my_first_data;
	GdkColor color;
	GdkDrawable *canvas;
	GdkGC *gc;
	GdkFont *font;
	canvas = widget->window; 
	gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];
 
	gdk_draw_rectangle(canvas, gc, TRUE, 0, 5, 460, 105);
	color.red = 0;
	color.green = 20000;
	color.blue = 0;
	gdk_gc_set_rgb_fg_color(gc, &color);	
	for(i=20;i<110;i+=15)	/*绘制横线*/
	{
		gdk_draw_line(canvas, gc, 10, i, 450, i);
	}

	for(i=10;i<450;i+=15)
	{
		gdk_draw_line(canvas, gc, i+cpu_start_position,5, i+cpu_start_position,110);			
	}
	cpu_start_position-=3;
	if(cpu_start_position==0)  cpu_start_position=15;

	if(flag2==0)	/*第一次清空数据*/
	{
		for(i=0;i<66;i++)
			cpu_data[i]=0;
		flag2=1;
		cpu_first_data=0;
	}

	cpu_data[cpu_first_data]=cpu_used_percent/100;
	cpu_first_data++;	
	if(cpu_first_data==66) cpu_first_data=0;

	color.red = 0;
	color.green = 65535;
	color.blue = 0;
	gdk_gc_set_rgb_fg_color(gc, &color);
	
	my_first_data=cpu_first_data;
	for(i=0;i<65;i++)
	{
		gdk_draw_line(canvas,gc,i*7,110-104*cpu_data[my_first_data%66],(i+1)*7,110-104*cpu_data[(my_first_data+1)%66]);
		my_first_data++;
		if(my_first_data==66)	my_first_data=0;
	}
 	color.red = 0;
	color.green = 0;
	color.blue = 0;
	gdk_gc_set_rgb_fg_color(gc, &color);
	return TRUE;
}

void get_proc_info(GtkWidget *clist,int *p,int *q,int *r,int *s)
{
        DIR *dir;
        struct dirent *ptr;
        int i,j;
        FILE *fp;
        char buf[1024];
        
        char bufy[1024];
        
        char _buffer[1024];
        char _buffery[1024];
        
        char *buffery = _buffery;
        char *buffer=_buffer;
        char *buffer2;
        char proc_pid[1024];
        char proc_name[1024];
        char proc_stat[1024];
        char proc_pri[1024];
        char proc_takeup[1024];
        char disk_1[1024];
        char disk_2[1024];
        char net[1024];
        char text[7][1024];
        gchar *txt[7];
        int y = 0;
        gtk_clist_set_column_title(GTK_CLIST(clist),0,"PID");
        gtk_clist_set_column_title(GTK_CLIST(clist),1,"NAME");
	    gtk_clist_set_column_title(GTK_CLIST(clist),2,"STATE");
	    gtk_clist_set_column_title(GTK_CLIST(clist),3,"PRIORITY");
	    gtk_clist_set_column_title(GTK_CLIST(clist),4,"RAM USAGE");
	    gtk_clist_set_column_title(GTK_CLIST(clist),5,"DISK");
	    gtk_clist_set_column_title(GTK_CLIST(clist),6,"NET");
        gtk_clist_set_column_width(GTK_CLIST(clist),0,50);
        gtk_clist_set_column_width(GTK_CLIST(clist),1,100);
        gtk_clist_set_column_width(GTK_CLIST(clist),2,50);
        gtk_clist_set_column_width(GTK_CLIST(clist),3,50);
        gtk_clist_set_column_width(GTK_CLIST(clist),4,55);
        gtk_clist_set_column_width(GTK_CLIST(clist),6,55);
        
        gtk_clist_column_titles_show(GTK_CLIST(clist)); 
        dir=opendir("/proc");

        while(ptr=readdir(dir)){
                if((ptr->d_name)[0]>=48&&(ptr->d_name)[0]<=57){
                        (*p)++;
                        sprintf(buf,"/proc/%s/stat",ptr->d_name);
				        sprintf(bufy,"/proc/%s/net/dev",ptr->d_name);
				        
				        
				        
				        FILE * fpy = fopen(bufy,"r");
				        int i3;
				        for(i3 = 0; i3<3;i3++)
				        {
				            fgets(buffery,1024,fpy);
				        }
				        fclose(fpy);
				        int j3 = 0,j4 = 0;
				        for(j3 = 0,j4 = 0;j4 != 2;j3++)
				        {
				            while(buffery[j3]==' ')
				            {
				                j3++;
				            }
				            j4+=1;
				            if(j4 == 2)
				            {
				                break;
				            }
				            while(buffery[j3]!=' ')
				            {
				                j3++;
				            }
				        }
				        for(j4=0;buffery[j3]!=' ';j3++)
				        {
				            net[j4]=buffery[j3];
				            j4+=1;
				        }
				        net[j4] = '\0';
				        
				        
				        
                	    fp=fopen(buf,"r");
                        fgets(buffer,1024,fp);
                        fclose(fp);
                        int i1,j1;
                        for(i1=0,j1=0;i1<1024&&j1<9;i1++)
                        {
                            if(buffer[i1]==' ') j1++;
                        }
                        i1++;
                        for(j1=0;;i1++)
                        {
                            if(buffer[i1]==' ')
                            {
                                disk_1[j1]='\0';
                                break;
                            }
                            else disk_1[j1] = buffer[i1];
                            j1++;
                        }
                        
                        
                        for(i1=0,j1=0;i1<1024&&j1<11;i1++)
                        {
                            if(buffer[i1]==' ') j1++;
                        }
                        i1++;
                        for(j1=0;;i1++)
                        {
                            if(buffer[i1]==' ')
                            {
                                disk_1[j1]='\0';
                                break;
                            }
                            else disk_2[j1] = buffer[i1];
                            j1++;
                        }                        
                        
                        for(i=0;i<1024;i++){
                        	if(buffer[i]==' ') break;
                        }
                        buffer[i]='\0';
                        strcpy(proc_pid,buffer);
                        i+=2;
                        buffer+=i;
                        for(i=0;i<1024;i++){
                        	if(buffer[i]==')') break;
                        }
                        buffer[i]='\0';
                        strcpy(proc_name,buffer);
                        i+=2;
                        buffer2=buffer+i;
                        buffer2[1]='\0';
                        strcpy(proc_stat,buffer2);
                        for(i=0,j=0;i<1024&&j<15;i++){
                        	if(buffer2[i]==' ') j++;
                        }
                        buffer2+=i;
                        for(i=0;i<1024;i++){
                        	if(buffer2[i]==' ') break;
                        }
                        buffer2[i]='\0';
                        strcpy(proc_pri,buffer2);
                        for(j=0;i<1024&&j<4;i++){
                        	if(buffer2[i]==' ') j++;
                        }
                        buffer2+=i;
                        for(i=0;i<1024;i++){
                        	if(buffer2[i]==' ') break;
                        }
                        buffer2[i]='\0';
                        strcpy(proc_takeup,buffer2);
                        
                        if(!strcmp(proc_stat,"R")) (*q)++;
                        if(!strcmp(proc_stat,"S")) (*r)++;
			            if(!strcmp(proc_stat,"Z")) (*s)++;
                                
                        sprintf(text[0],"%s",proc_pid);
                        pro_info[y].pid = atoi(proc_pid);
                        sprintf(text[1],"%s",proc_name);
                        sprintf(pro_info[y].name,"%s",proc_name);
                        sprintf(text[2],"%s",proc_stat);
                        sprintf(pro_info[y].state,"%s",proc_stat);
                        sprintf(text[3],"%s",proc_pri);
                        pro_info[y].priority = atoi(proc_pri);

			            sprintf(text[4],"%s",proc_takeup);
			            pro_info[y].ram = atoi(proc_takeup);
			            int disk = atoi(disk_1)+atoi(disk_2);
			            pro_info[y].disk = disk;
			            sprintf(text[5],"%d",disk);
			            sprintf(text[6],"%s",net);
			            pro_info[y].net = atoi(net);
			            y+=1;

			            txt[0]=text[0];
		            	txt[1]=text[1];
			            txt[2]=text[2];
			            txt[3]=text[3];
		            	txt[4]=text[4];
		            	txt[5]=text[5];
		            	txt[6]=text[6];
		            

                        gtk_clist_append(GTK_CLIST(clist),txt);
                }
                n_info = y;
        } 
        closedir(dir);
}

gboolean cpu_record_callback(GtkWidget *widget,GdkEventExpose *event, gpointer  data)
{
    gtk_timeout_add(100,(GtkFunction)cpu_record_draw,(gpointer)widget);
    return TRUE;
}




char* stat_read()
{
	long  user_t, nice_t, system_t, idle_t,total_t;	/*此次读取的数据*/
	long total_c,idle_c;	/*此次数据与上次数据的差*/
	char cpu_t[10],buffer[70+1];		
	int fd;
	fd=open("/proc/stat",O_RDONLY);
	read(fd,buffer,70);
	sscanf(buffer, "%s %ld %ld %ld %ld", cpu_t, &user_t, &nice_t, &system_t, &idle_t);

	if(flag==0)	
	{
		flag=1;	
		idle=idle_t;
		total=user_t+nice_t+system_t+idle_t;
		cpu_used_percent=0;	
	}
	else
	{
		total_t=user_t+nice_t+system_t+idle_t;
		total_c=total_t-total;
		idle_c=idle_t-idle;	
		cpu_used_percent=100*(total_c-idle_c)/total_c;
		total=total_t;	/*此次数据保存*/
		idle=idle_t;
	}
	close(fd);	
	sprintf(temp_cpu,"cpu使用率：%0.1f%%",cpu_used_percent);
	//puts(temp_cpu);
	return temp_cpu;
}

gint mem_record_draw(GtkWidget *widget)	/*内存记录绘图函数*/
{
	int i;
	int my_first_data;
	GdkColor color;
	GdkDrawable *canvas;
	GdkGC *gc;
	GdkFont *font;
	canvas = widget->window; 
	gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];
 
	gdk_draw_rectangle(canvas, gc, TRUE, 0, 5, 460, 105);
	color.red = 0;
	color.green = 20000;
	color.blue = 0;
	gdk_gc_set_rgb_fg_color(gc, &color);	
	for(i=20;i<110;i+=15)	/*绘制横线*/
	{
		gdk_draw_line(canvas, gc, 10, i, 450, i);
	}

	for(i=10;i<450;i+=15)
	{
		gdk_draw_line(canvas, gc, i+mem_start_position,5, i+mem_start_position,110);			
	}
	mem_start_position-=3;
	if(mem_start_position==0)  mem_start_position=15;

	if(flag3==0)	/*第一次清空数据*/
	{
		for(i=0;i<66;i++)
			mem_data[i]=0;
		flag3=1;
		mem_first_data=0;
	}

	mem_data[mem_first_data]=(float)(mem_total-mem_free)/mem_total;
	mem_first_data++;	
	if(mem_first_data==66) mem_first_data=0;

	color.red = 0;
	color.green = 65535;
	color.blue = 0;
	gdk_gc_set_rgb_fg_color(gc, &color);
	
	my_first_data=mem_first_data;
	for(i=0;i<65;i++)
	{
		gdk_draw_line(canvas,gc,i*7,109-104*mem_data[my_first_data%66],(i+1)*7,109-104*mem_data[(my_first_data+1)%66]);
		my_first_data++;
		if(my_first_data==66)	my_first_data=0;
	}
 	color.red = 0;
	color.green = 0;
	color.blue = 0;
	gdk_gc_set_rgb_fg_color(gc, &color);
	return TRUE;
}

gboolean mem_record_callback(GtkWidget *widget,GdkEventExpose *event,gpointer data)	/*内存记录回调函数*/
{
	gtk_timeout_add(100,(GtkFunction)mem_record_draw,(gpointer)widget);
	return TRUE;
}

char* meminfo_read()
{
	char buffer[100+1];
	char data[20];	
	long total=0,free=0;	/*总内存和用户内存*/
	int i=0,j=0,counter=0;
	int fd;
	fd=open("/proc/meminfo",O_RDONLY);
	read(fd,buffer,100);	
	
	for(i=0,j=0;i<100;i++,j++)
	{
		if (buffer[i]==':') counter++;
		if (buffer[i]==':'&&counter==1)	/*MemTotal总内存*/
		{
			while(buffer[++i]==' ');
			for(j=0;j<20;j++,i++)
			{
				if(buffer[i]=='k')  break;
				data[j]=buffer[i];
			}
			data[--j]='\0';
			total=atol(data)/1024;
		}

		if (buffer[i]==':'&&counter==2)	/*MemFree空闲内存*/
		{
			while(buffer[++i]==' ');
			for(j=0;j<20;j++,i++)
			{
				if(buffer[i]=='k')  break;
				data[j]=buffer[i];
			}
			data[--j]='\0';
			free+=atol(data)/1024;
		}
	}
	mem_total=total;
	mem_free=free;
	sprintf(temp_mem,"内存:%ldM/%ldM",total-free,total);
	close(fd);	
	return temp_mem;
}



char* procsum_read()	/*进程数*/
{
	int i,sum=0;	/*进程总数*/ 
	int fd;
	char path[30];	

	for(i=1;i<32768;i++)
	{
		sprintf(path,"/proc/%d/statm",i);
		if( !((fd=open(path,O_RDONLY))<0))
		{
			sum++; 		
			close(fd);
		}			
	}
	sprintf(temp_process,"进程数：%d",sum);
	//puts(temp_process);
	return temp_process;
}


gint process_refresh(gpointer process_label)
{
	gtk_label_set_text(GTK_LABEL(process_label),procsum_read());	
	gtk_widget_show(process_label);
	return TRUE;
}


gint cpu_refresh(gpointer cpu_label)
{
	gtk_label_set_text(GTK_LABEL(cpu_label),stat_read());
	gtk_widget_show(cpu_label);
	return TRUE;
}


gint mem_refresh(gpointer mem_label)
{
	gtk_label_set_text(GTK_LABEL(mem_label),meminfo_read());	
	gtk_widget_show(mem_label);
	return TRUE;
}


char* disc_read(){
	FILE *fp;
	char txt[100];
	temp_disc[0] = '\0';
	system("df -hl > disc.txt");
	fp = fopen("disc.txt","r");	
	while((fgets(txt,100,fp))!=NULL){
		strcat(temp_disc,txt);
	}
	fclose(fp);
	return temp_disc;
}

gint disc_refresh(gpointer disc_label){
	gtk_label_set_text(GTK_LABEL(disc_label), disc_read());
	gtk_widget_show(disc_label);
	return TRUE;
}


char* net_read(){
	FILE *fp;
	char txt[100];
	temp_nett[0] = '\0';
	fp = fopen("/proc/net/dev","r");	
	while((fgets(txt,100,fp))!=NULL){
		strcat(temp_nett,txt);
	}
	fclose(fp);
	return temp_nett;
}

gint net_refresh(gpointer net_label){
	gtk_label_set_text(GTK_LABEL(net_label), net_read());
	gtk_widget_show(net_label);
	return TRUE;
}


int main(int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *button1;
	GtkWidget *button2;
	GtkWidget *button3;
	GtkWidget *table;
	GtkWidget *notebook;
	GtkWidget *label;
	GtkWidget *frame;
	GtkWidget *frame2;
	GtkWidget *clist;
	GtkWidget *cpu_hbox;
	GtkWidget *mem_hbox;
	GtkWidget *cpu_record_drawing_area;
	GtkWidget *cpu_record;
	GtkWidget *mem_record;
	GtkWidget *mem_record_drawing_area;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *process_label;	/*进程数*/
    GtkWidget *cpu_label;	/*cpu使用率*/
    GtkWidget *mem_label;	/*内存使用情况*/
	char buf1[256],buf2[256],buf3[256],buf4[256];
	char bufferl[10];
    char bufferf1[1000];
    char bufferf2[1000];
    char bufferf3[1000];
	GtkWidget *scrolled_window;
	
	//new window
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//set title
	gtk_window_set_title(GTK_WINDOW(window), "MyCat");
	//set size
	gtk_widget_set_size_request(window,600,600);
	//set signal
	g_signal_connect (G_OBJECT(window), "delete_event",G_CALLBACK(delete_event), NULL);
	//set border
	gtk_container_set_border_width(GTK_CONTAINER(window),5);
	
	
	//new table
	table = gtk_table_new(3,6,FALSE);//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
	//add table to window
	gtk_container_add(GTK_CONTAINER(window),table);
	
	
	//new notebook
	notebook = gtk_notebook_new();
	//set position
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);
	//attach  notebook to table
	gtk_table_attach_defaults(GTK_TABLE(table),notebook,0,6,0,1);//YYYYYYYYYYYYYYYYYY
	gtk_widget_show(notebook);
	
	
/*1.建立系统信息选项卡*/
	sprintf(bufferf1, "INFO of CPU");
    sprintf(bufferf2,"INFO of OS");
    sprintf(bufferl, "System");
    
    //new vbox
    vbox = gtk_vbox_new(FALSE,0);
    
    
    //new frame bufferf1 as title
    frame = gtk_frame_new(bufferf1);
    gtk_container_set_border_width(GTK_CONTAINER(frame),5);
    //set frame size
    gtk_widget_set_size_request(frame,500,150);
    sprintf(bufferf1, "    CPU：%s\n    CPU TYPE：%s\n    CPU_F：%s MHz\n    Cache SZIE：%s\n",get_cpu_name(buf1),get_cpu_type(buf2),get_cpu_f(buf3),get_cache_size(buf4));
    //set label
    label = gtk_label_new(bufferf1);
    //add label to frame
    gtk_container_add(GTK_CONTAINER(frame),label);
    gtk_widget_show(label);
    gtk_box_pack_start(GTK_BOX(vbox),frame,FALSE,FALSE,5);
    gtk_widget_show(frame);
    
    //new frame2 bufferf2 as title
    frame2 = gtk_frame_new(bufferf2);
    gtk_container_set_border_width(GTK_CONTAINER(frame2),5);
    gtk_widget_set_size_request(frame2,500,150);
    sprintf(bufferf2, "    OS：%s\n    OS VERSION：%s\n    GCC VERSION：%s\n",get_system_type(buf1),get_system_version(buf2),get_gcc_version(buf3));
    label = gtk_label_new(bufferf2);
    gtk_container_add(GTK_CONTAINER(frame2),label);
    gtk_widget_show(label);
    gtk_box_pack_start(GTK_BOX(vbox),frame2,FALSE,FALSE,5);
    gtk_widget_show(frame2);
    
    gtk_widget_show(vbox);
    //set page of notebook
    label = gtk_label_new(bufferl);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, label);
    
/*2.建立进程信息选项卡*/
    sprintf(bufferl,"Process");
    hbox = gtk_hbox_new(FALSE,5);
    
    scrolled_window = gtk_scrolled_window_new(NULL,NULL);
    gtk_widget_set_size_request(scrolled_window,300,300);
    //
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    
    clist = gtk_clist_new(7);
    int p=0,q=0,r=0,s=0;
    get_proc_info(clist,&p,&q,&r,&s);
    //gtk_clist_column_titles_active(clist);
    gtk_signal_connect(GTK_OBJECT(clist),"select_row",GTK_SIGNAL_FUNC(select_row_callback),NULL);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window),clist);
    gtk_box_pack_start(GTK_BOX(hbox),scrolled_window,TRUE,TRUE,5);
    
    //sort
    gtk_signal_connect(GTK_OBJECT(clist),"click_column",GTK_SIGNAL_FUNC(select_column_callback),NULL);
    
    
    vbox = gtk_vbox_new(FALSE,5);
    frame = gtk_frame_new(bufferl);
    gtk_widget_set_size_request(frame,100,215);
    sprintf(bufferf1,"All process：%d\n\nRunning：%d\n\nSleeping：%d\n\nZombied：%d\n",p,q,r,s);
    label = gtk_label_new(bufferf1);
    gtk_container_add(GTK_CONTAINER(frame),label);
    gtk_box_pack_start(GTK_BOX(vbox),frame,FALSE,FALSE,10);
    button1 = gtk_button_new_with_label("End Process");
    g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(kill_proc),"End Process");
    gtk_box_pack_start(GTK_BOX(vbox),button1,FALSE,FALSE,10);
    
    button2 = gtk_button_new_with_label("Update");
    g_signal_connect_swapped(G_OBJECT (button2), "clicked",G_CALLBACK (refresh), clist); 
    g_signal_connect_swapped (G_OBJECT (button2), "clicked",G_CALLBACK (refresh_lable), label); 
    gtk_box_pack_start(GTK_BOX(vbox), button2, FALSE, FALSE, 10);
    
    gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 5);
    
    gtk_widget_show_all(hbox);
    
    label = gtk_label_new(bufferl);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),hbox,label);
    
/* 3.建立内存资源选项卡 */
    GtkWidget *capability;
    capability = gtk_vbox_new(FALSE,0);	/*内存资源*/
    gtk_container_set_border_width(GTK_CONTAINER(capability),5);
    gtk_widget_set_size_request(capability,200,320);
    gtk_widget_show(capability);
    label = gtk_label_new("RESOURCE");	
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),capability,label);
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook),3);	/*把此页设为当前显示页*/
    cpu_hbox = gtk_hbox_new(FALSE,0);		/*cpu横向box*/
    gtk_box_pack_start(GTK_BOX(capability),cpu_hbox,TRUE,TRUE,2);
    gtk_widget_show(cpu_hbox);
    mem_hbox = gtk_hbox_new(FALSE,0);		/*mem横向box*/
    gtk_box_pack_start(GTK_BOX(capability),mem_hbox,TRUE,TRUE,2);
    gtk_widget_show(mem_hbox);
    
    //begin to draw
    //draw cpu
    cpu_record = gtk_frame_new("The Usage of CPU");
    gtk_container_set_border_width(GTK_CONTAINER(cpu_record),5);
    gtk_widget_set_size_request(cpu_record,1000,130);
    gtk_widget_show(cpu_record);
    gtk_box_pack_start(GTK_BOX(cpu_hbox),cpu_record,TRUE,TRUE,2);
    cpu_record_drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(cpu_record_drawing_area,50,50);
    //g_signal_connect(G_OBJECT(cpu_record_drawing_area), "expose_event",G_CALLBACK(cpu_record_callback),NULL);
    gtk_timeout_add(1000,(GtkFunction)cpu_record_draw,(gpointer)cpu_record_drawing_area);
    gtk_container_add(GTK_CONTAINER(cpu_record),cpu_record_drawing_area);
    gtk_widget_show(cpu_record_drawing_area);
    
    //draw ram
    mem_record = gtk_frame_new("Usage of RAM");	
    gtk_container_set_border_width(GTK_CONTAINER(mem_record),5);
    gtk_widget_set_size_request(mem_record,1000,130);
    gtk_widget_show(mem_record);
    gtk_box_pack_start(GTK_BOX(mem_hbox),mem_record,TRUE,TRUE,2);

    mem_record_drawing_area = gtk_drawing_area_new ();
    gtk_widget_set_size_request (mem_record_drawing_area, 50,50);
    //g_signal_connect (G_OBJECT(mem_record_drawing_area), "expose_event",G_CALLBACK(mem_record_callback),NULL);
    gtk_timeout_add(1000,(GtkFunction)mem_record_draw,(gpointer)mem_record_drawing_area);
    gtk_container_add (GTK_CONTAINER(mem_record), mem_record_drawing_area);
    gtk_widget_show (mem_record_drawing_area);
    
    //text
    hbox = gtk_hbox_new(FALSE,0);		
    gtk_box_pack_start(GTK_BOX(capability),hbox,FALSE,FALSE,2);
    gtk_widget_show(hbox);

    process_label = gtk_label_new("");
    cpu_label = gtk_label_new("");
    mem_label = gtk_label_new("");	

    gtk_timeout_add(100,(GtkFunction)process_refresh,(gpointer)process_label);	/*进程数刷新*/
    gtk_timeout_add(100,(GtkFunction)cpu_refresh,(gpointer)cpu_label);	/*cpu使用率刷新*/
    gtk_timeout_add(100,(GtkFunction)mem_refresh,(gpointer)mem_label);	/*内存使用刷新*/


    gtk_label_set_justify(GTK_LABEL(process_label),GTK_JUSTIFY_RIGHT);
    gtk_label_set_justify(GTK_LABEL(cpu_label),GTK_JUSTIFY_RIGHT);
    gtk_label_set_justify(GTK_LABEL(mem_label),GTK_JUSTIFY_RIGHT);
    gtk_box_pack_start(GTK_BOX(hbox),process_label,FALSE,FALSE,10);
    gtk_box_pack_start(GTK_BOX(hbox),cpu_label,FALSE,FALSE,10);
    gtk_box_pack_start(GTK_BOX(hbox),mem_label,FALSE,FALSE,10);
    gtk_widget_show(process_label);
    gtk_widget_show(cpu_label);
    gtk_widget_show(mem_label);    
    
    
    //4
   /* GtkWidget * disk_label;

    GtkWidget *diskbox = gtk_hbox_new(FALSE,0);
    GtkWidget *netbox = gtk_hbox_new(FALSE,0);
    GtkWidget *whole = gtk_vbox_new(FALSE,0);
    GtkWidget *net_label = gtk_label_new("NET STATE");
    
    gtk_container_set_border_width(GTK_CONTAINER(whole),5);
    gtk_widget_set_size_request(whole,200,320);
    gtk_widget_show(whole);
    label = gtk_label_new("DISK&NET");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),whole,label);
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook),4);
    
    
    
    disk_label = gtk_label_new("DISK STATE");
    gtk_box_pack_start(GTK_BOX(whole),diskbox,FALSE,FALSE,10);
    gtk_timeout_add(1000,(GtkFunction)disc_refresh,(gpointer)disk_label);
    gtk_box_pack_start(GTK_BOX(diskbox),disk_label,FALSE,FALSE,10);
    gtk_widget_show(disk_label);
    
    net_label = gtk_label_new("DISK STATE");
    gtk_box_pack_start(GTK_BOX(whole),netbox,FALSE,FALSE,10);
    gtk_timeout_add(1000,(GtkFunction)net_refresh,(gpointer)net_label);
    gtk_box_pack_start(GTK_BOX(netbox),net_label,FALSE,FALSE,10);
    gtk_widget_show(net_label);  
    
     
    gtk_widget_show(table);
    gtk_widget_show(window);
    gtk_main();*/
    
    char buff[1024];
    
    
    GtkWidget *frame1;
    GtkWidget *net_label;
    //网络信息选项卡
	sprintf(buff,"NET");
	vbox = gtk_vbox_new(FALSE, 0);

	frame1 = gtk_frame_new(buff);
	gtk_container_set_border_width(GTK_CONTAINER(frame1), 10);
	gtk_widget_set_size_request(frame1, 500, 150);
	net_label = gtk_label_new("");
	gtk_timeout_add(100,(GtkFunction)net_refresh,(gpointer)net_label);	
	gtk_container_add(GTK_CONTAINER(frame1), net_label);
	gtk_widget_show(net_label);
	gtk_box_pack_start(GTK_BOX(vbox), frame1, FALSE, FALSE, 5);
	gtk_widget_show(frame1); 

	gtk_widget_show(vbox);
	
	
	
	
   GtkWidget *frame3;
    GtkWidget *disk_label;
    //GtkWidget *vbox2;
    //网络信息选项卡
	sprintf(buff,"DISK");
	//vbox2 = gtk_vbox_new(FALSE, 0);

	frame3 = gtk_frame_new(buff);
	gtk_container_set_border_width(GTK_CONTAINER(frame3), 10);
	gtk_widget_set_size_request(frame3, 500, 250);
	disk_label = gtk_label_new("");
	gtk_timeout_add(100,(GtkFunction)disc_refresh,(gpointer)disk_label);	
	gtk_container_add(GTK_CONTAINER(frame3), disk_label);
	gtk_widget_show(disk_label);
	gtk_box_pack_start(GTK_BOX(vbox), frame3, FALSE, FALSE, 5);
	gtk_widget_show(frame3); 

	gtk_widget_show(vbox);
	
	
	label = gtk_label_new("NET&DISK");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, label);
	
    gtk_widget_show(table);
    gtk_widget_show(window);
    gtk_main();
    
    
    return 0;
	
}
