#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */

int init_module(void){
    printk(KERN_INFO "Hello Fredy.\n");
    /*
    * A non 0 return means init_module failed; module can't be loaded.
    */
    return 0;
}

void cleanup_module(void){
    printk(KERN_INFO "Goodbye Fredy.\n");
}

MODULE_LICENSE("GPL");

/*
make
sudo insmod hello-1.ko
lsmod

sudo rmmod hello-1

cd var/log

dmesg | tail -1

tail -f syslog
*/