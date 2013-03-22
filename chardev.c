#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<asm/uaccess.h>

#include "chardev.h"

#define device_name "chardev"
#define buf_len 80

static char msg[buf_len];
static char *msg_ptr;
static int Device_open=0;

static int device_open(struct inode *inode,struct file *filp)
{
#ifdef DEBUG
  printk(KERN_INFO "device_open (%p)\n",file);
#endif
  if(Device_open)
    return -EBUSY;
  Device_open++;
  msg_ptr=msg;
  try_module_get(THIS_MODULE);
  return 0;
}

static int device_release(struct inode *inode,struct file *filp)
{
  Device_open--;
#ifdef DEBUG
  printk(KERN_INFO "device_release (%p %p)\n",inode,file);
#endif
  module_put(THIS_MODULE);
  return 0;
}

static ssize_t device_read(struct file *filp,char __user *buffer,size_t length,loff_t *offset)
{
  int byte_read=0;
#ifdef DEBUG
  printk(KERN_INFO "device_read (%p%p%d)",file,buffer,length);
#endif
  if(*msg_ptr==0)
    return 0;
  while(length&&*msg_ptr)
    {
      put_user(*(msg_ptr++),buffer++);
      length--;
      byte_read++;
}
#ifdef DEBUG
  printk(KERN_INFO "read%p left %p",byte_read,length);
#endif
  return byte_read;
}

ssize_t device_write(struct file *filp,const char __user *buffer,size_t length,loff_t *offset)
{
  int i;
#ifdef DEBUG
  printk(KERN_INFO "device_write (%p,%s,%d),file,buffer,length");
#endif  
for(i=0;i<length&&i<buf_len;i++)
    get_user(msg[i],buffer+i);
 msg_ptr=msg;
 return i;
}

int device_ioctl(struct inode *inode,struct file *filp,unsigned int ioctl_no,unsigned long ioctl_param)
{
  int j;
  char *temp,ch;
  switch(ioctl_no){
 case IOCTL_SET_MSG:
  temp=(char *)ioctl_param;
  get_user(ch,temp);
  for(j=0;ch&&j<buf_len;j++,temp++)
  get_user(ch,temp);
  device_write(filp,(char *)ioctl_param,j,0);
  break;
  case IOCTL_GET_MSG:
    j=device_write(filp,(char *)ioctl_param,99,0);
    put_user('\0',(char *)ioctl_param+j);
    break;
  case IOCTL_GET_NTH_MSG:
    return msg[ioctl_param];
    break;
}
  return 0;
}

struct file_operations fops={
  .read=device_read,
  .write=device_write,
  .open=device_open,
.release=device_release
};

int init_module(void)
{
  int ret;
  ret=register_chrdev(major_no,device_name,&fops);
  if(ret<0)
    {
    printk(KERN_ALERT "registered char device failed with %d\n",major_no);
  return ret;
    }
printk(KERN_INFO "/mknod/%s c %d\n",device_name,major_no);
return 0;
}

void cleanup_module(void)
{
  unregister_chrdev(major_no,device_name);
}
