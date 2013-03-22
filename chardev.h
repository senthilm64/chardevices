
#include<linux/ioctl.h>

#define major_no 100

#define IOCTL_SET_MSG _IOR(major_no,0,char *)
#define IOCTL_GET_MSG _IOR(major_no,1,char *)
#define IOCTL_GET_NTH_MSG _IOWR(major_no,2,int)

#define device_file_name "chardev"
