#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

static int itron_probe(struct usb_interface *interface,
		      const struct usb_device_id *id)
{
	printk(KERN_INFO "Itron mouse (%04X:%04X) plugged\n",
	       id->idVendor,
	       id->idProduct);
	return 0;
}

static void itron_disconnect(struct usb_interface *interface)
{
	printk(KERN_INFO "Itron mouse removed\n");
}

static struct usb_device_id itron_table[] =
{
	// { USB_DEVICE_INTERFACE_NUMBER(0x195d, 0x1016, 02) }, // ASUS CLAW
	{ USB_DEVICE(0x195d, 0x1016) }, // ASUS CLAW
	{ USB_DEVICE(0x195d, 0x1017) }, // SAGITTA
	{ USB_DEVICE(0x12cf, 0x700b) }, // COUGAR 700M
	{} /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, itron_table);

static struct usb_driver itron_driver =
{
	.name = "itron_driver",
	.id_table = itron_table,
	.probe = itron_probe,
	.disconnect = itron_disconnect,
};

static int __init itron_init(void)
{
	return usb_register(&itron_driver);
}

static void __exit itron_exit(void)
{
	usb_deregister(&itron_driver);
}

module_init(itron_init);
module_exit(itron_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tristan Šneider <tristan.sneider@gmail.com>");
MODULE_DESCRIPTION("Itron mice driver");
