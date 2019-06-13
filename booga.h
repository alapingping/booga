#ifndef _BOOGA_H_
#define _BOOGA_H_

#include <linux/random.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <asm/errno.h> 
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/cdev.h>



#define SUCCESS 0

void create_new_proc(void);
void remoev_proc_file(void);
void unregister_device(void);
int register_device(void);
int get_random_number(void);
void get_random_bytes(void *buf, int nbytes);

static int booga_open (struct inode *inode, struct file *filp);
static int booga_release (struct inode *inode, struct file *filp);
static ssize_t booga_read (struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t booga_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static int __init booga_init(void);
static void __exit booga_exit(void);

//存放设备号
static int device_file_major_number[5] = {0,0,0,0,0};
//存放设备名
static const char* device_name[5] = {"booga", "booga0", "booga1", "booga2", "booga3"};
//设备引索
static int device_index;
//设备打开状态
static int device_open = 0;
//总计写入字节数
static int total_bytes_read = 0;
//总结读取字节数
static int total_bytes_write = 0;
//各设备打开次数
static int dev_open_times[] = {0,0,0,0};
//各字符串输出次数
static int string_output_times[] = {0,0,0,0};
//四种输出的字符串
static char * strings[] = {"booga! booga!", "googoo! gaagaa!", "wooga! wooga!", "neka! maka!"};


static const char    g_s_Hello_World_string[] = 
"bytes read = 0\n"
"bytes write = 0\n"
"number of opens:\n"
"  /dev/booga0 = 0 times\n"
"  /dev/booga1 = 0 times\n"
"  /dev/booga2 = 0 times\n"
"  /dev/booga3 = 0 times\n"
"strings output:\n"
"  booga! booga! = 0 times\n"
"  googoo! gaagaa! = 0 times\n"
"  wooga! wooga! = 0 times\n"
"  neka! maka! = %d times\n";

static const ssize_t g_s_Hello_World_size = sizeof(g_s_Hello_World_string);

typedef struct {
    char * data;
    unsigned int number;    /* minor number */
    struct cdev my_cdev;
}Booga_Dev;

Booga_Dev * booga_dev = NULL;

//定义设备号
static dev_t dev;
//定义设备类指针
struct class *dev_class = NULL;
static struct cdev led_cdev;


#endif