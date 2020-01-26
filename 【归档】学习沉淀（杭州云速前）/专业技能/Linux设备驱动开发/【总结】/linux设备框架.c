[driver\base\base.h]
struct bus_type {
	const char		*name;
	struct bus_attribute	*bus_attrs;
	struct device_attribute	*dev_attrs;
	struct driver_attribute	*drv_attrs;

	int (*match)(struct device *dev, struct device_driver *drv);
	int (*uevent)(struct device *dev, struct kobj_uevent_env *env);
	int (*probe)(struct device *dev);
	int (*remove)(struct device *dev);
	void (*shutdown)(struct device *dev);

	int (*suspend)(struct device *dev, pm_message_t state);
	int (*resume)(struct device *dev);

	const struct dev_pm_ops *pm;

	struct bus_type_private *p;
};

/**
 * struct bus_type_private - structure to hold the private to the driver core portions of the bus_type structure.
 *
 * @subsys - the struct kset that defines this bus.  This is the main kobject
 * @drivers_kset - the list of drivers associated with this bus
 * @devices_kset - the list of devices associated with this bus
 * @klist_devices - the klist to iterate over the @devices_kset
 * @klist_drivers - the klist to iterate over the @drivers_kset
 * @bus_notifier - the bus notifier list for anything that cares about things
 * on this bus.
 * @bus - pointer back to the struct bus_type that this structure is associated
 * with.
 *
 * This structure is the one that is the actual kobject allowing struct
 * bus_type to be statically allocated safely.  Nothing outside of the driver
 * core should ever touch these fields.
 */
struct bus_type_private {
	struct kset subsys;
	struct kset *drivers_kset;
	struct kset *devices_kset;
	struct klist klist_devices;
	struct klist klist_drivers;
	struct blocking_notifier_head bus_notifier;
	unsigned int drivers_autoprobe:1;
	struct bus_type *bus;
};
klist_devices表示该总线类型上的挂载设备。
klist_drivers表示该总线类型上的挂载驱动。

函数bus_add_device向总线上添加设备，函数原型int bus_add_device(struct device *dev)。设备框架模型中接口函数为device_add，调用栈为device_add->bus_add_device。
函数bus_add_driver向总线上添加驱动，函数原型int bus_add_driver(struct device_driver *drv)。设备框架模型中接口函数为driver_register，调用栈为driver_register->bus_add_driver。

函数bus_for_each_dev轮询总线上的设备，函数原型int bus_for_each_dev(struct bus_type *bus, struct device *start,
		     void *data, int (*fn)(struct device *, void *))。
函数bus_for_each_drv轮询总线上的驱动，函数原型int bus_for_each_drv(struct bus_type *bus, struct device_driver *start,
		     void *data, int (*fn)(struct device_driver *, void *))。


[driver/base/bus.c]
/**
 * bus_register - register a bus with the system.
 * @bus: bus.
 *
 * Once we have that, we registered the bus with the kobject
 * infrastructure, then register the children subsystems it has:
 * the devices and drivers that belong to the bus.
 */
