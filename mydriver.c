#include <linux/module.h>
#include   <linux/kernel.h>
#include <linux/init.h>
#include<linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "mydriver"

static char message[]="Hello from kernel space!\n";


static ssize_t proc_read(struct file *file_pointer, char __user *user_buffer,
                         size_t count, loff_t *offset)
{
    size_t len   =  sizeof(message) - 1;


    if (*offset>= len)
        return 0;

    if (count >len - *offset)
        count = len - *offset;


    if (copy_to_user(user_buffer, message + *offset, count))
        return -EFAULT;

    *offset +=count;
    return count;
}


static const struct proc_ops proc_file_ops = {
    .proc_read =proc_read,
};


static struct proc_dir_entry *proc_entry;

static int __init driver_init(void)
{
    proc_entry= proc_create(PROC_NAME, 0444, NULL, &proc_file_ops);
    if (!proc_entry) {
        pr_err("mydriver: failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }
    pr_info("mydriver: loaded, created /proc/%s\n", PROC_NAME);
    return 0;
}

static void __exit driver_exit(void)
{
    proc_remove(proc_entry);
    pr_info("mydriver: unloaded, removed /proc/%s\n", PROC_NAME);
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ayanava Dutta");
MODULE_DESCRIPTION("A simple /proc kernel module that copies data to user space");
