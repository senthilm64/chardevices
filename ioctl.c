#include "chardev.h"

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ioctl.h>

main()
{
  int file_desc,ret;
  char *msg="message passed by ioctl\n";
  file_desc=open(device_file_name,0);
  if(file_desc<0)
    {
      printf("cant open %s file\n",device_file_name);
      exit (-1);
}
  ioctl_set_msg(file_desc,msg);
  ioctl_get_msg(file_desc);
  ioctl_get_nth_byte(file_desc);
  close(file_desc);
}

ioctl_set_msg(int file_desc,char *msg)
{
  int ret=ioctl(file_desc,IOCTL_SET_MSG,msg);
  if(ret<0)
    {
      printf("ioctl_set_msg is failed :%d\n",ret);
      exit (-1);
}
}

ioctl_get_msg(int file_desc)
{
  char msg[100];
  int ret=ioctl(file_desc,IOCTL_GET_MSG,msg);
  if(ret<0)
    {
      printf("ioctl_get_msg is failed %d\n",ret);
      exit (-1);
}
  printf("get_msg message:%s\n",msg);
}

ioctl_get_nth_msg(int file_desc)
{
  int i=0;
  char c;
do
  {
    c=ioctl(file_desc,IOCTL_GET_NTH_MSG,i++);
    if(c<0)
      {
	printf("ioctl_get_nth_msg is failed at %d\n",i);
	exit (-1);
}
    putchar(c);
  }
while(c!=0);
 putchar('\n');
}
