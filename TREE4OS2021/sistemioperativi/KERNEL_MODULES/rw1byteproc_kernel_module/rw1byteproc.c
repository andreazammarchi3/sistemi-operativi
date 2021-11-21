/*
 * Sample byte stream implementation for kernel version > 3.10
 *
 * Vittorio vic Ghini  <vittorio.ghini@unibo.it>
 * 2015-12-08
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/mutex.h>
#include <linux/kfifo.h>
#include <asm/uaccess.h>

/*
 * This module shows how to create a byte buffer
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vittorio \"vic\" Ghini");

/* name of the proc entry */
#define	PROC_FIFO	"rw1byte"

/* lock for procfs read access */
static DEFINE_MUTEX(read_lock);

/* lock for procfs write access */
static DEFINE_MUTEX(write_lock);

#define BUFFERSIZE 256

static unsigned char circular_buffer[BUFFERSIZE];
static int first=0, num=0;

static ssize_t fifo_write(struct file *file, const char __user *userbuf,
						size_t count, loff_t *ppos)
{
	int noncopiati, i, j;
	unsigned int copied;
	unsigned char buffer[BUFFERSIZE];

	if( (BUFFERSIZE-num)<count ) {
		count=(BUFFERSIZE-num);
	}

	if (mutex_lock_interruptible(&write_lock))
		return -ERESTARTSYS;

	/* Returns number of bytes that could not be copied. On success, this will be zero. */
	noncopiati = copy_from_user( buffer, userbuf, count );

	mutex_unlock(&write_lock);

	copied=count-noncopiati;

	for( j=0,i=(first+num)%BUFFERSIZE; j<copied; j++, i=(i+1)%BUFFERSIZE ) {
		circular_buffer[i] = buffer[j];
	}
	num+=copied;
	
	if( copied>0 )
		return( copied );
	else
		return( 0 );
	
	// return ret ? ret : copied;
}

static ssize_t fifo_read(struct file *file, char __user *userbuf,
						size_t count, loff_t *ppos)
{
	int noncopiati, i, j;
	unsigned int copied;
	unsigned char buffer[BUFFERSIZE];

	if( count>num ) {
		count=num;
	}
	if( count>BUFFERSIZE ) {
		count=BUFFERSIZE;
	}
	for( j=0,i=first; j<count; j++, i=(i+1)%BUFFERSIZE ) {
		buffer[j] = circular_buffer[i];
	}

	if (mutex_lock_interruptible(&read_lock))
		return -ERESTARTSYS;

	/* Returns number of bytes that could not be copied. On success, this will be zero. */
	noncopiati = copy_to_user( userbuf, buffer, count );

	mutex_unlock(&read_lock);

	copied=count-noncopiati;

	num-=copied;
	first=(first+copied)%BUFFERSIZE;
	
	if( copied>0 )
		return( copied );
	else
		return( 0 );
}

static const struct file_operations fifo_fops = {
	.owner		= THIS_MODULE,
	.read		= fifo_read,
	.write		= fifo_write,
	.llseek		= noop_llseek,
};

static int __init example_init(void)
{
	if (proc_create(PROC_FIFO, 0777, NULL, &fifo_fops) == NULL) {
		return -ENOMEM;
	}
	return 0;
}

static void __exit example_exit(void)
{
	remove_proc_entry(PROC_FIFO, NULL);
}

module_init(example_init);
module_exit(example_exit);
