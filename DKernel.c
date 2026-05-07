#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/string.h>

#define TARGET_PATH "/sdcard/respost.txt"
#define LOG_MESSAGE "DKernel: Hello from kernel module!\n"

static struct file *file_open(const char *path, int flags, int mode)
{
    struct file *f = NULL;
    mm_segment_t old_fs = get_fs();
    set_fs(KERNEL_DS);
    f = filp_open(path, flags, mode);
    set_fs(old_fs);
    if (IS_ERR(f))
        return NULL;
    return f;
}

static void file_close(struct file *f)
{
    if (f)
        filp_close(f, NULL);
}

static int file_write(struct file *f, const char *data, size_t len)
{
    mm_segment_t old_fs = get_fs();
    int ret;
    if (!f)
        return -ENOENT;
    set_fs(KERNEL_DS);
    ret = vfs_write(f, data, len, &f->f_pos);
    set_fs(old_fs);
    return ret;
}

static int __init dkernel_init(void)
{
    struct file *f;
    printk(KERN_INFO "DKernel: Module loaded\n");
    printk(KERN_INFO "DKernel: Writing to %s\n", TARGET_PATH);

    f = file_open(TARGET_PATH, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (f) {
        file_write(f, LOG_MESSAGE, strlen(LOG_MESSAGE));
        file_close(f);
        printk(KERN_INFO "DKernel: Write successful\n");
    } else {
        printk(KERN_ERR "DKernel: Failed to open %s\n", TARGET_PATH);
    }

    return 0;
}

static void __exit dkernel_exit(void)
{
    printk(KERN_INFO "DKernel: Module unloaded\n");
}

module_init(dkernel_init);
module_exit(dkernel_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple kernel module that writes log to /sdcard/respost.txt");
MODULE_VERSION("1.0");
