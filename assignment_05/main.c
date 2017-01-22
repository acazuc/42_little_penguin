#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aurelien Cazuc <acazuc@student.42.fr>");
MODULE_DESCRIPTION("Basic module");

char login[7] = "acazuc";

static int ft_open(struct inode *inode, struct file *fp)
{
	printk(KERN_DEBUG "ft_open\n");
	return 0;
}
EXPORT_SYMBOL(ft_open);

static ssize_t ft_read(struct file *fp, char __user *data, size_t len
		, loff_t *off)
{
	int available;
	int result;

	if (*off >= 6)
	{
		result = 0;
		goto end;
	}
	if (len < 6 - *off)
		available = len;
	else
		available = 6 - *off;
	if (copy_to_user(data, login + *off, available))
	{
		result = -EINVAL;
	}
	else
	{
		result = available;
		*off += result;
	}

end:
	return result;
}
EXPORT_SYMBOL(ft_read);

static ssize_t ft_write(struct file *fp, const char __user *data, size_t len
		, loff_t *off)
{
	int result;

	if (len != 6)
	{
		result = -EINVAL;
		goto end;
	}
	if (memcmp(data, "acazuc", 6))
	{
		//result = -EINVAL;
		result = -EFAULT;
		goto end;
	}
	result = 6;

end:
	return result;
}
EXPORT_SYMBOL(ft_write);

static int ft_release(struct inode *inode, struct file *fp)
{
	printk(KERN_DEBUG "ft_release\n");
	return 0;
}
EXPORT_SYMBOL(ft_release);

struct file_operations ops = {
	.owner = THIS_MODULE,
	.open = &ft_open,
	.read = &ft_read,
	.write = &ft_write,
	.release = &ft_release
};

struct miscdevice ft_device = {
	MISC_DYNAMIC_MINOR,
	"fortytwo",
	&ops
};

int init_module(void)
{
	int	result;

	result = misc_register(&ft_device);
	if (result)
		printk(KERN_ERR "Failed to register file\n");
	else
		printk(KERN_INFO "/dev/fortytwo created\n");
	return result;
}

void cleanup_module(void)
{
	misc_deregister(&ft_device);
	pr_info("Destroying character device.\n");
}
