#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aurelien Cazuc <acazuc@student.42.fr>");
MODULE_DESCRIPTION("Basic module");

int init_module(void)
{
	pr_info("Hello world !\n");
	return 0;
}

void cleanup_module(void)
{
	pr_info("Cleaning up module.\n");
}
