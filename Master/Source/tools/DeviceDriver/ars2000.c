#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#define DEBUG

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/circ_buf.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#ifdef CONFIG_OF
#include <linux/of_device.h>
static const struct of_device_id ars2000_dt_ids[] = {
    { .compatible = "allrising,ars2000", },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, ars2000_dt_ids);
#else
#define pr_warn pr_warning
static struct resource ars2000_legacy_resource = {
    .start = 0x18000000,
    .end = 0x18ffffff,
};
static struct platform_device *ars2000_legacy_device;
#endif

#define ARS2000_CHIPNAME            "ars2000"
#define ARS2000_DEFAULT_RESOURCE    "ars2000-memory"
#define ARS2000_LEGACY_NAME         "ars2000-legacy"
#ifndef ARS2000_DRV_VERSION
#   define ARS2000_DRV_VERSION      "1.0-dev"
#endif
#define ARS2000_MIN_VERSION         0x500

MODULE_LICENSE("GPL");
MODULE_VERSION(ARS2000_DRV_VERSION);
MODULE_ALIAS("platform:ars2000");

#ifdef USE_DEBUG
static int debug = 16;
#else
static int debug = 3;
#endif

module_param(debug, int, 1);
MODULE_PARM_DESC(debug, "Debug level (0=none,...,16=all)");

static int default_buffer_size = 0x400000;  // Default 4MB buffer
module_param(default_buffer_size, int, 1);

static spinlock_t drv_spinlock;

struct ars2000_data {
    struct platform_device *pdev;
    void __iomem *ioaddr;
    struct resource *mem;
    struct resource *irq;
    struct cdev cdev;

    int version;    // Hardware version
    dev_t dev;

    struct circ_buf buffer;
    uint buffer_size;
};

static void ars2000_check_data(void)
{
    return;
}

static irqreturn_t ars2000_irqhandler(int irq, void *dev_id)
{
    struct ars2000_data *data = dev_id;
    int serviced = IRQ_HANDLED;
    pr_info("Handling irq %d, device %s\n", irq, data->pdev->name);
    ars2000_check_data();

    return serviced;
}

static int ars2000_open(struct inode *inode, struct file *filep)
{
    struct ars2000_data *dev = container_of(inode->i_cdev, struct ars2000_data, cdev);

    dev->buffer_size = default_buffer_size;
    dev->buffer.buf = kzalloc(dev->buffer_size, GFP_KERNEL);
    if (!dev->buffer.buf) {
        return -ENOMEM;
    }
    return 0;
}

static int ars2000_release(struct inode *inode, struct file *filep)
{
    struct ars2000_data *dev = container_of(inode->i_cdev, struct ars2000_data, cdev);
    kfree(dev->buffer.buf);
    return 0;
}

static void ars2000_drv_cleanup(struct platform_device *pdev)
{
    struct ars2000_data *data;

    data = platform_get_drvdata(pdev);
    if (!data)
        return;

    if (data->cdev.dev) {
        pr_info("  Delete cdev\n");
        cdev_del(&data->cdev);
    }
    if (data->dev) {
        pr_info("  Unregister char dev %i:%i\n", MAJOR(data->dev), MINOR(data->dev));
        unregister_chrdev_region(data->dev, 1);
    }
    if (data->irq) {
        pr_info("  Free irq %i\n", data->irq->start);
        free_irq(data->irq->start, data);
    }
    if (data->mem) {
        pr_info("  Release memory region %x size %x\n", data->mem->start, resource_size(data->mem));
        release_mem_region(data->mem->start, resource_size(data->mem));
    }
    if (data->ioaddr) {
        pr_info("  Unmap io address %p\n", data->ioaddr);
        iounmap(data->ioaddr);
    }
    kfree(data);
    platform_set_drvdata(pdev, NULL);
}

static int ars2000_drv_remove(struct platform_device *pdev)
{
    pr_warn("Removing ars2000 %s\n", pdev->name);
    ars2000_drv_cleanup(pdev);

    return 0;
}

static const struct file_operations ars2000_file_operations = {
    .open = ars2000_open,
    .release = ars2000_release,
};

