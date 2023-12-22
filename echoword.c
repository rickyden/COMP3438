#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#define DRIVER_NAME "echoword_driver"
#define N_D 10
#define S_N 1

static dev_t devno;
static int major;
static struct cdev form;
static char msg[256];

static ssize_t read_impl(struct file * fp, char __user *buf, size_t max_count, loff_t * offset){
	int num = 0;
	int ret = 0;
	int strlength =	strlen(msg);
	if (strlength < max_count) {
		num = strlength;
	} else {
		num = max_count;
	}
	ret = copy_to_user(buf, msg, num);
	if (ret) {
		printk("Fail to copy data from the kernel space to user space.\n");
		return ret;
	}
	return num;			
}

static ssize_t write_impl(struct file * fp, const char __user *buf, size_t count, loff_t *offset){
	int ret;
	ret = copy_from_user(&msg, buf, sizeof(msg));
	if(ret){
		printk("Fail to copy from user space.\n");
	}
	printk("Echoed from kernal: %s\n", msg);
}

static int open_impl(struct inode *pinode, struct file *pf){ 
	printk("Device " DRIVER_NAME " open.\n");
	return 0;	
}

static int release_impl(struct inode *pinode, struct file *pf){
	printk("Device " DRIVER_NAME " release.\n");
	return 0;
}

static struct file_operations fops = {
	owner: THIS_MODULE,
	read: read_impl,
	write: write_impl,
	open: open_impl,
	release: release_impl,
};

static int __init wordecho_init(void) {
	int ret;
	ret = alloc_chrdev_region(&devno, S_N, N_D, DRIVER_NAME);
	if (ret < 0) {
		printk("Device " DRIVER_NAME " cannot get major number.\n");
		return ret;
	}
	major = MAJOR(devno);
	printk("Device " DRIVER_NAME " initialized (Major number = %d).\n", major);

	cdev_init(&form, &fops);
	form.owner = THIS_MODULE;
	ret = cdev_add(&form, devno, N_D);
	if (ret < 0) {
		printk("Device " DRIVER_NAME " register fail.\n");
		return ret;
	}
	return 0;
}

static void __exit wordecho_exit(void) {
	cdev_del(&form);
	unregister_chrdev_region(devno, N_D);
	printk("Device " DRIVER_NAME " unloaded.\n");
}

module_init(wordecho_init);
moudle_exit(wordecho_exit);


