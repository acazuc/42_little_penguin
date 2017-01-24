#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/mount.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aurelien Cazuc <acazuc@student.42.fr>");
MODULE_DESCRIPTION("/proc/mymounts module");

static struct proc_dir_entry *mymounts;

static ssize_t file_read(struct file *fp, char __user *data, size_t len
		, loff_t *off)
{
	return 0;
}

static struct file_operations mymounts_fops = {
	.owner = THIS_MODULE,
	.read = &file_read
};

int init_module(void)
{
	int ret;

	mymounts = proc_create("mymounts", 0444, NULL, &mymounts_fops);
	if (!mymounts) {
		printk(KERN_ERR "/proc/mymounts creation failed\n");
		ret = -ENOMEM;
		goto end;
	}
	printk(KERN_INFO "/proc/mymounts created\n");
	ret = 0;

end:
	return ret;
}

void cleanup_module(void)
{
	remove_proc_entry("mymounts", NULL);
	printk(KERN_INFO "/proc/mymounts removed\n");
}
