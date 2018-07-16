#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> // we need this header for char. driver support 

/*
useful function definitions from linux/fs.h
ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
int (*open) (struct inode *, struct file *);
int (*release) (struct inode *, struct file *);
*/ 


#include <linux/gpio.h>                 // Required for the GPIO functions
#include <linux/interrupt.h>            // Required for the IRQ code


MODULE_LICENSE("GPL");    

static unsigned int gpioLED = 66;
//static unsigned int irqNumber;
//static irq_handler_t  demo1_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs);



int lkm_open (struct inode *pinode, struct file *pfile){

	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	//printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	return 0; //to indicate sucess


}


ssize_t lkm_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset){

	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	return 0; //we are saying the file is empty



}


ssize_t lkm_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){

	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);

	return length;

}

int lkm_close (struct inode *pinode, struct file *pfile){

	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	return 0;


}

struct file_operations lkm_file_operations = {
//this is a variable that contains the kinds of operations you can do with this device
//the struct file_operations structure is defined in linux/fs.h 
	.owner	 = THIS_MODULE,
	.open 	 = lkm_open,
	.read    = lkm_read,
	.write	 = lkm_write,
	.release = lkm_close
	//we have to define each of these functions 

};



int lkm_demo_init(void)
{

	//step1: Register with the kernel and indicate that we are registering a char. device driver
	register_chrdev(
		422/*Major Number */,
		"Simple Char Drv2"/*Name of Driver*/,
		&lkm_file_operations/*POINTER to File Operations*/);


	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	

	 gpio_direction_output(gpioLED, 1);   // Set the gpio to be in output mode and on
	 gpio_export(gpioLED, false);
	 gpio_set_value(gpioLED, 1);

	return 0;

}

void lkm_demo_exit(void)
{
	//We must unregister the Char. device driver:

	gpio_set_value(gpioLED, 0);
	gpio_free(gpioLED);
	unregister_chrdev(421, "Simple Char Drv");


	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	//no return statement!

}

module_init(lkm_demo_init);
module_exit(lkm_demo_exit);


//static irq_handler_t lkm_demo_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs){


	//return (irq_handler_t) IRQ_HANDLED;      // Announce that the IRQ has been handled correctly
//}
