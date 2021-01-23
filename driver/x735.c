#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/umh.h>

/* constants */
#define GPIO4       4
#define GPIO17      17
#define DEVICE_NAME "x735"

/* functions */
static irqreturn_t gpio_handler(int irq, void *arg);
static int gpio_init(void);
static void gpio_exit(void);

/* global varibles */
spinlock_t *lock;
static struct class *gpio_class;
static bool is_processed = false;

/* interrupt function */
static irqreturn_t gpio_handler(int irq, void *arg) {

    unsigned long flags;
    static char *argv[] = {
        "/bin/bash",
        "-c",
        "/usr/local/bin/poweroff.sh",
        NULL
    };

    if (is_processed) {
        return IRQ_HANDLED;
    }

    // disabled interrupt
    local_irq_save(flags);
    printk(KERN_INFO "process gpio_handler.\n");

    // release gpio
    gpio_free(GPIO4);
    gpio_free(GPIO17);

    // power off
    call_usermodehelper(argv[0], argv, NULL, UMH_NO_WAIT);

    // enabled interrupt
    local_irq_restore(flags);

    is_processed = true;

    return IRQ_HANDLED;
}

/* initialize funtion */
static int __init gpio_init(void) {

    int ret;
    int irq;
    unsigned long flags;

    // create class under /sys/class
    gpio_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (gpio_class == NULL) {
        printk(KERN_ERR "can't create class.\n");
        return -EINVAL;
    }

    lock = kmalloc(sizeof(spinlock_t), GFP_KERNEL);
    if (!lock) {
        printk("failed to kmalloc.\n");
        return -ENOMEM;
    }

    // Initial configuration of single GPIO4/17/18 request
    ret = gpio_request_one(GPIO4, GPIOF_DIR_IN, DEVICE_NAME);
    if (ret != 0) {
        printk(KERN_ERR "failed to request gpio4 err_code=%d\n", ret);
        return -ENODEV;
    }
    ret = gpio_request_one(GPIO17, GPIOF_OUT_INIT_HIGH, DEVICE_NAME);
    if (ret != 0) {
        printk(KERN_ERR "failed to request gpio17 err_code=%d\n", ret);
        return -ENODEV;
    }

    // spinlock guarantees exclusive control using busy wait locking
    spin_lock_init(lock);

    // open gpio4
    irq = gpio_to_irq(GPIO4);
    spin_lock_irqsave(lock, flags);
    ret = request_irq(irq, gpio_handler, IRQF_SHARED | IRQF_TRIGGER_RISING, DEVICE_NAME, lock);
    spin_unlock_irqrestore(lock, flags);
    if (ret != 0) {
        printk(KERN_ERR "failed to set interrupt. irq=%d, err_code=%d\n", irq, ret);
        return ret;
    }

    printk("initialized %s driver.\n", DEVICE_NAME);
    return 0;
}

/* exit function */
static void __exit gpio_exit(void) {

    kfree(lock);
    gpio_direction_output(GPIO4, 0);
    gpio_direction_output(GPIO17, 0);
    gpio_free(GPIO4);
    gpio_free(GPIO17);
    class_destroy(gpio_class);
    printk(KERN_INFO "removed %s driver.\n", DEVICE_NAME);
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_VERSION("0.1.0");
MODULE_AUTHOR("mst.uchida");
MODULE_DESCRIPTION("unofficial driver for x735");
MODULE_LICENSE("GPL v2");

