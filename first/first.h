/////////////////////////////////////////////////////////////////////////////
//  文件名 : fitst.h
//  工程 : first 
//  作者 : enjoy5512   修改者 : enjoy5512   最后优化注释者 : enjoy5512
//  个人技术博客 : blog.csdn.net/enjoy5512
//  个人GitHub   : github.com/whu-enjoy
//  描述 : 第一个驱动程序的头文件
//  版本 : 1.0  完成日期 : 2016年7月5日 09:25:30
//  修改 : 1.1  完成日期 : 2016年7月5日 16:43:47
//  参考文献 :
//       <<Windows内核安全与驱动开发>> 谭文 陈铭霖 编著
//////////////////////////////////////////////////////////////////////////////

#ifndef __FIRST_H__
#define __FIRST_H__


//////////////////////////////////////////////////////////////////////////////
//*= = 头文件声明
//////////////////////////////////////////////////////////////////////////////
#include <ntddk.h>

//////////////////////////////////////////////////////////////////////////////
//*= = 宏与结构体
//////////////////////////////////////////////////////////////////////////////
typedef struct _DEVICE_EXTENSION{
	PDEVICE_OBJECT DeviceObject;  //设备对象指针
	UNICODE_STRING DeviceName;    //设备名称
	UNICODE_STRING SymbolicLink;  //符号链接名
}DEVICE_EXTENSION, *PDEVICE_EXTENSION;

DRIVER_INITIALIZE
 DriverEntry;

DRIVER_UNLOAD
 DriverUnload;

__drv_dispatchType(IRP_MJ_CREATE)
__drv_dispatchType(IRP_MJ_CLOSE)
__drv_dispatchType(IRP_MJ_READ)
__drv_dispatchType(IRP_MJ_WRITE)
DRIVER_DISPATCH
 DefaultDispatch;

//////////////////////////////////////////////////////////////////////////////
//*= = 函数声明
//////////////////////////////////////////////////////////////////////////////
NTSTATUS
DriverEntry(
	__in PDRIVER_OBJECT DriverObject,
	__in PUNICODE_STRING RegistryPath
	);

VOID
DriverUnload(
	__in PDRIVER_OBJECT DriverObject
	);
	
NTSTATUS
DefaultDispatch(
	__in PDEVICE_OBJECT DeviceObject,
	__in PIRP Irp);
	
#endif //End of __FIRST_H__