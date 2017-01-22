#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aurelien Cazuc <acazuc@student.42.fr>");
MODULE_DESCRIPTION("Debugfs test module");

static char login[7] = "acazuc";

static void *foo_data = NULL;
static size_t foo_len = 0;
DEFINE_SEMAPHORE(foo_lock);

static struct dentry *file_fortytwo;
static struct dentry *file_id;
static struct dentry *file_jiffies;
static struct dentry *file_foo;

static ssize_t id_write(struct file *fp, const char __user *data, size_t len
		, loff_t *off)
{
	int result;

	if (len != 6)
	{
		result = EINVAL;
		goto end;
	}
	if (memcmp(data, login, 6))
	{
		result = -EINVAL;
		goto end;
	}
	result = 6;

end:
	return result;
}
EXPORT_SYMBOL(id_write);

static ssize_t id_read(struct file *fp, char __user *data, size_t len
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
EXPORT_SYMBOL(id_read);

static ssize_t jiffies_read(struct file *fp, char __user *data, size_t len
		, loff_t *off)
{
	unsigned long count;
	int result;

	count = jiffies;
	if (*off == 8)
	{
		result = 0;
		goto end;
	}
	if (len < 8)
	{
		result = -EINVAL;
		goto end;
	}
	result = copy_to_user(data, &count, 8);
	if (!result)
	{
		*off = 8;
		result = 8;
	}

end:
	return result;
}
EXPORT_SYMBOL(jiffies_read);

static ssize_t foo_write(struct file *fp, const char __user *data
		, size_t len, loff_t *off)
{
	int result;
	int available;

	down(&foo_lock);
	if (foo_len >= PAGE_SIZE)
	{
		result = 0;
		goto end;
	}
	if (len < PAGE_SIZE - foo_len)
		available = len;
	else
		available = PAGE_SIZE - foo_len;
	result = copy_from_user(foo_data + foo_len, data, available);
	if (result)
		goto end;
	foo_len += available;
	*off = foo_len;
	result = available;

end:
	up(&foo_lock);
	return result;
}
EXPORT_SYMBOL(foo_write);

static ssize_t foo_read(struct file *fp, char __user *data, size_t len,
		loff_t *off)
{
	int available;
	int result;

	down(&foo_lock);
	if (*off >= foo_len)
	{
		result = 0;
		goto end;
	}
	if (len < foo_len - *off)
		available = len;
	else
		available = foo_len - *off;
	result = copy_to_user(data + *off, data, available);
	if (result)
		goto end;
	*off += available;
	result = available;

end:
	up(&foo_lock);
	return result;
}
EXPORT_SYMBOL(foo_read);

static struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.write = &id_write,
	.read = &id_read,
};

static struct file_operations jiffies_fops = {
	.owner = THIS_MODULE,
	.read = &jiffies_read
};

static struct file_operations foo_fops = {
	.owner = THIS_MODULE,
	.write = &foo_write,
	.read = &foo_read
};

int init_module(void)
{
	int ret;

	ret = 0;
	if (!(file_fortytwo = debugfs_create_dir("fortyfor", NULL)))
	{
		printk(KERN_ERR "Failed to create fortytwo dir\n");
		goto err;
	}
	if (!(file_id = debugfs_create_file("id", 0666, file_fortytwo
					, NULL, &id_fops)))
	{
		printk(KERN_ERR "Failed to create id file\n");
		goto err_fortytwo;
	}
	if (!(file_jiffies = debugfs_create_file("jiffies", 0444
					, file_fortytwo, NULL
					, &jiffies_fops)))
	{
		printk(KERN_ERR "Failed to create jiffies file\n");
		goto err_id;
	}
	if (!(file_foo = debugfs_create_file("foo", 0644, file_fortytwo
					, NULL, &foo_fops)))
	{
		printk(KERN_ERR "Failed to create foo file\n");
		goto err_jiffies;
	}
	if (!(foo_data = kmalloc(PAGE_SIZE, GFP_KERNEL)))
	{
		printk(KERN_ERR "Failed to allocate page of memory\n");
		goto err_foo;
	}
	printk(KERN_INFO "fortytwo, id, jiffies & foo files created\n");
	goto end;

err_foo:
	debugfs_remove(file_foo);
err_jiffies:
	debugfs_remove(file_jiffies);
err_id:
	debugfs_remove(file_id);
err_fortytwo:
	debugfs_remove(file_fortytwo);
err:
	ret = -EACCES;
end:
	return ret;
}

void cleanup_module(void)
{
	debugfs_remove_recursive(file_fortytwo);
	kfree(foo_data);
	printk(KERN_INFO "Cleaning up module.\n");
}
