#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/fs.h> 
#include <linux/init.h> 
#include <linux/uaccess.h> 

#if CONFIG_MODVERSIONS == 1 
#define MODVERSIONS 
#include <linux/version.h> 
#endif 

#define DEVICE_NUM 0 //随机产生一个设备号 

static int device_num = 0; //用来保存创建成功后的设备号 
static char buffer[1024] = "mydriver"; //数据缓冲区 
static int open_nr = 0; //打开设备的进程数，用于内核的互斥 

//函数声明 
//inode；linux下文件的管理号。
//file：linux一切皆文件。文件结构体代表一个打开的文件，系统中的每个打开的文件在内核空间都有一个关联的 struct file。它由内核在打开文件时创建，并传递给在文件上进行操作的任何函数。在文件的所有实例都关闭后，内核释放这个数据结构 
static int mydriver_open(struct inode *inode, struct file *filp); 
static int mydriver_release(struct inode *inode, struct file* filp); 
static ssize_t mydriver_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos); //loff-t:long long 型 
static ssize_t mydriver_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos); //__user表明参数是一个用户空间的指针，不能在kernel代码中直接访问。 
//size_t:一个基本的无符号整数的C / C + +类型 
//填充file_operations结构相关入口 
static struct file_operations mydriver_fops = { 
.read = mydriver_read, 
.write = mydriver_write, 
.open = mydriver_open, 
.release = mydriver_release, 
}; 

//打开函数 
static int mydriver_open(struct inode *inode, struct file *filp) 
{ 
    printk("\nMain device is %d, and the slave device is %d\n", MAJOR(inode->i_rdev), MINOR(inode->i_rdev)); //把主从设备号传入 
    if (open_nr == 0) 
    { 
        open_nr++; 
        try_module_get(THIS_MODULE); //尝试打开模块 
        return 0; 
    } 
    else 
    { 
        printk(KERN_ALERT "Another process open the char device.\n");//进程挂起 
        return -1; 
    } 
} 

//读函数 
static ssize_t mydriver_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos) 
{ 
//if (buf == NULL) return 0; 
    if (copy_to_user(buf, buffer, sizeof(buffer))) //读缓冲 ,第一个参数是to：用户空间的地址，第二个参数是from，内核空间的地址，第三个参数是要从内核空间拷贝的字节数 
    { 
        return -1; 
    } 
    return sizeof(buffer); 
} 

//写函数,将用户的输入字符串写入 
static ssize_t mydriver_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos) 
{ 
//if (buf == NULL) return 0; 
    if (copy_from_user(buffer, buf, sizeof(buffer))) //写缓冲 
    { 
        return -1;
    } 
    return sizeof(buffer); 
} 

//释放设备函数 
static int mydriver_release(struct inode *inode, struct file* filp) 
{ 
    module_put(THIS_MODULE); //释放模块
    open_nr--; //进程数减1 
    printk("The device is released!\n"); 
    return 0; 
} 

//注册设备函数 
static int __init mydriver_init(void) 
{ 
    int result; 
    printk(KERN_ALERT "Begin to init Char Device!"); //注册设备 
    //向系统的字符登记表登记一个字符设备 
    result = register_chrdev(DEVICE_NUM, "mydriver", &mydriver_fops); //第一个参数等于0，则表示采用系统动态分配的主设备号；不为0，则表示静态注册。 第二个参数命名， 第三个参数为其地址 
    if (result < 0) 
    { 
        printk(KERN_WARNING "mydriver: register failure\n"); 
        return -1; 
    } 
    else
    { 
        printk("mydriver: register success!\n"); 
        device_num = result; 
        return 0; 
    } 
} 

//注销设备函数 
static void __exit mydriver_exit(void) 
{ 
    printk(KERN_ALERT "Unloading...\n"); 
    unregister_chrdev(device_num, "mydriver"); //注销设备 
    printk("unregister success!\n"); 
} 

//模块宏定义 
module_init(mydriver_init); //模块加载函数
module_exit(mydriver_exit); //设备卸载函数 
MODULE_LICENSE("GPL"); // "GPL" 是指明了 这是GNU General Public License的任意版本
