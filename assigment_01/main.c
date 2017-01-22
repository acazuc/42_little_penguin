#include <linux/module.h>
#include <linux/kernel.h>

int __init init_module(void)
{
	pr_info("Hello world !\n");
	return 0;
}

void cleanup_module(void)
{
	pr_info("Cleaning up module.\n");
}
