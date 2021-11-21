/*
As of Linux 2.4, you can rename the init and cleanup functions of your modules; they no longer have to be called init_module() and cleanup_module() respectively. This is done with the module_init() and module_exit() macros. These macros are defined in linux/init.h. The only caveat is that your init and cleanup functions must be defined before calling the macros, otherwise you'll get compilation errors
*/

#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/init.h>         /* Needed for the macros */

static int __init hello_module_start(void)
{
	printk(KERN_INFO "Loading hello module...\n");
	printk(KERN_INFO "Hello world\n");
	return 0;
}
static void __exit hello_module_end(void)
{
	printk(KERN_INFO "Goodbye Mr.\n");
}
module_init(hello_module_start);
module_exit(hello_module_end);

