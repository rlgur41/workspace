#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "md5char"
#define CLASS_NAME "md5"

MODULE_LICENSE("GPL");;
MODULE_AUTHOR("HL OS LAB");
MODULE_DESCRIPTION("Protected by MD5 character");
MODULE_VERSION("2");

static int major;
static int sizeof_message;
static char message[128] = { 0 };
static struct class* md5char_class = NULL;
static struct device* md5char_device = NULL;

static int md5char_open(struct inode*, struct file*);
static ssize_t md5char_read(struct file*, char*,  size_t, loff_t*);
static int md5char_write(struct file*, char*, size_t, loff_t*);
static int md5char_release(struct inode*, struct file*);



static struct file_operations fops = {

  .open = md5char_open,
  .read = md5char_read,
  .write = md5char_write,
  .release = md5char_release,
};

static int __init md5char_init(void){

  printk(KERN_INFO "[Init md5char device]\n");

  if((major = register_chrdev(0, DEVICE_NAME, &fops)) < 0){
  printk(KERN_ALERT "[Failed to register major number]\n");
  return major;
  }
  printk(KERN_INFO "[Registered successful, register number >> %d]\n", major);

  md5char_class = class_create(THIS_MODULE, CLASS_NAME);

  if(IS_ERR(md5char_class)){
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_ALERT "[Failed to register device class]\n");
    return PTR_ERR(md5char_class);
  }
  printk(KERN_INFO "[Class create Registered successful]\n");

  md5char_device = device_create(md5char_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
        if(IS_ERR(md5char_device)){
                class_destroy(md5char_class);
                unregister_chrdev(major, DEVICE_NAME);
                printk(KERN_ALERT "[failed to create device]\n");
                return PTR_ERR(md5char_device);
        }
        printk(KERN_INFO "[DEVICE CLASS create successfully]\n");
        return 0;
}

static void __exit md5char_exit(void){
        device_destroy(md5char_class, MKDEV(major, 0));
        class_unregister(md5char_class);
        class_destroy(md5char_class);
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_INFO "[exit Module]\n");
}


static int md5char_open(struct inode* inodep, struct file* filep){
        printk(KERN_INFO "[Device stream has been opened]\n");
        return 0;
}



static ssize_t md5char_read(struct file* filep, char *buf, size_t len, loff_t *offset){
  if((copy_to_user(buf, message, sizeof_message)) == 0){
    printk(KERN_INFO "[Sent [%d] char to user level]", sizeof_message);
    return sizeof_message = 0;
  }
  else{
    printk(KERN_INFO "[Failed to send [%d] char to user level]\n", sizeof_message);
    return -EFAULT;
    }
}

static int md5char_write(struct file* filep, char *buf, size_t len, loff_t *offset){
  sprintf(message, "%s", buf);
  sizeof_message = strlen(message);
  printk(KERN_INFO "[Received [%d] char from user level]\n", sizeof_message);
  return 0;
}

static int md5char_release(struct inode* inodep, struct file* filep){
  printk(KERN_INFO "[DEVICE closed(release}]\n");
  return 0;
}
module_init(md5char_init);
module_exit(md5char_exit);
