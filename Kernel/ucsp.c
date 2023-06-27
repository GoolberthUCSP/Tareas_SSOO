#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/kdev_t.h>
#include <linux/types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("UCSP");
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
#define SUCCESS 0
#define DEVICE_NAME "ucsp"
#define BUF_LEN 256

static int Major; /* Major number assigned to our device driver */
static int Device_Open = 0; // Is device open?

static char msg[BUF_LEN]; /* The msg the device will give when asked */
static char *msg_Ptr;
static short  size_of_msg;
static struct file_operations fops = {
   .read = device_read,
   .write = device_write,
   .open = device_open,
   .release = device_release
};
/*
* This function is called when the module is loaded
static struct file_operations fops = {
static struct file_operations fops = {
*/
int init_module(void)
{
   Major = register_chrdev(0, DEVICE_NAME, &fops);
   if (Major < 0) {
      printk(KERN_ALERT "Registering char device failed with %d\n", Major);
      return Major;
   }
   printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
   printk(KERN_INFO "the driver, create a dev file with\n");
   printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
   printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
   printk(KERN_INFO "the device file.\n");
   printk(KERN_INFO "Remove the device file and module when done.\n");
   return SUCCESS; 
}

void cleanup_module(void)
{
   unregister_chrdev(Major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file)
{
   static int counter = 0;
   //sprintf(msg, "I already told you %d times Hello world!\n", counter++);
   msg_Ptr = msg;
   try_module_get(THIS_MODULE);
   return SUCCESS;
}


static int device_release(struct inode *inode, struct file *file)
{
   Device_Open--;
   module_put(THIS_MODULE);
   return 0;
}

static ssize_t device_read(struct file *filp, /* see include/linux/fs.h */
   char *buffer, /* buffer to fill with data */
   size_t length, /* length of the buffer */
   loff_t * offset)
   {
   int bytes_read = 0;
   if (*msg_Ptr == 0)
       return 0;
   while (length && *msg_Ptr) {
      put_user(*(msg_Ptr++), buffer++);
      length--;
      bytes_read++;
   }
   return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off){
   sprintf(msg, "%s(%zu letters)", buff, len);   // appending received string with its length
   size_of_msg = strlen(msg);                 // store the length of the stored msg
   printk(KERN_INFO "EBBChar: Received %zu characters from the user\n", len);
   return len;
}

/*
make
sudo insmod ucsp.ko
lsmod

dmesg

mknod /dev/ucsp c major minor

sudo rmmod ucsp

dmesg | tail -1

tail -f syslog
*/