#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#define BUFFER_SIZE 256
MODULE_LICENSE("GPL");
static unsigned int gpioLED = 49;
static unsigned int gpioIn = 117;

int lkm_open (struct inode *pinode, struct file *pfile) {
    printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
    return 0;
}
ssize_t lkm_read (struct file *pfile,
                  char __user *buffer,
                  size_t length,
                  loff_t *offset) {
    unsigned long unRead = 0 ;
    int device_buffer = (int)gpio_get_value(gpioIn);
    unRead = copy_to_user(buffer, (void*)&device_buffer, sizeof(int));
    if(unRead)
        printk(KERN_ALERT "%lu bytes could not be copied\n", unRead);
    return sizeof(int);
}
ssize_t lkm_write (struct file *pfile,
                   const char __user *buffer,
                   size_t length,
                   loff_t *offset) {
    char *buffer_internal;
    int i = 0;
    buffer_internal = kmalloc(length, GFP_KERNEL);
    if (buffer_internal == NULL)
        return -ENOMEM;
    if (copy_from_user(buffer_internal, buffer, length)) {
        kfree(buffer_internal);
        return -EFAULT;
    }
    buffer_internal[length - 1] = '\0';
    while (buffer_internal[i]!= '\0' ) {

        if(buffer_internal[i]=='0')
            gpio_set_value(gpioLED, 0);
        else
            gpio_set_value(gpioLED, 1);
        i++;
    }
    kfree(buffer_internal);
    return length;
}
int lkm_close (struct inode *pinode,
               struct file *pfile) {
    printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
    gpio_set_value(gpioLED, 0);
    return 0;
}
struct file_operations lkm_file_operations = {
    .owner	 = THIS_MODULE,
    .open 	 = lkm_open,
    .read    = lkm_read,
    .write	 = lkm_write,
    .release = lkm_close
};
int lkm_demo_init(void)
{   register_chrdev(
        421/*Major Number */,
        "Simple Char Drv"/*Name of Driver*/,
        &lkm_file_operations/*POINTER to File Operations*/);
    printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
    gpio_request(gpioLED, "sysfs");
    gpio_direction_output(gpioLED, 0);
    gpio_request(gpioIn, "sysfs");
    gpio_direction_input(gpioIn);
    gpio_export(gpioLED, false);
    gpio_export(gpioIn, false);
    return 0;
}
void lkm_demo_exit(void)
{
    printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
    unregister_chrdev(421, "Simple Char Drv");
    gpio_set_value(gpioLED, 0);
    gpio_unexport(gpioLED);
    gpio_free(gpioLED);
    gpio_unexport(gpioIn);
    gpio_free(gpioIn);
    unregister_chrdev(421, "Simple Char Drv");
    printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
}
module_init(lkm_demo_init);
module_exit(lkm_demo_exit);
