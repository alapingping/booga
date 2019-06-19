#include "booga.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("ZYP");

/* the file operation struct for booga */
static struct file_operations booga_driver_fops = 
{
   .owner   = THIS_MODULE,
   .read    = booga_read,
   .write   = booga_write,
   .open    = booga_open,
   .release = booga_release,
};

u32 dev_major = 0;

static int booga_open (struct inode *inode, struct file *filp){

    /* get the major and minor device number for figuring out what the file is */
    int major_num = imajor(inode);
    int minor_num = iminor(inode);
    printk(KERN_NOTICE "booga-driver: dev major number: %d dev minor number:%d.", major_num, minor_num);

    /* 
    
        if both the major device number and minor device number are 0,
        then  the file opened is the /proc/driver/booga
        else the file is the device node in /dev/booga[0,1,2,3]
    
     */
    if( major_num == 0 && minor_num == 0 )
        current_dev_number = 4;
    else {
        current_dev_number = minor_num;
        dev_open_times[current_dev_number]++;
        for( device_index = 0; device_index < TOTAL_BOOGA_DEV; device_index++ )
            printk(KERN_NOTICE "booga-driver: device%d open times:%d" ,device_index, dev_open_times[device_index]);
    }


    //使booga模块引用计数加1
    //避免多个程序同时打开设备
    //导致文件内容错乱
    try_module_get(THIS_MODULE);

    printk(KERN_NOTICE "booga-driver: booga_open() is called." );
    return SUCCESS;
}

static int booga_release (struct inode *inode, struct file *filp){
    
    cat_lock = 0;
    //是模块计数减1
    //达到同步的目的
    module_put(THIS_MODULE);
    printk(KERN_NOTICE "booga-driver: booga_release() is called." );

    return 0;
}

static ssize_t booga_read (struct file *filp, char *buf, size_t count, loff_t *f_pos){
   
    printk( KERN_NOTICE "Booga-driver: Device file is read at offset = %i, read bytes count = %u"
            , (int)*f_pos
            , (unsigned int)count );
    
    unsigned int max = 65536;

    if(current_dev_number == 4){
        
        /*
            打开的文件是/proc/driver/booga
         */

        target_output = make_target(1,300);

    }
    else if( count == max ){
        
        /*
            打开的文件是设备文件
         */

        target_output = make_target(2, 20);

    }
    else {

        /*
            用户通过test-booga读取输出
         */

        target_output = make_target(3, count);

    }

    if( *f_pos >= target_output_size )
        return 0;

    if( *f_pos + count > target_output_size )
        count = target_output_size - *f_pos;

    if( copy_to_user(buf, target_output + *f_pos, count) != 0 )
        return -EFAULT;   

    *f_pos += count;

    kfree(temp);
    
   return count;
}

static ssize_t booga_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos){
    
    if( current_dev_number == 3 ){
        return count;
    }

    printk(KERN_NOTICE "booga-driver: %d bytes wrote to kernel.", count);
    
    //将读取的字节数累加
    total_bytes_write += count;
    printk(KERN_NOTICE "booga-driver: total write: %d.", total_bytes_write);
  
    return count;
}

char * make_target(int type, int length){

    //临时分配字符指针用于制作目标输出
    temp = kmalloc(length * sizeof(char), GFP_KERNEL);

    if(type == 1){

        /*
            如果打开文件是/proc/driver/booga
            则组装g_s_content,返回到用户缓冲区
         */

        sprintf(temp,g_s_content,
                total_bytes_read, total_bytes_write,
                dev_open_times[0],dev_open_times[1],dev_open_times[2],dev_open_times[3],
                string_output_times[0],string_output_times[1],string_output_times[2],string_output_times[3]);
        target_output_size = strlen(temp);
        return temp;
    }

    int chioce = get_random_number();

    if(type == 2){

        /*
            打开的文件是/dev/booga0123
            则随机输出四种字符串中的一种
         */

        if( cat_lock == 0 ){
            cat_lock = 1;
            strcpy(temp, g_s_strings[chioce]);
            strcat(temp, "\n");
            printk(KERN_NOTICE "strlen : %d", strlen(g_s_strings[chioce]));
            target_output_size = strlen(g_s_strings[chioce]);
            total_bytes_read += ( target_output_size );
            string_output_times[chioce]++;
        }
    }
    else{

        /*
            用户指定字符长度，制作目标长度字符串
            并向用户返回
         */
        string_output_times[chioce]++;
        int cpy_times = length / strlen(g_s_strings[chioce]);
        strcpy(temp, g_s_strings[chioce]);
        int i;
        for(i = 1; i < cpy_times; i++){
            strcat(temp,  g_s_strings[chioce]);
        }
        strncat(temp,  g_s_strings[chioce], length % strlen(g_s_strings[chioce]));
        target_output_size = length;
        total_bytes_read += length;
    }

    return temp;
}



//设备注册
int register_device(void)
{
    
    int i;
    //申请设备号
    i = alloc_chrdev_region(&dev, 0, 4, "booga");

    //初始化字符对象设备
    cdev_init(&led_cdev, &booga_driver_fops);

    //注册字符设备对象到内核
    cdev_add(&led_cdev, dev, 4);

    //创建设备类
    dev_class = class_create(THIS_MODULE, "booga");

    //创建四个设备文件-----/dev/booga0123
    for( device_index = 1; device_index < 5; device_index++ ){
        device_create(dev_class, NULL, \
        MKDEV(MAJOR(dev), device_index-1), \
        NULL, device_name[device_index]);
        printk(KERN_NOTICE "booga-driver: device %d creating.", device_index);
    }

      printk( KERN_NOTICE "Booga-driver: registered character device with major number = 1 and minor numbers 0...255");
      return 0;
}

//设备注销
void unregister_device(void)
{
    //删除四个设备文件-----/dev/booga0123
    for( device_index = 1; device_index < 5; device_index++ ){
        device_destroy(dev_class, \
        MKDEV(MAJOR(dev), device_index - 1));
        printk(KERN_NOTICE "booga-driver: device %d deleted.", device_index);
    }
    class_destroy(dev_class);
    //卸载字符对象设备
    cdev_del(&led_cdev);
    //释放设备号
    unregister_chrdev_region(dev, 4);

   printk( KERN_NOTICE "Booga-driver: unregister_device() is called" );
    
}

void create_new_proc(void){
    
    printk( KERN_NOTICE "Booga-driver: creating booga" );
    /*create new file in /proc/driver which is called booga*/
    proc_create("driver/booga", 0, NULL, &booga_driver_fops);

}

void remove_proc_file(void){

    printk( KERN_NOTICE "Booga-driver: cleaning booga" );
    //删除/proc/driver/booga
    remove_proc_entry("driver/booga",NULL);
}

int get_random_number(void){
    char val;
    get_random_bytes(&val, 1);
    int chioce = ( val & 0x7F ) % 4;
    return chioce;
}


static int __init booga_init(void){

    printk( KERN_NOTICE "Booga-driver: Initialization started" );
    create_new_proc();
    register_device();
    return 0;
}

static void __exit booga_exit(void){
    printk( KERN_NOTICE "Booga-driver: Exiting" );
    unregister_device();
    remove_proc_file();
}

module_init(booga_init);
module_exit(booga_exit);
