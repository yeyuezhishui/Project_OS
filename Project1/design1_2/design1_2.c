#include<gtk/gtk.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

void destroy_progress(GtkWidget *widget);
void *set_time();
void *set_sum();
void *set_file();
void gtk_happen(GtkWidget * lable);


pid_t pid1,pid2,pid3;
char t[100];//记录实时时间
char file_content[1000];//记录文件每行内容
char csum[100];//记录累加结果
GtkWidget * label1;
GtkWidget * label2;
GtkWidget * label3;

int main(int argc, char* argv[])
{
	/*
	创建三个线程，分别用来:动态获得时间,动态获取文件信息,动态累加求和
	*/
	pthread_t th1,th2,th3;
	int re;
	/*
	创建三个进程,父进程用来显示时间,pid1用来逐行读取文件,pid2用来动态求和
	*/

	pid1 = fork();
	if(pid1 != 0)
	{
		pid2 = fork();
	}
	if(pid1 != 0 && pid2 != 0)
	{
		label1 = gtk_label_new(t);
		re = pthread_create(&th1, NULL, (void *)set_time, NULL);
		gtk_init(&argc, &argv);
		gtk_happen(label1);
		//re = pthread_join(th1, NULL);
	}
	if(pid1 == 0)
	{
		label2 = gtk_label_new(csum);
		re = pthread_create(&th2, NULL, (void *)set_sum, NULL);
		gtk_init(&argc, &argv);
		gtk_happen(label2);
		//re = pthread_join(th2, NULL);
	}
	else if(pid2 == 0)
	{
		label3 = gtk_label_new(file_content);
		re = pthread_create(&th3, NULL, (void *)set_file, NULL);
		gtk_init(&argc, &argv);
		gtk_happen(label3);
	}
}



//销毁Gtk
void destroy_progress( GtkWidget *widget)
{
    gtk_main_quit ();
}

//线程1
void *set_time()
{
	while(1)
	{
		time_t time1;
		time(&time1);
		sprintf(t,"%s",ctime(&time1));
		gtk_label_set_text(GTK_LABEL(label1),t);
		sleep(1);
	}
}

void *set_sum()
{
	int i = 1;
	int sum = 0;
	int pre_sum = 0;
	while(1)
	{
		pre_sum = sum;
		sum = pre_sum + i;
		i++;
		sprintf(csum,"%d + %d = %d",pre_sum,i,sum);
		gtk_label_set_text(GTK_LABEL(label2),csum);
		sleep(1);
	}
}

void *set_file()
{
	FILE *fp = NULL;
	fp = fopen("/etc/fstab","rb");
	while(!feof(fp))
	{
		fgets(file_content,10000,fp);
		gtk_label_set_text(GTK_LABEL(label3),file_content);
		sleep(1);
	}
}

//gtk结构
void gtk_happen(GtkWidget * label)
{
		GtkWidget * window;//定义一个窗口
		GtkWidget * vbox;//定义一个组装盒
		GtkWidget * button;//定义一个按钮
		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);//创建一个新的窗口
		gtk_window_set_resizable (GTK_WINDOW(window), TRUE); //修改窗体的伸缩属性
		gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);//设定窗口的位置
		g_signal_connect (G_OBJECT(window), "destroy", G_CALLBACK (destroy_progress), NULL);//监听窗口的destroy事件
		gtk_window_set_title (GTK_WINDOW (window), "Timer");//设定窗口的标题
		gtk_container_set_border_width (GTK_CONTAINER(window), 20);//用来设定宽度
		
		//使用gtk_vbox_new函数建立纵向组装盒;
    //为了显示构件，必须将构件放入组装盒中,并将组装盒放在窗口内;
    vbox = gtk_vbox_new (FALSE, 10);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 100);//用来设定宽度;
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);
    
    //lable构件放入组装盒
    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 10);
    gtk_widget_show (label);
		
		button = gtk_button_new_with_label("Close");
		g_signal_connect_swapped (G_OBJECT (button), "clicked", G_CALLBACK (gtk_widget_destroy), window);//信号登记函数,监听按钮的clicked事件
		gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 10);
    GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
    gtk_widget_grab_default(button);
    gtk_widget_show (button);
    
    gtk_widget_show (window);
    //准备将窗口和所有的组件显示在屏幕上,这个函数必须在GTK程序的最后调用.
    gtk_main ();
}


