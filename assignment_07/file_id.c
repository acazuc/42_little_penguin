#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/uaccess.h>

ssize_t id_write(struct file *fp, const char __user *data, size_t len
		, loff_t *off)
{
	return 0;
}
EXPORT_SYMBOL(id_write);

ssize_t id_read(struct file *fp, char __user *data, size_t len, loff_t *off)
{
	return 0;
}
EXPORT_SYMBOL(id_read);
