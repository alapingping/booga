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
#include <linux/string.h>



#define SUCCESS 0
#define TOTAL_BOOGA_DEV 4

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
// static int device_file_major_number[5] = {0,0,0,0,0};
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
static const char * g_s_strings[4] = {"booga! booga!","googoo! gaagaa!","wooga! wooga!","neka! maka!"};
static const char g_s_booga[] = "booga! booga!";
static const char g_s_googoo[] = "googoo! gaagaa!";
static const char g_s_wooga[] =  "wooga! wooga!";
static const char g_s_neka[] = "neka! maka!";

static char buff[300];
static int buff_size = 0;

static char g_s_Hello_World_string[] = 
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

static int current_dev_number;

#endif