#ifndef _BOOGA_H_
#define _BOOGA_H_

#include <linux/random.h>   /* get_random_bytes() */
#include <linux/module.h>   /* THIS_MODULE */
#include <linux/init.h>     /* __init __exit */
#include <linux/fs.h>       /* file stuff */
#include <asm/uaccess.h>    /* copy_to_user() */
#include <linux/proc_fs.h>  /* proc_create() remove_proc_entry() */
#include <linux/kernel.h>   
#include <asm/errno.h>      
#include <linux/slab.h>     /* kmalloc() */
#include <linux/device.h>   /* class stuff */
#include <linux/cdev.h>     /* char device stuff */
#include <linux/string.h>   /* strlen strcat strcpy */



#define SUCCESS 0
#define TOTAL_BOOGA_DEV 4

/**
 * 功能：在/proc/driver/内创建booga文件
 */
void create_new_proc(void);

/**
 * 功能：删除/proc/driver/booga
 */
void remoev_proc_file(void);

/**
 * 功能：注销已注册的设备
 */
void unregister_device(void);

/**
 * 功能：在/dev下注册设备，具体包括/dev/booga0123，并注册到/proc/devices中
 * 返回值：0----成功   其他----失败
 */
int register_device(void);

/**
 * 功能：在0,1,2,3中随机获取一个数
 * 返回值：随机到的数字
 */
int get_random_number(void);

/**
 * 功能：根据打开文件的类型，确定最终返回给用户的信息
 * 参数：type----文件类型
 *      length----构造目标字符串的长度
 * 返回值：要返回给用户的字符串
 */
char * make_target(int type, int length);

static int booga_open (struct inode *inode, struct file *filp);
static int booga_release (struct inode *inode, struct file *filp);
static ssize_t booga_read (struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t booga_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static int __init booga_init(void);
static void __exit booga_exit(void);

//存放设备名
static const char* device_name[5] = {"booga", "booga0", "booga1", "booga2", "booga3"};
//设备引索
static int device_index;
//总计写入字节数
static int total_bytes_read = 0;
//总结读取字节数
static int total_bytes_write = 0;
//各设备打开次数
static int dev_open_times[] = {0,0,0,0};
//各字符串输出次数
static int string_output_times[] = {0,0,0,0};
//四种输出的字符串
static const char * g_s_strings[4] = {"booga! booga! ","googoo! gaagaa! ","wooga! wooga! ","neka! maka! "};
//文件内容信息模板
static char g_s_content[] = 
"bytes read = %d\n"
"bytes write = %d\n"
"number of opens:\n"
"  /dev/booga0 = %d times\n"
"  /dev/booga1 = %d times\n"
"  /dev/booga2 = %d times\n"
"  /dev/booga3 = %d times\n"
"strings output:\n"
"  booga! booga! = %d times\n"
"  googoo! gaagaa! = %d times\n"
"  wooga! wooga! = %d times\n"
"  neka! maka! = %d times\n";

//定义设备号
static dev_t dev;
//定义设备类指针
struct class *dev_class = NULL;
//分配cdev
static struct cdev led_cdev;
//最终向用户返回的目标字符串
static char * target_output;
//最终信息长度
static int target_output_size;
//当前打开设备号
static int current_dev_number;
//cat锁，避免cat时重复计数
static int cat_lock = 0;

char * temp;
#endif