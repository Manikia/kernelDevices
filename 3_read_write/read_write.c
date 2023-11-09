#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

/*Meta information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Monika Tutorial 4 GNU/Linux");
MODULE_DESCRIPTION("Registers a device number and implements some callback functions");

/*Buffer for data*/
static char buffer[255];
static int buffer_pointer;

/**
 * @brief read data out of the buffer
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs){
	int to_copy, not_copied, delta;

	/*Get amount of data to copy*/
	to_copy = min(count, buffer_pointer);

	/*copy data to user*/
	not_copied = copy_to_user(user_buffer, buffer, to_copy);
	
	/*calcualte data*/
	delta = to_copy - not_copied;

	return delta;
}

/*write data to buffer*/
static ssize_t driver_write(struct file *File,const char *user_buffer, size_t count, loff_t *offs){
        int to_copy, not_copied, delta;

        /*Get amount of data to copy*/
        to_copy = min(count, sizeof(buffer));

        /*copy data to user*/
        not_copied = copy_from_user(buffer, user_buffer, to_copy);
	buffer_pointer = to_copy;
	
        /*calcualte data*/
        delta = to_copy - not_copied;

        return delta;
}

/*
 * @brief this function is called when the dvice file is opened
 */

static int driver_open(struct inode *device_file, struct file *instance){
	printk("dev_nr - open was called! \n");
	return 0;
}

/*
 * @brief this function is called when the dvice file is closed
 */

static int driver_close(struct inode *device_file, struct file *instance){
        printk("dev_nr - close was called! \n");
        return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.read = driver_read,
	.write = driver_write
};

#define MYMAJOR 91

/**
 * @brief this function is called whent he module is loaded into the kernel
 */
static int __init ModuleInit(void){
	int retval;
	printk("Hello, kernel!\n");
	/* register device number */
	retval = register_chrdev(MYMAJOR, "my_dev_nr", &fops);
	if(retval == 0){
		/*register if the device number is free by checking: cat /proc/devices*/
		printk("dev_nr - registered Device number Major: %d, Minor %d\n", MYMAJOR, 0);
	}
	else if (retval > 0){
		printk("dev_nr - registerd Device number Major: %d, Minor: %d\n", retval>>20, retval&0xfffff);
	}
	else {
		printk("Could not register device number\n");
		return -1;
	}
	return 0;
}
/**
 * @brief this function is called when the module is removed from the kernel
 */
static void __exit ModuleExit(void){
	unregister_chrdev(MYMAJOR, "my_dev_nr");
	printk("Goodbye, kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);


