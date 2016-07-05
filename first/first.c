/////////////////////////////////////////////////////////////////////////////
//  �ļ��� : fitst.c
//  ���� : first 
//  ���� : enjoy5512   �޸��� : enjoy5512   ����Ż�ע���� : enjoy5512
//  ���˼������� : blog.csdn.net/enjoy5512
//  ����GitHub   : github.com/whu-enjoy
//  ���� : ��һ����������
//  ��Ҫ���� :
//     	NTSTATUS DriverEntry(__in PDRIVER_OBJECT DriverObject, __in PUNICODE_STRING RegistryPath) ��ں���
//		VOID DriverUnload(__in PDRIVER_OBJECT DriverObject) ж�غ���
//      NTSTATUS DefaultDispatch(__in PDEVICE_OBJECT DeviceObject, __in PIRP Irp) Ĭ����ǲ����
//  �汾 : 1.0          ������� : 2016��7��5�� 09:36:17
//  �޸� :1.1(���ƴ���) ������� : 2016��7��5�� 16:52:43
//  �ο����� :
//       <<Windows�ں˰�ȫ����������>> ̷�� ������ ����
//		 <<�����辶:����ǳ��Windows��������>> ���� ���� ����Դ����
/////////////////////////////////////////////////////////////////////////////
	
#include "first.h"

/////////////////////////////////////////////////////////////////////////////
//*= = Ԥ���崦��
/////////////////////////////////////////////////////////////////////////////
#pragma alloc_text(INIT, DriverEntry)
//#pragma alloc_text(PAGE, DefaultDispatch)
//#pragma alloc_text(PAGE, DriverUnload)

//����˵����ʼ
//==================================================================================
//  ���� : ����������ں���
//  ���� : 	__in PDRIVER_OBJECT DriverObject, in PUNICODE_STRING RegistryPath
//  (���)  __in PDRIVER_OBJECT DriverObject  : ��������
//			__in PUNICODE_STRING RegistryPath : ������ע����еļ�ֵ
//  (����)  ��
//  ���� :  NTSTATUS
//  ��Ҫ˼· :
//  ���þ��� : 
//  ���� : 2016��7��5�� 15:46:16
//==================================================================================
//����˵������
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
	
	//������ǲ����
	for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		DriverObject->MajorFunction[i] = DefaultDispatch;
	}
	
	DriverObject->DriverUnload = DriverUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = DefaultDispatch;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = DefaultDispatch;
	DriverObject->MajorFunction[IRP_MJ_READ] = DefaultDispatch;
	DriverObject->MajorFunction[IRP_MJ_WRITE] = DefaultDispatch;
	
	//�����豸
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
	
	//������������
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

//����˵����ʼ
//==================================================================================
//  ���� : ��������ж��
//  ���� : 	__in PDRIVER_OBJECT DriverObject
//  (���)  __in PDRIVER_OBJECT DriverObject  : ��������
//  (����)  ��
//  ���� :  VOID
//  ��Ҫ˼· :
//  ���þ��� : 
//  ���� : 2016��7��5�� 16:51:51
//==================================================================================
//����˵������
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
		
		//ɾ�������������豸
		LinkName = DeviceExtension->SymbolicLink;
		IoDeleteSymbolicLink(&LinkName);
		DeviceObject = DeviceObject->NextDevice;
		IoDeleteDevice(DeviceExtension->DeviceObject);
	}
	
	KdPrint(("End of the DriverUnload!!\n"));
}

//����˵����ʼ
//==================================================================================
//  ���� : ��������Ĭ����ǲ����
//  ���� : 	__in PDRIVER_OBJECT DriverObject, __in PIRP Irp
//  (���)  __in PDRIVER_OBJECT DriverObject  : ��������
//          __in PIRP Irp : IRP����
//  (����)  ��
//  ���� :  NTSTATUS
//  ��Ҫ˼· : 
//  ���þ��� : 
//  ���� : 2016��7��5�� 16:51:40
//==================================================================================
//����˵������
NTSTATUS DefaultDispatch(__in PDEVICE_OBJECT DeviceObject, __in PIRP Irp)
{
	NTSTATUS Status = STATUS_SUCCESS;
	
	KdPrint(("Enter the DefaultDispatch!!\n "));
	
	UNREFERENCED_PARAMETER(DeviceObject);
	
	//���IRP����
	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	
	KdPrint(("End of the DefaultDispatch!!\n "));
	return Status;
}	