int bus_register(struct bus_type *bus)
{
	int retval;
	struct bus_type_private *priv;

	priv = kzalloc(sizeof(struct bus_type_private), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->bus = bus;
	bus->p = priv;

	BLOCKING_INIT_NOTIFIER_HEAD(&priv->bus_notifier);

	retval = kobject_set_name(&priv->subsys.kobj, "%s", bus->name);
	if (retval)
		goto out;

	priv->subsys.kobj.kset = bus_kset;
	priv->subsys.kobj.ktype = &bus_ktype;
	priv->drivers_autoprobe = 1;

	retval = kset_register(&priv->subsys);
	if (retval)
		goto out;

	retval = bus_create_file(bus, &bus_attr_uevent);
	if (retval)
		goto bus_uevent_fail;

	priv->devices_kset = kset_create_and_add("devices", NULL,
						 &priv->subsys.kobj);
	if (!priv->devices_kset) {
		retval = -ENOMEM;
		goto bus_devices_fail;
	}

	priv->drivers_kset = kset_create_and_add("drivers", NULL,
						 &priv->subsys.kobj);
	if (!priv->drivers_kset) {
		retval = -ENOMEM;
		goto bus_drivers_fail;
	}

	klist_init(&priv->klist_devices, klist_devices_get, klist_devices_put);
	klist_init(&priv->klist_drivers, NULL, NULL);

	retval = add_probe_files(bus);
	if (retval)
		goto bus_probe_files_fail;

	retval = bus_add_attrs(bus);
	if (retval)
		goto bus_attrs_fail;

	pr_debug("bus: '%s': registered\n", bus->name);
	return 0;

bus_attrs_fail:
	remove_probe_files(bus);
bus_probe_files_fail:
	kset_unregister(bus->p->drivers_kset);
bus_drivers_fail:
	kset_unregister(bus->p->devices_kset);
bus_devices_fail:
	bus_remove_file(bus, &bus_attr_uevent);
bus_uevent_fail:
	kset_unregister(&bus->p->subsys);
out:
	kfree(bus->p);
	bus->p = NULL;
	return retval;
}
EXPORT_SYMBOL_GPL(bus_register);

1、bus_register函数可以总结为三个注册
 （1）注册bus对象自身priv->subsys，相当于在bus目录下创建了以bus->name为名字的目录。函数kset_register(&priv->subsys)进行注册，priv->subsys.kobj.parent为bus目录的kobject。
 （2）注册bus内的设备对象priv->devices_kset。相当于在bus->name目录下创建了device目录。函数kset_create_and_add("devices", NULL,&priv->subsys.kobj)进行注册，priv->devices_kset.kobj.parent为priv->subsys.kobj。
 （3）注册bus内的驱动对象priv->drivers_kset。相当于在bus->name目录下创建了driver目录。函数kset_create_and_add("drivers", NULL,&priv->subsys.kobj)进行注册，priv->drivers_kset.kobj.parent为priv->subsys.kobj
2、函数add_probe_files在bus->name目录下，创建bus_attr_drivers_probe与bus_attr_drivers_autoprobe文件，这两个文件以struct bus_attribute总线属性存在，使用bus_create_file创建文件。


提示：
1、平台总线是虚拟总线，无特定总线的片上资源向其注册。通常平台总线上的设备都是固定存在，因此在开机中就会通过函数platform_device_add来添加硬件，硬件struct platform_device *pdev也会内核中静态定义完成。注册平台驱动通过函数platform_driver_register来完成，驱动中会负责匹配对应的硬件设备。

 

举例：
[driver\i2c\i2c-core.c]

struct bus_type i2c_bus_type = {
	.name		= "i2c",
	.match		= i2c_device_match,
	.probe		= i2c_device_probe,
	.remove		= i2c_device_remove,
	.shutdown	= i2c_device_shutdown,
	.pm		= &i2c_device_pm_ops,
};
EXPORT_SYMBOL_GPL(i2c_bus_type);
static int __init i2c_init(void)
{
	int retval;

	retval = bus_register(&i2c_bus_type);
	....
}

----------------------------------------------------------------------------------------------------

[include\linux\device.h]
struct device {
	struct device		*parent;

	struct device_private	*p;

	struct kobject kobj;
	const char		*init_name; /* initial name of the device */
	struct device_type	*type;

	struct mutex		mutex;	/* mutex to synchronize calls to
					 * its driver.
					 */

	struct bus_type	*bus;		/* type of bus device is on */
	struct device_driver *driver;	/* which driver has allocated this
					   device */
	void		*platform_data;	/* Platform specific data, device
					   core doesn't touch it */
	struct dev_pm_info	power;

#ifdef CONFIG_NUMA
	int		numa_node;	/* NUMA node this device is close to */
#endif
	u64		*dma_mask;	/* dma mask (if dma'able device) */
	u64		coherent_dma_mask;/* Like dma_mask, but for
					     alloc_coherent mappings as
					     not all hardware supports
					     64 bit addresses for consistent
					     allocations such descriptors. */

	struct device_dma_parameters *dma_parms;

	struct list_head	dma_pools;	/* dma pools (if dma'ble) */

	struct dma_coherent_mem	*dma_mem; /* internal for coherent mem
					     override */
	/* arch specific additions */
	struct dev_archdata	archdata;
#ifdef CONFIG_OF
	struct device_node	*of_node;
#endif

	dev_t			devt;	/* dev_t, creates the sysfs "dev" */

	spinlock_t		devres_lock;
	struct list_head	devres_head;

	struct klist_node	knode_class;
	struct class		*class;
	const struct attribute_group **groups;	/* optional groups */

	void	(*release)(struct device *dev);
};
/**
 * struct device_private - structure to hold the private to the driver core portions of the device structure.
 *
 * @klist_children - klist containing all children of this device
 * @knode_parent - node in sibling list
 * @knode_driver - node in driver list
 * @knode_bus - node in bus list
 * @driver_data - private pointer for driver specific info.  Will turn into a
 * list soon.
 * @device - pointer back to the struct class that this structure is
 * associated with.
 *
 * Nothing outside of the driver core should ever touch these fields.
 */
struct device_private {
	struct klist klist_children;
	struct klist_node knode_parent;
	struct klist_node knode_driver;
	struct klist_node knode_bus;
	void *driver_data;
	struct device *device;
};
#define to_device_private_parent(obj)	\
	container_of(obj, struct device_private, knode_parent)
#define to_device_private_driver(obj)	\
	container_of(obj, struct device_private, knode_driver)
#define to_device_private_bus(obj)	\
	container_of(obj, struct device_private, knode_bus)
knode_bus表示设备节点加入到bus的设备列表中。
knode_driver表示设备节点加入到驱动的设备列表中。

[driver\base\core.c]
/**
 * device_add - add device to device hierarchy.
 * @dev: device.
 *
 * This is part 2 of device_register(), though may be called
 * separately _iff_ device_initialize() has been called separately.
 *
 * This adds @dev to the kobject hierarchy via kobject_add(), adds it
 * to the global and sibling lists for the device, then
 * adds it to the other relevant subsystems of the driver model.
 *
 * NOTE: _Never_ directly free @dev after calling this function, even
 * if it returned an error! Always use put_device() to give up your
 * reference instead.
 */
int device_add(struct device *dev)
{
	struct device *parent = NULL;
	struct class_interface *class_intf;
	int error = -EINVAL;

	dev = get_device(dev);
	if (!dev)
		goto done;

	if (!dev->p) {
		error = device_private_init(dev);
		if (error)
			goto done;
	}

	/*
	 * for statically allocated devices, which should all be converted
	 * some day, we need to initialize the name. We prevent reading back
	 * the name, and force the use of dev_name()
	 */
	if (dev->init_name) {
		dev_set_name(dev, "%s", dev->init_name);
		dev->init_name = NULL;
	}

	if (!dev_name(dev)) {
		error = -EINVAL;
		goto name_error;
	}

	pr_debug("device: '%s': %s\n", dev_name(dev), __func__);

	parent = get_device(dev->parent);
	setup_parent(dev, parent);

	/* use parent numa_node */
	if (parent)
		set_dev_node(dev, dev_to_node(parent));

	/* first, register with generic layer. */
	/* we require the name to be set before, and pass NULL */
	error = kobject_add(&dev->kobj, dev->kobj.parent, NULL);
	if (error)
		goto Error;

	/* notify platform of device entry */
	if (platform_notify)
		platform_notify(dev);

	error = device_create_file(dev, &uevent_attr);
	if (error)
		goto attrError;

	if (MAJOR(dev->devt)) {
		error = device_create_file(dev, &devt_attr);
		if (error)
			goto ueventattrError;

		error = device_create_sys_dev_entry(dev);
		if (error)
			goto devtattrError;

		devtmpfs_create_node(dev);
	}

	error = device_add_class_symlinks(dev);
	if (error)
		goto SymlinkError;
	error = device_add_attrs(dev);
	if (error)
		goto AttrsError;
	error = bus_add_device(dev);
	if (error)
		goto BusError;
	error = dpm_sysfs_add(dev);
	if (error)
		goto DPMError;
	device_pm_add(dev);

	/* Notify clients of device addition.  This call must come
	 * after dpm_sysf_add() and before kobject_uevent().
	 */
	if (dev->bus)
		blocking_notifier_call_chain(&dev->bus->p->bus_notifier,
					     BUS_NOTIFY_ADD_DEVICE, dev);

	kobject_uevent(&dev->kobj, KOBJ_ADD);
	bus_probe_device(dev);
	if (parent)
		klist_add_tail(&dev->p->knode_parent,
			       &parent->p->klist_children);

	if (dev->class) {
		mutex_lock(&dev->class->p->class_mutex);
		/* tie the class to the device */
		klist_add_tail(&dev->knode_class,
			       &dev->class->p->class_devices);

		/* notify any interfaces that the device is here */
		list_for_each_entry(class_intf,
				    &dev->class->p->class_interfaces, node)
			if (class_intf->add_dev)
				class_intf->add_dev(dev, class_intf);
		mutex_unlock(&dev->class->p->class_mutex);
	}
done:
	put_device(dev);
	return error;
 DPMError:
	bus_remove_device(dev);
 BusError:
	device_remove_attrs(dev);
 AttrsError:
	device_remove_class_symlinks(dev);
 SymlinkError:
	if (MAJOR(dev->devt))
		devtmpfs_delete_node(dev);
	if (MAJOR(dev->devt))
		device_remove_sys_dev_entry(dev);
 devtattrError:
	if (MAJOR(dev->devt))
		device_remove_file(dev, &devt_attr);
 ueventattrError:
	device_remove_file(dev, &uevent_attr);
 attrError:
	kobject_uevent(&dev->kobj, KOBJ_REMOVE);
	kobject_del(&dev->kobj);
 Error:
	cleanup_device_parent(dev);
	if (parent)
		put_device(parent);
name_error:
	kfree(dev->p);
	dev->p = NULL;
	goto done;
}

------------------------------------------------------------------------------------------------
[include\linux\device.h]
struct device_driver {
	const char		*name;
	struct bus_type		*bus;

	struct module		*owner;
	const char		*mod_name;	/* used for built-in modules */

	bool suppress_bind_attrs;	/* disables bind/unbind via sysfs */

#if defined(CONFIG_OF)
	const struct of_device_id	*of_match_table;
#endif

	int (*probe) (struct device *dev);
	int (*remove) (struct device *dev);
	void (*shutdown) (struct device *dev);
	int (*suspend) (struct device *dev, pm_message_t state);
	int (*resume) (struct device *dev);
	const struct attribute_group **groups;

	const struct dev_pm_ops *pm;

	struct driver_private *p;
};
struct driver_private {
	struct kobject kobj;
	struct klist klist_devices;
	struct klist_node knode_bus;
	struct module_kobject *mkobj;
	struct device_driver *driver;
};
#define to_driver(obj) container_of(obj, struct driver_private, kobj)
knode_bus表示设备节点加入到bus的驱动列表中。
klist_devices表示该驱动上挂载的设备列表。

函数driver_for_each_device轮询该驱动上设备，函数原型为int driver_for_each_device(struct device_driver *drv, struct device *start,void *data, int (*fn)(struct device *, void *))，该函数也为框架接口函数。
函数driver_find_device在指定驱动上查找设备，函数原型为struct device *driver_find_device(struct device_driver *drv,struct device *start, void *data,int (*match)(struct device *dev, void *data))，该函数也为框架接口函数。

函数driver_bound将设备节点加入到驱动的设备列表上，函数原型为static void driver_bound(struct device *dev)。触发该函数有几种情况
（1）通过框架接口函数device_attach，函数原型是int device_attach(struct device *dev)，尝试将设备关联上驱动。如果参数dev->driver已制定，直接使用device_bind_driver绑定驱动，反之使用bus_for_each_drv在总线上寻找驱动进行绑定。
a、该框架接口函数被框架外部直接调用，通常发生在mcu片上资源的platform方式上（可参考rtc-at91sam9.c），设备在开机初始化arch_initcall被platform_device_register添加。驱动通过模块初始化中platform_driver_register添加。从先后顺上卡，设备添加在前，驱动添加在后，肯定无法通过device_add来关联设备与驱动了，只要在驱动中自己调用device_attach来关联设备与驱动。
b、platform_device_register中也会调用device_add来向总线添加设备，但不一定会使用device_attach来关联驱动和设备，因为需要条件bus->p->drivers_autoprobe为1。
c、在进行复位（reset），重扫描（rescan），重探针（reprobe）,重绑定（rebind），重连接(reconnect)等操作也会触发驱动和设备重新关联，调用到device_attach。

（2）通过框架接口函数driver_attach，函数原型是int driver_attach(struct device_driver *drv)，尝试需找合适设备，应用该驱动。使用bus_for_each_dev在总线上需找设备进行关联。
a.该框架接口函数被框架外部直接调用在于，driver_register中直接就让驱动和设备的关联需要条件bus->p->drivers_autoprobe=1，因此多半都是要直接调用driver_attach来关联驱动和设备的。

（3）通过框架接口函数device_add，函数原型是int device_add(struct device *dev)，在添加设备的时候，尝试为设备需找驱动，限制条件是bus->p->drivers_autoprobe为1。调用栈为device_add->bus_probe_device->device_attach。
（4）通过框架接口函数driver_register，函数原型是int driver_register(struct device_driver *drv)，在添加驱动的时候，尝试为驱动寻找合适设备，限制条件是bus->p->drivers_autoprobe为1。调用栈为driver_register->bus_add_driver->driver_attach。

提示：
1、device_add是将设备加入到总线中，device_attach是让设备关联上合适的驱动。
2、driver_register是将驱动加入到总线中，driver_attach是让驱动找到合适设备。
3、device_add中可能会调用device_attach，但需要bus->p->drivers_autoprobe=1。不然后面需要自行调用device_attach来进行关联。调用栈为device_add->bus_probe_device->device_attach
4、driver_register中可能会调用driver_attach，但需要bus->p->drivers_autoprobe=1。不然后面需要自行调用driver_attach来进行关联。
5、上述所有步骤都需要基于一条总线上，因此bus_register时间顺序上，优先级别最高，要被最先创建出来。
6、mcu里的片上资源，没有关连到具体总线上的设备与驱动，全部被关联到虚拟总线platfrom中。
7、按照时间顺序来说，通常设备添加在前，驱动添加在后。driver_register中bus->p->drivers_autoprobe=1的情况下，会调用driver_attach，如果对driver_attach返回大于0，那么驱动就不会加入到总线的驱动列表中了。但其实driver_attach总是会返回0，如下表述，函数driver_attach主要功能已经变成调用驱动的probe函数，调用栈driver_probe_device->really_probe-> drv->probe
static int __driver_attach(struct device *dev, void *data)
{
	struct device_driver *drv = data;

	/*
	 * Lock device and try to bind to it. We drop the error
	 * here and always return 0, because we need to keep trying
	 * to bind to devices and some drivers will return an error
	 * simply if it didn't support the device.
	 *
	 * driver_probe_device() will spit a warning if there
	 * is an error.
	 */

	if (!driver_match_device(drv, dev))
		return 0;

	if (dev->parent)	/* Needed for USB */
		device_lock(dev->parent);
	device_lock(dev);
	if (!dev->driver)
		driver_probe_device(drv, dev);
	device_unlock(dev);
	if (dev->parent)
		device_unlock(dev->parent);

	return 0;
}
8、设备被添加的方法有，（1）将设备信息静态写死在文件中；（2）通过扫描来获取设备信息；（3）热插拔。
9、device_attach关联设备与驱动通过使用struct bus_type *bus中match函数来进行匹配。
10、driver_attach关联设备与驱动通过probe来关联相关设备。
（1）首先查看struct bus_type *bus是否有probe函数，bus的probe中使用driver中的id_table来匹配设备（例如pcie），接着再调用driver的probe函数。调用栈为driver_attach-> bus-probe -> driver->probe
（2）总线结构中没有probe函数，直接调用驱动结构的probe函数。这时候就需要驱动结构中的probe来完成匹配工作。调用栈为driver_attach-> driver->probe



[drivers\base\driver.c]
/**
 * driver_register - register driver with bus
 * @drv: driver to register
 *
 * We pass off most of the work to the bus_add_driver() call,
 * since most of the things we have to do deal with the bus
 * structures.
 */
int driver_register(struct device_driver *drv)
{
	int ret;
	struct device_driver *other;

	BUG_ON(!drv->bus->p);

	if ((drv->bus->probe && drv->probe) ||
	    (drv->bus->remove && drv->remove) ||
	    (drv->bus->shutdown && drv->shutdown))
		printk(KERN_WARNING "Driver '%s' needs updating - please use "
			"bus_type methods\n", drv->name);

	other = driver_find(drv->name, drv->bus);
	if (other) {
		put_driver(other);
		printk(KERN_ERR "Error: Driver '%s' is already registered, "
			"aborting...\n", drv->name);
		return -EBUSY;
	}

	ret = bus_add_driver(drv);
	if (ret)
		return ret;
	ret = driver_add_groups(drv, drv->groups);
	if (ret)
		bus_remove_driver(drv);
	return ret;
}

----------------------------- class ------------------------------------------------------------
/*
 * device classes
 */
struct class {
	const char		*name;
	struct module		*owner;

	struct class_attribute		*class_attrs;
	struct device_attribute		*dev_attrs;
	struct kobject			*dev_kobj;

	int (*dev_uevent)(struct device *dev, struct kobj_uevent_env *env);
	char *(*devnode)(struct device *dev, mode_t *mode);

	void (*class_release)(struct class *class);
	void (*dev_release)(struct device *dev);

	int (*suspend)(struct device *dev, pm_message_t state);
	int (*resume)(struct device *dev);

	const struct kobj_ns_type_operations *ns_type;
	const void *(*namespace)(struct device *dev);

	const struct dev_pm_ops *pm;

	struct class_private *p;
};
类是一个设备的高层视图，它抽象出底层的实现细节。驱动程序看到的是scsi磁盘和ata磁盘，但是在类的层次上，它们都是磁盘而已。类允许用户空间使用设备所提供的功能，而不关心设备是如何连接的，以及它们是如何工作的。
几乎所有的类都显示在/sys/class目录中，可以通过ls -l /sys/class来显示。不管网络接口的类型是什么，所有的网络接口都在/sys/class/net下。输入设备可以在/sys/class/input下找到。串行设备都集中在/sys/class/tty中。出于历史的原因，有一个例外：块设备显示在/sys/block目录中。
在许多情况，类子系统是向用户空间导出信息的最好方法。当类子系统创建一个类时，它将完全拥有这个类，根本不用担心哪个模块拥有那些属性，而且信息的表示也比较友好。
为了管理类，驱动程序核心导出两个不同的接口，一是提供包含设备号的属性以便自动创建设备节点，所以udev的使用离不开类。类函数和结构与设备模型的其他部分遵循相同的模式。

struct class_attribute {
	struct attribute attr;
	ssize_t (*show)(struct class *class, struct class_attribute *attr,
			char *buf);
	ssize_t (*store)(struct class *class, struct class_attribute *attr,
			const char *buf, size_t count);
};

----------------------------- kobject事件 ---------------------------------------------------------‘
[include\linux\kobject.h]
enum kobject_action {
	KOBJ_ADD,
	KOBJ_REMOVE,
	KOBJ_CHANGE,
	KOBJ_MOVE,
	KOBJ_ONLINE,
	KOBJ_OFFLINE,
	KOBJ_MAX
};


int kobject_uevent(struct kobject *kobj, enum kobject_action action)
kobject_uevent(&dev->kobj, KOBJ_ADD);
1、函数kobject_uevent触发uevent事件消息。
2、函数device_add触发add事件，device_del触发remove事件，

int add_uevent_var(struct kobj_uevent_env *env, const char *format, ...)
上层驱动可以通过函数add_uevent_var来增加事件内容，例如usb通过函数static int usb_uevent(struct device *dev, struct kobj_uevent_env *env)来进行事件消息内容定制化，接着再触发事件上报。
struct bus_type usb_bus_type = {
	.name =		"usb",
	.match =	usb_device_match,
	.uevent =	usb_uevent,
#ifdef CONFIG_USB_SUSPEND
	.pm =		&usb_bus_pm_ops,
#endif
};



[lib/kobject_uevent.c]
/**
 * kobject_uevent_env - send an uevent with environmental data
 *
 * @action: action that is happening
 * @kobj: struct kobject that the action is happening to
 * @envp_ext: pointer to environmental data
 *
 * Returns 0 if kobject_uevent_env() is completed with success or the
 * corresponding error when it fails.
 */
int kobject_uevent_env(struct kobject *kobj, enum kobject_action action,
		       char *envp_ext[])
{
	struct kobj_uevent_env *env;
	const char *action_string = kobject_actions[action];
	const char *devpath = NULL;
	const char *subsystem;
	struct kobject *top_kobj;
	struct kset *kset;
	const struct kset_uevent_ops *uevent_ops;
	u64 seq;
	int i = 0;
	int retval = 0;
#ifdef CONFIG_NET
	struct uevent_sock *ue_sk;
#endif

	pr_debug("kobject: '%s' (%p): %s\n",
		 kobject_name(kobj), kobj, __func__);

	/* search the kset we belong to */
	top_kobj = kobj;
	while (!top_kobj->kset && top_kobj->parent)
		top_kobj = top_kobj->parent;

	if (!top_kobj->kset) {
		pr_debug("kobject: '%s' (%p): %s: attempted to send uevent "
			 "without kset!\n", kobject_name(kobj), kobj,
			 __func__);
		return -EINVAL;
	}

	kset = top_kobj->kset;
	uevent_ops = kset->uevent_ops;

	/* skip the event, if uevent_suppress is set*/
	if (kobj->uevent_suppress) {
		pr_debug("kobject: '%s' (%p): %s: uevent_suppress "
				 "caused the event to drop!\n",
				 kobject_name(kobj), kobj, __func__);
		return 0;
	}
	/* skip the event, if the filter returns zero. */
	if (uevent_ops && uevent_ops->filter)
		if (!uevent_ops->filter(kset, kobj)) {
			pr_debug("kobject: '%s' (%p): %s: filter function "
				 "caused the event to drop!\n",
				 kobject_name(kobj), kobj, __func__);
			return 0;
		}

	/* originating subsystem */
	if (uevent_ops && uevent_ops->name)
		subsystem = uevent_ops->name(kset, kobj);
	else
		subsystem = kobject_name(&kset->kobj);
	if (!subsystem) {
		pr_debug("kobject: '%s' (%p): %s: unset subsystem caused the "
			 "event to drop!\n", kobject_name(kobj), kobj,
			 __func__);
		return 0;
	}

	/* environment buffer */
	env = kzalloc(sizeof(struct kobj_uevent_env), GFP_KERNEL);
	if (!env)
		return -ENOMEM;

	/* complete object path */
	devpath = kobject_get_path(kobj, GFP_KERNEL);
	if (!devpath) {
		retval = -ENOENT;
		goto exit;
	}

	/* default keys */
	retval = add_uevent_var(env, "ACTION=%s", action_string);
	if (retval)
		goto exit;
	retval = add_uevent_var(env, "DEVPATH=%s", devpath);
	if (retval)
		goto exit;
	retval = add_uevent_var(env, "SUBSYSTEM=%s", subsystem);
	if (retval)
		goto exit;

	/* keys passed in from the caller */
	if (envp_ext) {
		for (i = 0; envp_ext[i]; i++) {
			retval = add_uevent_var(env, "%s", envp_ext[i]);
			if (retval)
				goto exit;
		}
	}

	/* let the kset specific function add its stuff */
	if (uevent_ops && uevent_ops->uevent) {
		retval = uevent_ops->uevent(kset, kobj, env);
		if (retval) {
			pr_debug("kobject: '%s' (%p): %s: uevent() returned "
				 "%d\n", kobject_name(kobj), kobj,
				 __func__, retval);
			goto exit;
		}
	}

	/*
	 * Mark "add" and "remove" events in the object to ensure proper
	 * events to userspace during automatic cleanup. If the object did
	 * send an "add" event, "remove" will automatically generated by
	 * the core, if not already done by the caller.
	 */
	if (action == KOBJ_ADD)
		kobj->state_add_uevent_sent = 1;
	else if (action == KOBJ_REMOVE)
		kobj->state_remove_uevent_sent = 1;

	/* we will send an event, so request a new sequence number */
	spin_lock(&sequence_lock);
	seq = ++uevent_seqnum;
	spin_unlock(&sequence_lock);
	retval = add_uevent_var(env, "SEQNUM=%llu", (unsigned long long)seq);
	if (retval)
		goto exit;

#if defined(CONFIG_NET)
	/* send netlink message */
	mutex_lock(&uevent_sock_mutex);
	list_for_each_entry(ue_sk, &uevent_sock_list, list) {
		struct sock *uevent_sock = ue_sk->sk;
		struct sk_buff *skb;
		size_t len;

		/* allocate message with the maximum possible size */
		len = strlen(action_string) + strlen(devpath) + 2;
		skb = alloc_skb(len + env->buflen, GFP_KERNEL);
		if (skb) {
			char *scratch;

			/* add header */
			scratch = skb_put(skb, len);
			sprintf(scratch, "%s@%s", action_string, devpath);

			/* copy keys to our continuous event payload buffer */
			for (i = 0; i < env->envp_idx; i++) {
				len = strlen(env->envp[i]) + 1;
				scratch = skb_put(skb, len);
				strcpy(scratch, env->envp[i]);
			}

			NETLINK_CB(skb).dst_group = 1;
			retval = netlink_broadcast_filtered(uevent_sock, skb,
							    0, 1, GFP_KERNEL,
							    kobj_bcast_filter,
							    kobj);
			/* ENOBUFS should be handled in userspace */
			if (retval == -ENOBUFS)
				retval = 0;
		} else
			retval = -ENOMEM;
	}
	mutex_unlock(&uevent_sock_mutex);
#endif

	/* call uevent_helper, usually only enabled during early boot */
	if (uevent_helper[0] && !kobj_usermode_filter(kobj)) {
		char *argv [3];

		argv [0] = uevent_helper;
		argv [1] = (char *)subsystem;
		argv [2] = NULL;
		retval = add_uevent_var(env, "HOME=/");
		if (retval)
			goto exit;
		retval = add_uevent_var(env,
					"PATH=/sbin:/bin:/usr/sbin:/usr/bin");
		if (retval)
			goto exit;

		retval = call_usermodehelper(argv[0], argv,
					     env->envp, UMH_WAIT_EXEC);
	}

exit:
	kfree(devpath);
	kfree(env);
	return retval;
}
EXPORT_SYMBOL_GPL(kobject_uevent_env);

1、函数add_uevent_var组织事件消息，以字符串表示
2、配置CONFIG_NET后，netlink广播事件消息，函数netlink_broadcast_filtered发送netlink广播事件
（1）用户态通过创建NETLINK的socket，类型为NETLINK_KOBJECT_UEVENT，指定netlink目的地址。
（2）内核态通过函数netlink_kernel_create(net, NETLINK_KOBJECT_UEVENT,1, NULL, NULL, THIS_MODULE)创建内核态netlink的sock，与用户态指定相同目的地址。
（3）内核态sock在函数uevent_net_init中被创建，加入全局列表uevent_sock_list中。
（4）内核态sock和用户态socket都指定同一个广播域，因此在内核态发送广播消息后，用户态将受到消息。
3、uevent_helper字符串，配置了hotplug应用，将事件消息从内核发送到上层hotplug应用。
（1）
{
	.procname	= "hotplug",
	.data		= &uevent_helper,
	.maxlen		= UEVENT_HELPER_PATH_LEN,
	.mode		= 0644,
	.proc_handler	= proc_dostring,
},

/* uevent helper program, used during early boot */
static ssize_t uevent_helper_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", uevent_helper);
}
static ssize_t uevent_helper_store(struct kobject *kobj,
				   struct kobj_attribute *attr,
				   const char *buf, size_t count)
{
	if (count+1 > UEVENT_HELPER_PATH_LEN)
		return -ENOENT;
	memcpy(uevent_helper, buf, count);
	uevent_helper[count] = '\0';
	if (count && uevent_helper[count-1] == '\n')
		uevent_helper[count-1] = '\0';
	return count;
}
KERNEL_ATTR_RW(uevent_helper);

char uevent_helper[UEVENT_HELPER_PATH_LEN] = CONFIG_UEVENT_HELPER_PATH;

uevent_helper_attr 是通过 KERNEL_ATTR_RW(uevent_helper)定义的,也就是说,生成名为 /sys/kernel/uevent_helper 可读写的属性文件,其作用与/proc/sys/kernel/hotplug 相同,最终是读写 uevent_helper[]数组。读写的方法分别是uevent_helper_show()和 uevent_helper_store()例程。
（2）uevent_helper可能是hotplug（默认值），udev（标准linux下常用），mdev（嵌入式常用）。
（3）