static int ars2000_drv_probe(struct platform_device *pdev)
{
    struct ars2000_data *data = kzalloc(sizeof(*data), GFP_KERNEL);
    int retval = 0;

    if (!data) {
        pr_warn("Could not probe device. Out of memory\n");
        retval = -ENOMEM;
        goto out_0;
    }
    data->pdev = pdev;

    platform_set_drvdata(pdev, data);
    data->mem = platform_get_resource_byname(pdev, IORESOURCE_MEM,
                       ARS2000_DEFAULT_RESOURCE);
    if (!data->mem) {
        data->mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    }
    if (!data->mem) {
        pr_warn("Could not allocate memory resource\n");
        retval = -ENODEV;
        goto out_0;
    } else {
        int res_size = resource_size(data->mem);
        pr_info("Probing ars2000 %s with version %s: %x - %x, size %x\n", pdev->name, ARS2000_DRV_VERSION, data->mem->start, data->mem->end, res_size);

        if (!request_mem_region(data->mem->start, res_size, ARS2000_CHIPNAME)) {
            pr_warn("Could not request mem region\n");
            retval = -EBUSY;
            goto out_0;
        }

        pr_info("  Requesting ioremap %x size %x", data->mem->start, res_size);
        data->ioaddr = ioremap_nocache(data->mem->start, res_size);
        if (!data->ioaddr) {
            pr_warn("Invalid ioremap\n");
            retval = -ENOMEM;
            goto out_0;
        }
        pr_info("  Mapped io addr at %p\n", data->ioaddr);
    }

    data->version = ioread16(data->ioaddr + 0x24);
    if (data->version < ARS2000_MIN_VERSION) {
        pr_warn("Invalid hardware version %x on dev %s\n", data->version, pdev->name);
//        retval = -ENODEV;
//        goto out_0;
    }
    pr_info("  Found hardware with version: 0x%x\n", data->version);

    data->irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
    if (!data->irq) {
        pr_warn("No irq configured\n");
    } else {
        unsigned long irq_flags = (data->irq->flags & IRQF_TRIGGER_MASK) | IRQF_SHARED;
        pr_info("  Requesting irq %d with flags %lx (%lx)\n", data->irq->start, data->irq->flags, irq_flags);
        retval = request_irq(data->irq->start, ars2000_irqhandler,
                     irq_flags, pdev->name, data);
        if (retval) {
            pr_warn("Unable to claim requested irq: %d\n", data->irq->start);
            data->irq = 0;
        }
    }

    retval = alloc_chrdev_region(&data->dev, 0, 1, pdev->name);
    if (retval) {
        pr_warn("Error %i registering device", retval);
        goto out_0;
    } else {
        pr_info("  Char interface: \"%s\" at %i:%i", pdev->name, MAJOR(data->dev), MINOR(data->dev));
    }

    cdev_init(&data->cdev, &ars2000_file_operations);
    retval = cdev_add(&data->cdev, data->dev, 1);
    if (retval) {
        pr_warn("Unable to add cdev\n");
        goto out_0;
    }
    pr_warn("  Successfully probed ars2000\n");
    return 0;

out_0:
    pr_warn("Error probing ars2000, cleaning up\n");
    ars2000_drv_cleanup(pdev);
    return retval;
}

static struct platform_device_id ars2000_id_table[] = {
    { .name = ARS2000_LEGACY_NAME, },
    {},
};

static struct platform_driver ars2000_driver = {
    .probe = ars2000_drv_probe,
    .remove = ars2000_drv_remove,
    .driver = {
        .name	= ARS2000_CHIPNAME,
        .owner	= THIS_MODULE,
#ifdef CONFIG_OF
        .of_match_table = of_match_ptr(ars2000_dt_ids),
#endif
    },
    .id_table = ars2000_id_table,
};

/* Entry point for loading the module */
static int __init ars2000_init_module(void)
{
    pr_warn("Initializing ars2000 module\n");
    spin_lock_init(&drv_spinlock);
#ifndef CONFIG_OF
    pr_warn("Allocating platform device\n");
    ars2000_legacy_device = platform_device_register_simple(ARS2000_LEGACY_NAME, -1, &ars2000_legacy_resource, 1);
    if (IS_ERR(ars2000_legacy_device)) {
        pr_warn("Could not allocate device\n");
        return -ENOMEM;
    }
    return 0;
#endif
    return platform_driver_register(&ars2000_driver);
}

/* entry point for unloading the module */
static void __exit ars2000_cleanup_module(void)
{
    pr_warn("Uninitializing ars2000 module\n");
    platform_driver_unregister(&ars2000_driver);
#ifndef CONFIG_OF
    pr_warn("Unregister platform device\n");
    platform_device_unregister(ars2000_legacy_device);
    platform_device_del(ars2000_legacy_device);
#endif
}

module_init(ars2000_init_module);
module_exit(ars2000_cleanup_module);
