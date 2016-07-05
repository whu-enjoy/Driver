/////////////////////////////////////////////////////////////////////////////
//  �ļ��� : fitst.h
//  ���� : first 
//  ���� : enjoy5512   �޸��� : enjoy5512   ����Ż�ע���� : enjoy5512
//  ���˼������� : blog.csdn.net/enjoy5512
//  ����GitHub   : github.com/whu-enjoy
//  ���� : ��һ�����������ͷ�ļ�
//  �汾 : 1.0  ������� : 2016��7��5�� 09:25:30
//  �޸� : 1.1  ������� : 2016��7��5�� 16:43:47
//  �ο����� :
//       <<Windows�ں˰�ȫ����������>> ̷�� ������ ����
//////////////////////////////////////////////////////////////////////////////

#ifndef __FIRST_H__
#define __FIRST_H__


//////////////////////////////////////////////////////////////////////////////
//*= = ͷ�ļ�����
//////////////////////////////////////////////////////////////////////////////
#include <ntddk.h>

//////////////////////////////////////////////////////////////////////////////
//*= = ����ṹ��
//////////////////////////////////////////////////////////////////////////////
typedef struct _DEVICE_EXTENSION{
	PDEVICE_OBJECT DeviceObject;  //�豸����ָ��
	UNICODE_STRING DeviceName;    //�豸����
	UNICODE_STRING SymbolicLink;  //����������
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
//*= = ��������
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