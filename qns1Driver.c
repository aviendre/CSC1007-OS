/*
 *  @file qns1Driver.c
 *  @brief Basic character Device Driver
 */

#include <linux/init.h>     //Macros to mark up functions
#include <linux/module.h>   //Core header to load LKM into the kernel
#include <linux/device.h>   //Kernel driver model
#include <linux/kernel.h>   //Kernel types, macros and functions
#include <linux/fs.h>       //Linux file system
#include <linux/uaccess.h>  //Copy to user function
#define DEVICE_NAME "CSC1007OS"  
#define CLASS_NAME "CSC1007"

//modinfo details
MODULE_LICENSE("GPL");      //License Type
MODULE_AUTHOR("CSC1007_OS");
MODULE_DESCRIPTION("Character Device Driver for linux");
MODULE_VERSION("0.1");

static int      majorNumber;
static char     message[256] = {0};
static short    size_of_message;
static int      numberOpens = 0;
static struct class* csc1007Class = NULL;
static struct device* csc1007Device = NULL;
static DEFINE_MUTEX(csc1007_mutex);

//Prototype functions
static int      dev_open(struct inode *,struct file *);
static int      dev_release(struct inode *, struct file *);
static ssize_t  dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t  dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

/**
 * @brief Character device kernel initialization. Will run once at initialization time and 
 * discarded when it is done to free up memory. 
 * 
 * @return returns 0 if successful
 */
static int __init csc1007_init(void){
    printk(KERN_INFO "DRIVER: Initializing CSC1007OS LKM\n");

    //Allocate a major number for the device dynamically
    majorNumber = register_chrdev(0,DEVICE_NAME, &fops);
    if(majorNumber < 0){
        printk(KERN_ALERT "CSC1007OS failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "CSC1007OS:registered with major number %d\n",majorNumber);

    //Register the device class
    csc1007Class = class_create(THIS_MODULE, CLASS_NAME);
    //Error-checking for the initialization of device class
    if(IS_ERR(csc1007Class)){
        unregister_chrdev(majorNumber,DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(csc1007Class);
    }
    printk(KERN_INFO "CSC1007OS: device class registered\n");

    //Register the device driver
    csc1007Device = device_create(csc1007Class, NULL, MKDEV(majorNumber,0), NULL, DEVICE_NAME);
    //Error-checking for the initialization of device driver
    if(IS_ERR(csc1007Device)){
        class_destroy(csc1007Class);
        unregister_chrdev(majorNumber,DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(csc1007Device);
    }
    printk(KERN_INFO "CSC1007OS:device class created\n");

    mutex_init(&csc1007_mutex);
    return 0;
}

/**
 * @brief Character device kernel exit function to handle any cleanup such as unregistering 
 * device class and driver so future kernels may use the major number.
 */
static void __exit csc1007_exit(void){
    mutex_destroy(&csc1007_mutex);
    device_destroy(csc1007Class, MKDEV(majorNumber,0));
    class_unregister(csc1007Class);
    class_destroy(csc1007Class);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "CSC1007OS:Closing kernel\n");
}

/**
 * @brief Open function will run each time the device is opened.
 * Incrementing the numberOpens counter to see if the device is being accesed.
 * @param inodep A pointer to an inode object 
 * @param filep A pointer to a file object
 */
static int dev_open(struct inode *inodep, struct file *filep){
    if(!mutex_trylock(&csc1007_mutex)){
        printk(KERN_ALERT "CSC1007OS:Device is in use\n");
        return -EBUSY;
    }
    numberOpens++;
    printk(KERN_INFO "CSC1007OS:Device has been opened %d times\n",numberOpens);
    return 0;
}

/**
 * @brief Read function is called whenever the device is being read from the user space.
 * copy_to_user() function to send the buffer string to the user and captures any errors
 * @param filep A pointer to a file object
 * @param buffer The pointer to the buffer to which this function writes the data
 * @param len length of message
 * @param offset any offset if required
 */
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
    int errorCount = 0;
    errorCount = copy_to_user(buffer, message, size_of_message);

    //if no error, device has been read succesfully by user
    if(errorCount == 0){
        printk(KERN_INFO "CSC1007OS:Device has been read by %d times\n",size_of_message);
        return (size_of_message = 0);    //clear the position to the start and return 0
    } else {
        printk(KERN_INFO "CSC1007OS:Failed to send %d characters to the user\n",size_of_message);
        return -EFAULT;     //return a bad address message
    }
}

/**
 * @brief Write function is called whenever the device is being written from the user space.
 * Message is copied into the message[] array in this driver using the sprintf() function.
 * @param filep A pointer to a file object
 * @param buffer The pointer to the buffer that contains the string to write to the device
 * @param len The length of the array of data that is being passed in the const charr buffer
 * @param offset Any offset if required
 */
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
    sprintf(message, "%s(%zu letters)",buffer, len);
    size_of_message = strlen(message);
    printk(KERN_INFO "CSC1007OS:Device has been written by %d times\n",len);
    return len;
}

/**
 * @brief Release function is called whenever the device is closed by the user space.
 * @param inodep A pointer to an inode object 
 * @param filep A pointer to an file object 
 * @return int 
 */
static int dev_release(struct inode *inodep, struct file *filep){
    mutex_unlock(&csc1007_mutex);
    printk(KERN_INFO "CSC1007OS:Device has been closed\n");
    return 0;
}

module_init(csc1007_init);
module_exit(csc1007_exit);