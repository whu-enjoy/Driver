/////////////////////////////////////////////////////////////////////////////
//  文件名 : fitst.c
//  工程 : first 
//  作者 : enjoy5512   修改者 : enjoy5512   最后优化注释者 : enjoy5512
//  个人技术博客 : blog.csdn.net/enjoy5512
//  个人GitHub   : github.com/whu-enjoy
//  描述 : 第一个驱动程序
//  主要函数 :
//     	NTSTATUS DriverEntry(__in PDRIVER_OBJECT DriverObject, __in PUNICODE_STRING RegistryPath) 入口函数
//		VOID DriverUnload(__in PDRIVER_OBJECT DriverObject) 卸载函数
//      NTSTATUS DefaultDispatch(__in PDEVICE_OBJECT DeviceObject, __in PIRP Irp) 默认派遣函数
//  版本 : 1.0          完成日期 : 2016年7月5日 09:36:17
//  修改 :1.1(完善代码) 完成日期 : 2016年7月5日 16:52:43
//  参考文献 :
//       <<Windows内核安全与驱动开发>> 谭文 陈铭霖 编著
//		 <<竹林蹊径:深入浅出Windows驱动开发>> 张佩 马勇 董鉴源编著
/////////////////////////////////////////////////////////////////////////////
	
#include "first.h"

/////////////////////////////////////////////////////////////////////////////
//*= = 预定义处理
/////////////////////////////////////////////////////////////////////////////
#pragma alloc_text(INIT, DriverEntry)
//#pragma alloc_text(PAGE, DefaultDispatch)
//#pragma alloc_text(PAGE, DriverUnload)

//程序说明开始
//==================================================================================
//  功能 : 驱动程序入口函数
//  参数 : 	__in PDRIVER_OBJECT DriverObject, in PUNICODE_STRING RegistryPath
//  (入口)  __in PDRIVER_OBJECT DriverObject  : 驱动对象
//			__in PUNICODE_STRING RegistryPath : 驱动在注册表中的键值
//  (出口)  无
//  返回 :  NTSTATUS
//  主要思路 :
//  调用举例 : 
//  日期 : 2016年7月5日 15:46:16
//==================================================================================
//程序说明结束
NTSTATUS 
DriverEntry(
	__in PDRIVER_OBJECT DriverObject, 
	__in PUNICODE_STRING RegistryPath
	)
{
	NTSTATUS Status = STATUS_SUCCESS;
	PDEVICE_OBJECT DeviceObject = NULL;
	PDEVICE_EXTENSION DeviceExtension = NULL;
	UNICODE_STRING SymbolicLink = {0};
	UNICODE_STRING DeviceName = {0};
	ULONG i = 0;
	
	#if DBG
		_asm int 3
	#endif
	
	KdPrint(("Enter DriverEntry!!\n"));
	
	UNREFERENCED_PARAMETER(RegistryPath);
	
	RtlInitUnicodeString(&DeviceName, L"\\Device\\first");
	
	//处理派遣函数
	for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		DriverObject->MajorFunction[i] = DefaultDispatch;
	}
	
	DriverObject->DriverUnload = DriverUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = DefaultDispatch;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = DefaultDispatch;
	DriverObject->MajorFunction[IRP_MJ_READ] = DefaultDispatch;
	DriverObject->MajorFunction[IRP_MJ_WRITE] = DefaultDispatch;
	
	//创建设备
	Status = IoCreateDevice(DriverObject,
							sizeof(DEVICE_EXTENSION),
							&DeviceName,
							FILE_DEVICE_UNKNOWN,
							0,
							TRUE,
							&DeviceObject);
	if (!NT_SUCCESS(Status))
	{
		DbgPrint("Create Device fault!! -- %d\n",Status);
		return Status;
	}
	DeviceObject->Flags |= DO_BUFFERED_IO;
	DeviceExtension = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;
	DeviceExtension->DeviceObject = DeviceObject;
	DeviceExtension->DeviceName = DeviceName;
	RtlInitUnicodeString(&SymbolicLink, L"\\??\\first");
	DeviceExtension->SymbolicLink = SymbolicLink;
	
	//创建符号链接
	Status = IoCreateSymbolicLink(&SymbolicLink, &DeviceName);
	if (!NT_SUCCESS(Status))
	{
		DbgPrint("Create SymbolicLink fault!! -- %d\n",Status);
		IoDeleteDevice(DeviceObject);
		return Status;
	}
	
	KdPrint(("End of first DriverEntry!!\n"));
	return Status;
}

//程序说明开始
//==================================================================================
//  功能 : 驱动程序卸载
//  参数 : 	__in PDRIVER_OBJECT DriverObject
//  (入口)  __in PDRIVER_OBJECT DriverObject  : 驱动对象
//  (出口)  无
//  返回 :  VOID
//  主要思路 :
//  调用举例 : 
//  日期 : 2016年7月5日 16:51:51
//==================================================================================
//程序说明结束
VOID 
DriverUnload(
	__in PDRIVER_OBJECT DriverObject
	)
{
	PDEVICE_OBJECT DeviceObject = NULL;
	UNICODE_STRING LinkName = {0};
	PDEVICE_EXTENSION DeviceExtension = NULL;
	
	KdPrint(("Unloading Driver!!\r\n"));
	
	DeviceObject = DriverObject->DeviceObject;
	while (NULL != DeviceObject)
	{
		DeviceExtension = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;
		
		//删除符号链接与设备
		LinkName = DeviceExtension->SymbolicLink;
		IoDeleteSymbolicLink(&LinkName);
		DeviceObject = DeviceObject->NextDevice;
		IoDeleteDevice(DeviceExtension->DeviceObject);
	}
	
	KdPrint(("End of the DriverUnload!!\n"));
}

//程序说明开始
//==================================================================================
//  功能 : 驱动程序默认派遣函数
//  参数 : 	__in PDRIVER_OBJECT DriverObject, __in PIRP Irp
//  (入口)  __in PDRIVER_OBJECT DriverObject  : 驱动对象
//          __in PIRP Irp : IRP请求
//  (出口)  无
//  返回 :  NTSTATUS
//  主要思路 : 
//  调用举例 : 
//  日期 : 2016年7月5日 16:51:40
//==================================================================================
//程序说明结束
NTSTATUS DefaultDispatch(__in PDEVICE_OBJECT DeviceObject, __in PIRP Irp)
{
	NTSTATUS Status = STATUS_SUCCESS;
	
	KdPrint(("Enter the DefaultDispatch!!\n "));
	
	UNREFERENCED_PARAMETER(DeviceObject);
	
	//完成IRP请求
	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	
	KdPrint(("End of the DefaultDispatch!!\n "));
	return Status;
}	