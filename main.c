// SPDX-License-Identifier: GPL-3.0-only
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/init.h>

#define DEVICE_NAME "naiwa"

static const char device_data[] = "THIS IS NAIWA                                           \n                     .:::::::.                         \n                   :::*%-::::::.                       \n                   ::-----::::::.                      \n                   --=-----::::::.                     \n                     ===----:::::.:                    \n                     :=----::::::...                   \n                     -----::::::::...                  \n                    ---:::::::::::....                 \n                  .:::::::::---::::..:.                \n                 -::::::::::---:::::.::                \n                ::::::::::::-+-:::::::::               \n              -:::::::::::::::::::::::::.              \n             +#:::-+=-:------------::::::              \n            =**-:=++++=------------:::::::             \n             *#-=****++=======-----:::::::             \n              ----*===-------------:::::::             \n              :----------------------:::::             \n               -=------------------------              \n                .==-==------------------               \n                  :=-==---------------                 \n                   ===========-----::                  \n                   -===++++ :=-----::                  \n                    ===++++  ==------                  \n                     =++++-   ====--.                  \n                      .***-    ++===.                  \n                       :#*      *++=                   \n                +***********     #*-                   \n             ...+*+++*+:::...:+++====                  \n              .............###*%#*++.                  \n                                                       \n";

static ssize_t naiwa_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    return simple_read_from_buffer(buf, count, pos,
                                   device_data, sizeof(device_data) - 1);
}

static ssize_t naiwa_write(struct file *file, const char __user *buf, size_t count, loff_t *pos)
{
    return count;
}

static const struct file_operations naiwa_ops = {
    .owner = THIS_MODULE,
    .read = naiwa_read,
    .write = naiwa_write,
};

static struct miscdevice naiwa_dev = {
    .name = DEVICE_NAME,
    .minor = MISC_DYNAMIC_MINOR,
    .fops = &naiwa_ops,
    .mode = 0666,
};

static int __init naiwa_dev_init(void)
{
    const int ret = misc_register(&naiwa_dev);
    if (ret)
    {
        return ret;
    }
    pr_info("naiwa_dev_init()\n");

    return 0;
}

static void __exit naiwa_dev_exit(void)
{
    misc_deregister(&naiwa_dev);
}

module_init(naiwa_dev_init);
module_exit(naiwa_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Net Salmon");
MODULE_DESCRIPTION("A virtual char device when you read it, it will fill buffer with a NaiWa Ascii Art.");
