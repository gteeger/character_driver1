
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> // we need this header for char. driver support 
#include <linux/uaccess.h>
/*
   useful function definitions from linux/fs.h
   ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
   ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
int (*open) (struct inode *, struct file *);
  int (*release) (struct inode *, struct file *);
   */ 


#include <linux/gpio.h>                 // Required for the GPIO functions
#include <linux/interrupt.h>            // Required for the IRQ code
#define BUFFER_SIZE 256
MODULE_LICENSE("GPL");    
static unsigned int gpioLED = 49;
//static unsigned int gpioLED2 = 3;
//static unsigned int irqNumber;
//static irq_handler_t  demo_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs);
//static unsigned int irqNumber;   < Used to share the IRQ number within this file
int lkm_open (struct inode *pinode, struct file *pfile){
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	//printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	return 0; //to indicate sucess
}
ssize_t lkm_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset){
#if 0
	static char device_buffer[BUFFER_SIZE];
	int bytesRead;
	int bytesToRead = BUFFER_SIZE - *offset;

	// If we are at the end of the file, STOP READING!
	if (bytesToRead == 0){
		printk(KERN_ALERT "Reached the end of the file");
		return bytesToRead;
	}
	
	// Get bytes read by subtracting return of copy_to_user (returns unread bytes)
	bytesRead = bytesToRead - copy_to_user(buffer, device_buffer + *offset, bytesToRead);
	printk(KERN_ALERT "READING with Simple Character Driver. Reading %d bytes\n", bytesRead);

	// Set offset so that we can eventually reach the end of the file
	*offset += bytesRead;

return bytesRead;


#endif

int device_buffer = 1;

//static char device_buffer[BUFFER_SIZE]; 
copy_to_user(buffer, (void*)&device_buffer, sizeof(int));



	return sizeof(int); //we are saying the file is empty

}
ssize_t lkm_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){
	//printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	//gpio_set_value(gpioLED, 1);
	//gpio_set_value(gpioLED2, 0);
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
	//printk(KERN_INFO "HVACCHAR: Entered dev_write()\n");
	//printk(KERN_INFO "HVACChar: b4 sprintf(): buffer=%c\n", buf_internal[0]);
	while (buffer_internal[i]!= '\0' ){
	
	printk(KERN_INFO "buffer_internalW = %c", buffer_internal[i]);
		if(buffer_internal[i]=='0') 
			gpio_set_value(gpioLED, 0);
		else
			gpio_set_value(gpioLED, 1);
		i++;
	}
	kfree(buffer_internal);
	//printk(KERN_ALERT "buff: %s", buffer[3]);
	return length;
}
int lkm_close (struct inode *pinode, struct file *pfile){
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	gpio_set_value(gpioLED, 0);
//	gpio_set_value(gpioLED2, 1);
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
			421/*Major Number */,
			"Simple Char Drv"/*Name of Driver*/,
			&lkm_file_operations/*POINTER to File Operations*/);
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	gpio_request(gpioLED, "sysfs");          // gpioLED is hardcoded to 49, request it
	gpio_direction_output(gpioLED, 0);   // Set the gpio to be in output mode and on
	// gpio_set_value(gpioLED, ledOn);          // Not required as set by line above
	gpio_export(gpioLED, false);             // Causes gpio49 to appear in /sys/class/gpio
	//irqNumber = gpio_to_irq(gpioButton);
	//gpio_direction_output(gpioLED, 1);   // Set the gpio to be in output mode and on
	//gpio_export(gpioLED, false);
	//gpio_set_value(gpioLED, 1);
//	gpio_request(gpioLED2, "sysfs");          // gpioLED is hardcoded to 49, request it
//	gpio_direction_output(gpioLED2, 0);   // Set the gpio to be in output mode and on
	// gpio_set_value(gpioLED, ledOn);          // Not required as set by line above
//	gpio_export(gpioLED2, false);             // Causes gpio49 to appear in /sys/class/gpio
	return 0;
}
void lkm_demo_exit(void)
{
	//We must unregister the Char. device driver:
	//gpio_set_value(gpioLED, 0);
	//gpio_free(gpioLED);
	unregister_chrdev(421, "Simple Char Drv");
	gpio_set_value(gpioLED, 0);              // Turn the LED off, makes it clear the device was unloaded
	gpio_unexport(gpioLED);                  // Unexport the LED GPIO
	//free_irq(irqNumber, NULL);               // Free the IRQ number, no *dev_id required in this case
	//gpio_unexport(gpioButton);               // Unexport the Button GPIO
	gpio_free(gpioLED);                      // Free the LED GPIO
//	gpio_set_value(gpioLED2, 0);              // Turn the LED off, makes it clear the device was unloaded
//	gpio_unexport(gpioLED2);                  // Unexport the LED GPIO
//	gpio_free(gpioLED2);                      // Free the LED GPIO
	//gpio_free(gpioButton);
	unregister_chrdev(421, "Simple Char Drv");
	//free_irq(irqNumber, NULL);
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	//no return statement!
}
module_init(lkm_demo_init);
module_exit(lkm_demo_exit);
/*
   static irq_handler_t lkm_demo_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs){
   return (irq_handler_t) IRQ_HANDLED;      // Announce that the IRQ has been handled correctly
   }
   */