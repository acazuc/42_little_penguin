#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include "file_id.h"
#include "file_jiffies.h"
#include "file_foo.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aurelien Cazuc <acazuc@student.42.fr>");
MODULE_DESCRIPTION("Debugfs test module");

static struct dentry *file_fortytwo;
static struct dentry *file_id;
static struct dentry *file_jiffies;
static struct dentry *file_foo;

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

	if (!(file_fortytwo = debugfs_create_dir("fortytwo"
					, arch_debugfs_dir)))
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
	printk(KERN_INFO "fortytwo, id, jiffies & foo files created\n");
	ret = 0;
	goto end;

err_jiffies:
	debugfs_remove(file_jiffies);
err_id:
	debugfs_remove(file_id);
err_fortytwo:
	debugfs_remove(file_fortytwo);
err:
	ret = -EINVAL;
end:
	return ret;
}

void cleanup_module(void)
{
	debugfs_remove_recursive(file_fortytwo);
	pr_info("Cleaning up module.\n");
}
