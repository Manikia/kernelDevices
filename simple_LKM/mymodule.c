#include <linux/module.h>
#include <linux/init.h>

/*Meta information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Monika Tutorial 4 GNU/Linux");
MODULE_DESCRIPTION("Creating hello world LKM");

/**
 * @brief this function is called whent he module is loaded into the kernel
 */
static int __init ModuleInit(void){
	printk("Hello, kernel!\n");
	return 0;
}
/**
 * @brief this function is called when the module is removed from the kernel
 */
static void __exit ModuleExit(void){
	printk("Goodbye, kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);


