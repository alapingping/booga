#include "booga.h"

MODULE_LICENSE("Dual BSD/GPL");

//booga读写文件操作符
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
    // static int counter = 0;

    // Booga_Dev * boo = kzalloc(sizeof(Booga_Dev), GFP_KERNEL);
    printk(KERN_NOTICE "booga-driver: step: %d", 1);
    // booga_dev = container_of(inode->i_cdev, Booga_Dev , my_cdev);
    printk(KERN_NOTICE "booga-driver: step: %d", 2);
    // booga_dev->number = iminor(inode);
    printk(KERN_NOTICE "booga-driver: step: %d", 3);
    // filp->private_data = boo;
    printk(KERN_NOTICE "booga-driver: step: %d", 4);

    int major_num = imajor(inode);
    int minor_num = iminor(inode);
    printk(KERN_NOTICE "booga-driver: dev major number: %d dev minor number:%d.", major_num, minor_num);

    if( major_num == 0 && minor_num == 0 )
        current_dev_number = 4;
    else {
        current_dev_number = minor_num;
        dev_open_times[current_dev_number]++;
        for( device_index = 0; device_index < TOTAL_BOOGA_DEV; device_index++ )
            printk(KERN_NOTICE "booga-driver: device%d open times:%d" ,device_index, dev_open_times[device_index]);
    
    }
    // printk(KERN_NOTICE "booga-driver: dev minor number: %d.", MINOR(d));
    try_module_get(THIS_MODULE);
    // counter++;
    printk(KERN_NOTICE "booga-driver: booga_open() is called." );
    // MOD_INC_USE_COUNT;

    return SUCCESS;
}

static int booga_release (struct inode *inode, struct file *filp){
    
    // device_open--;
    module_put(THIS_MODULE);
    printk(KERN_NOTICE "booga-driver: booga_release() is called." );
    // MOD_DEC_USE_COUNT;

    return 0;
}

static ssize_t booga_read (struct file *filp, char *buf, size_t count, loff_t *f_pos){
   
    printk( KERN_NOTICE "Booga-driver: Device file is read at offset = %i, read bytes count = %u"
            , (int)*f_pos
            , (unsigned int)count );

    if(current_dev_number == 4){
    // Booga_Dev * boo = filp->private_data;
    
    // printk(KERN_NOTICE "booga-driver: private data: %s %d.", dev->data, dev->number);
        sprintf(buff,g_s_Hello_World_string,
        total_bytes_read, total_bytes_write,
        dev_open_times[0],dev_open_times[1],dev_open_times[2],dev_open_times[3],
        string_output_times[0],string_output_times[1],string_output_times[2],string_output_times[3]);

        buff_size = strlen(buff);

        if( *f_pos >= buff_size )
            return 0;

        if( *f_pos + count > buff_size )
            count = buff_size - *f_pos;

        if( copy_to_user(buf, buff + *f_pos, count) != 0 )
            return -EFAULT;   

        *f_pos += count;
    }
    else {

        total_bytes_read += count;

        int chioce = get_random_number();

        string_output_times[chioce]++;

        size_t output_string_size = strlen(g_s_strings[chioce]);

        printk(KERN_NOTICE "booga-driver: output_string_size: %d.", output_string_size);

        if( *f_pos >= g_s_strings[chioce] )
            return 0;

        if( *f_pos + count > output_string_size )
            count = output_string_size - *f_pos;

        if( copy_to_user(buf, g_s_strings[chioce] + *f_pos, count) != 0 )
            return -EFAULT;   

        *f_pos += count;
       
        printk(KERN_NOTICE "booga-driver: total read: %d.", total_bytes_read);
    }
   return count;
}

static ssize_t booga_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos){
    

    // Booga_Dev * boo = filp->private_data;
    printk(KERN_NOTICE "booga-driver: %d bytes wrote to kernel.", count);
    // printk(KERN_NOTICE "booga-driver: private data: %s %d.", dev->data, dev->number);

    total_bytes_write += count;
    printk(KERN_NOTICE "booga-driver: total write: %d.", total_bytes_write);
    return count;
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

    for( device_index = 1; device_index < 5; device_index++ ){
        device_create(dev_class, NULL, \
        MKDEV(MAJOR(dev), device_index-1), \
        NULL, device_name[device_index]);
        printk(KERN_NOTICE "booga-driver: device %d creating.", device_index);
    }


    //  for(device_index = 1; device_index < 5; device_index++ ){
      
    //     int result = 0;
    //     printk( KERN_NOTICE "Booga-driver: register_device() is called." );
    //       result = register_chrdev( 0, device_name[device_index], &booga_driver_fops );
    //       if( result < 0 )
    //       {
    //           printk( KERN_WARNING "Booga-driver:  can\'t register character device with errorcode = %i", result );
    //           return result;
    //       }
    //       device_file_major_number[device_index] = result;

    //       dev_class = class_create(THIS_MODULE, "booga");
    //       device_create(dev_class, NULL, MKDEV(device_file_major_number[device_index], 0), NULL, device_name[device_index]);
        
    //   }
      printk( KERN_NOTICE "Booga-driver: registered character device with major number = 1 and minor numbers 0...255");
      return 0;
}

//设备注销
void unregister_device(void)
{

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
//    for ( device_index = 1; device_index < 5; device_index++){
//         if(device_file_major_number[device_index] != 0)
//             unregister_chrdev(device_file_major_number[device_index], device_name[device_index]);
//    }
    
}

void create_new_proc(void){
    
    printk( KERN_NOTICE "Booga-driver: creating booga" );
    /*create new file in /proc/driver which is called booga*/
    proc_create("driver/booga", 0, NULL, &booga_driver_fops);

}

void remove_proc_file(void){

    printk( KERN_NOTICE "Booga-driver: cleaning booga" );
    remove_proc_entry("driver/booga",NULL);
}

int get_random_number(void){
    char val;
    get_random_bytes(&val, 1);
    int chioce = ( val & 0x7F ) % 4;
    return chioce;
}


static int __init booga_init(void){
    int result = 0;
    printk( KERN_NOTICE "Booga-driver: Initialization started" );
    create_new_proc();

    // booga_dev = (Booga_Dev*)kmalloc(sizeof(Booga_Dev), GFP_KERNEL);

    register_device();
    return 0;
}

static void __exit booga_exit(void){
    printk( KERN_NOTICE "Booga-driver: Exiting" );
    // unregister_device();
    remove_proc_file();
}

module_init(booga_init);
module_exit(booga_exit);
