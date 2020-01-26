一、
[driver\usb\core\usb.c]
struct bus_type usb_bus_type = {
	.name =		"usb",
	.match =	usb_device_match,
	.uevent =	usb_uevent,
#ifdef CONFIG_USB_SUSPEND
	.pm =		&usb_bus_pm_ops,
#endif
};


struct usb_driver usbfs_driver = {
	.name =		"usbfs",
	.probe =	driver_probe,
	.disconnect =	driver_disconnect,
	.suspend =	driver_suspend,
	.resume =	driver_resume,
};

struct usb_device_driver usb_generic_driver = {
	.name =	"usb",
	.probe = generic_probe,
	.disconnect = generic_disconnect,
#ifdef	CONFIG_PM
	.suspend = generic_suspend,
	.resume = generic_resume,
#endif
	.supports_autosuspend = 1,
};

/*
 * Init
 */
static int __init usb_init(void)
{
	int retval;
	if (nousb) {
		pr_info("%s: USB support disabled\n", usbcore_name);
		return 0;
	}

	retval = usb_debugfs_init();
	if (retval)
		goto out;

	retval = bus_register(&usb_bus_type);
	if (retval)
		goto bus_register_failed;
	retval = bus_register_notifier(&usb_bus_type, &usb_bus_nb);
	if (retval)
		goto bus_notifier_failed;
	retval = usb_major_init();
	if (retval)
		goto major_init_failed;
	retval = usb_register(&usbfs_driver);
	if (retval)
		goto driver_register_failed;
	retval = usb_devio_init();
	if (retval)
		goto usb_devio_init_failed;
	retval = usbfs_init();
	if (retval)
		goto fs_init_failed;
	retval = usb_hub_init();
	if (retval)
		goto hub_init_failed;
	retval = usb_register_device_driver(&usb_generic_driver, THIS_MODULE);
	if (!retval)
		goto out;

	usb_hub_cleanup();
hub_init_failed:
	usbfs_cleanup();
fs_init_failed:
	usb_devio_cleanup();
usb_devio_init_failed:
	usb_deregister(&usbfs_driver);
driver_register_failed:
	usb_major_cleanup();
major_init_failed:
	bus_unregister_notifier(&usb_bus_type, &usb_bus_nb);
bus_notifier_failed:
	bus_unregister(&usb_bus_type);
bus_register_failed:
	usb_debugfs_cleanup();
out:
	return retval;
}

1、最优先最基础的，注册总线，表示这是基于usb总线，bus_register(&usb_bus_type)。
2、