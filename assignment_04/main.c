#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/usb/input.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aurelien Cazuc <acazuc@student.42.fr>");
MODULE_DESCRIPTION("Basic module of usb callback");

static int probe_callback(struct usb_interface *interface,
		const struct usb_device_id *id)
{
	pr_info("USB keyboard connected\n");
	return 0;
}
EXPORT_SYMBOL(probe_callback);

static void disconnect_callback(struct usb_interface *interface)
{
	pr_info("USB keyboard disconnected\n");
}
EXPORT_SYMBOL(disconnect_callback);

static struct usb_device_id id_table = {
	
}

static struct usb_driver main_usb = {
	.owner = THIS_MODULE,
	.name = "main_usb_module",
	.probe = &probe_callback,
	.disconnect = &disconnect_callback,
	.id_table = id_table
};

int init_module(void)
{
	int result;

	pr_info("Hello world !\n");
	result = usb_register(&main_usb);
	if (result)
		pr_info("usb_register failed");
	return result;
}

void cleanup_module(void)
{
	pr_info("Cleaning up module.\n");
	usb_deregister(&main_usb);
}
