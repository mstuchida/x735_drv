#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/umh.h>

/* constants */
#define GPIO4       4
#define GPIO17      17
#define DEVICE_NAME "x735"

/* functions */
static int gpio_init(void);
static int regist_interrupt_handler(void);
static irqreturn_t interrupt_handler(int irq, void *arg);
static void gpio_exit(void);

/* global varibles */
spinlock_t *lock;
static struct class *gpio_class;
static bool is_processed = false;
static struct gpio gpios[] = {
    {GPIO4, GPIOF_DIR_IN, DEVICE_NAME},
    {GPIO17, GPIOF_OUT_INIT_HIGH, DEVICE_NAME}
};

/* initialize funtion */
static int __init gpio_init(void) {

    int ret;

    // create class under /sys/class
    gpio_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (gpio_class == NULL) {
        printk(KERN_ERR "can't create class.\n");
        return -EINVAL;
    }

    // Initial configuration of gpio request
    ret = gpio_request_array(gpios, ARRAY_SIZE(gpios));
    if (ret != 0) {
        printk(KERN_ERR "failed to request gpio err_code=%d\n", ret);
        return -ENODEV;
    }

    lock = kmalloc(sizeof(spinlock_t), GFP_KERNEL);
    if (lock == NULL) {
        printk("failed to kmalloc.\n");
        return -ENOMEM;
    }

    // spinlock guarantees exclusive control using busy wait locking
    spin_lock_init(lock);

    // registering an interrupt handler
    ret = regist_interrupt_handler();
    if (ret != 0) {
        printk(KERN_ERR "failed to regist interrupt handler. err_code=%d\n", ret);
        return ret;
    }

    printk("initialized %s driver.\n", DEVICE_NAME);
    return 0;
}

/* registering handler function */
static int regist_interrupt_handler(void) {

    int ret;
    int irq;
    unsigned long flags;

    irq = gpio_to_irq(GPIO4);
    spin_lock_irqsave(lock, flags);
    ret = request_irq(irq, interrupt_handler, IRQF_SHARED | IRQF_TRIGGER_RISING, DEVICE_NAME, lock);
    spin_unlock_irqrestore(lock, flags);
    return ret;
}

/* interrupt handler function */
static irqreturn_t interrupt_handler(int irq, void *arg) {

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

    printk(KERN_INFO "process gpio_handler.\n");

    local_irq_save(flags);
    gpio_free_array(gpios, ARRAY_SIZE(gpios));
    call_usermodehelper(argv[0], argv, NULL, UMH_NO_WAIT);
    local_irq_restore(flags);
    is_processed = true;
    return IRQ_HANDLED;
}

/* exit function */
static void __exit gpio_exit(void) {

    kfree(lock);
    gpio_free_array(gpios, ARRAY_SIZE(gpios));
    class_destroy(gpio_class);
    printk(KERN_INFO "removed %s driver.\n", DEVICE_NAME);
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_VERSION("0.1.2");
MODULE_AUTHOR("mst.uchida");
MODULE_DESCRIPTION("unofficial driver for x735");
MODULE_LICENSE("GPL v2");

