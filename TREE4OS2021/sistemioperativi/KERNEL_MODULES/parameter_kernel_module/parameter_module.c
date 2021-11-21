/*
As of Linux 2.4, you can rename the init and cleanup functions of your modules; they no longer have to be called init_module() and cleanup_module() respectively. This is done with the module_init() and module_exit() macros. These macros are defined in linux/init.h. The only caveat is that your init and cleanup functions must be defined before calling the macros, otherwise you'll get compilation errors
*/

#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/init.h>         /* Needed for the macros */

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("module example that manages parameters, operating systems course, 2015/16");
MODULE_AUTHOR("Vittorio \"vic\" Ghini");

// These global variables can be set with command line arguments when you insmod
// the module in. 
//
static u8             mybyte = 'A';
static unsigned short myshort = 1;
static int            myint = 20;
static long           mylong = 9999;
static char           *mystring = "blah";
static int            myintArray[4] = { 0, 420, 77, 666 };
/* numero di elementi dell'array inizializzati da riga di comando
   non viene modificato se non passo valori per questo argomento */
static unsigned int myintArray_counter=0;


/* old initialization mode
MODULE_PARAM(mybyte, "b");
MODULE_PARAM(myshort, "h");
MODULE_PARAM(myint, "i");
MODULE_PARAM(mylong, "l");
MODULE_PARAM(mystring, "s");
MODULE_PARAM(myintArray, "1-2i");
*/

/*  Now we're actually setting the mechanism up -- making the variables command
 *  line arguments rather than just a bunch of global variables.
 */

/**
 * module_param - typesafe helper for a module/cmdline parameter
 * @value: the variable to alter, and exposed parameter name.
 * @type: the type of the parameter
 * @perm: visibility in sysfs.
 *
 * @value becomes the module parameter, or (prefixed by KBUILD_MODNAME and a
 * ".") the kernel commandline parameter.  Note that - is changed to _, so
 * the user can use "foo-bar=1" even for variable "foo_bar".
 *
 * @perm is 0 if the the variable is not to appear in sysfs, or 0444
 * for world-readable, 0644 for root-writable, etc.  Note that if it
 * is writable, you may need to use kernel_param_lock() around
 * accesses (esp. charp, which can be kfreed when it changes).
 *
 * The @type is simply pasted to refer to a param_ops_##type and a
 * param_check_##type: for convenience many standard types are provided but
 * you can create your own by defining those variables.
 *
 * Standard types are:
 *      byte, short, ushort, int, uint, long, ulong
 *      charp: a character pointer
 *      bool: a bool, values 0/1, y/n, Y/N.
 *      invbool: the above, only sense-reversed (N = true).
 */

 /* NOTE:  Rusty Russell said:
  The kernel param code currently uses a single mutex to protect
  modification of any and all kernel params.  While this generally works,
  there is one specific problem with it; a module callback function
  cannot safely load another module, i.e. with request_module() or even
  with indirect calls such as crypto_has_alg().  If the module to be
  loaded has any of its params configured (e.g. with a /etc/modprobe.d/\*
  config file), then the attempt will result in a deadlock between the
  first module param callback waiting for modprobe, and modprobe trying to
  lock the single kernel param mutex to set the new module's param.

  This fixes that by using per-module mutexes, so that each individual module
  is protected against concurrent changes in its own kernel params, but is
  not blocked by changes to other module params.
*/
/* see  /sys/module/parameter_module/parameters/   */
module_param(mybyte,     byte,  0644 );
module_param(myshort,    short, 0644 );
module_param(myint,      int,   0644 );
module_param(mylong,     long,  0644 );
module_param(mystring,   charp, 0644 );
module_param_array(myintArray, int, &myintArray_counter, 0644);


/* Always use a MODULE_PARM_DESC() for each MODULE_PARM(). */
MODULE_PARM_DESC(mybyte,   "This u8 is for example purpouse.");
MODULE_PARM_DESC(myshort,  "This unsigned short  is for example purpouse only.");
MODULE_PARM_DESC(myint,    "This int is for example purpouse only.");
MODULE_PARM_DESC(mylong,   "This long is for example purpouse only.");
MODULE_PARM_DESC(mystring, "This charptr is for example purpouse only.");
MODULE_PARM_DESC(myintArray, "This int array  is for example purpouse only.");

/*  module loading example:
sudo insmod parameter_module.ko mystring="vaf" mybyte=255 mystring="MERDA" myintArray="33,44,55,66,77,88"
sudo insmod parameter_module.ko mystring="vaf" mybyte=255 mystring="MERDA" myintArray="33"
sudo insmod parameter_module.ko mystring="vaf" mybyte=255 mystring="MERDA" myintArray="33,44"
sudo insmod parameter_module.ko mystring="vaf" mystring="MERDA" mylong="991" myintArray="33,44"
sudo insmod parameter_module.ko mystring="vaf" mystring="MERDA" mylong="991" myintArray="pappa"

After loading, the module will appear at  
	/sys/module/parameter_module/
and the parameters will be available for reading at
	/sys/module/parameter_module/parameters/
*/

static int __init parameter_module_start(void)
{
	printk(KERN_INFO "Loading parameter module...\n");
	printk(KERN_INFO "parameter_module says: Hello world\n");
	
	kernel_param_lock(THIS_MODULE);
	printk(KERN_ALERT "mybyte is an 8 bit integer: %i\n", mybyte);
	printk(KERN_ALERT "myshort is a short integer: %hi\n", myshort);
	printk(KERN_ALERT "myint is an integer: %i\n", myint);
	printk(KERN_ALERT "mylong is a long integer: %li\n", mylong);
	printk(KERN_ALERT "mystring is a string: %s\n", mystring);
	printk(KERN_ALERT "myintArray is %i %i %i %i\n", myintArray[0], myintArray[1],
							 myintArray[2], myintArray[3] );
	printk(KERN_ALERT "myintArray_counter is %i \n", myintArray_counter );
	kernel_param_unlock(THIS_MODULE);

	return 0;
}

static void __exit parameter_module_end(void)
{
	printk(KERN_INFO "parameter_module says: Goodbye Mr.\n");
}
module_init(parameter_module_start);
module_exit(parameter_module_end);

