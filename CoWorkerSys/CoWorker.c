#include "CoWork.h"

//////////////////////////////////////////////////////////////////////////
NTSTATUS CwkDispatch(PDEVICE_OBJECT pDriver, PIRP pIRP)
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG ulReturnLength = 0;
	
	//Get IRP Stack location
	PIO_STACK_LOCATION islIRP = IoGetCurrentIrpStackLocation(pIRP);
	while (pDriver == g_CDO)
	{
		//Other IRP
		if (islIRP->MajorFunction == IRP_MJ_CREATE || islIRP->MajorFunction == IRP_MJ_CLOSE)
		{
			break;
		}
		//Control IRP
		if (islIRP->MajorFunction == IRP_MJ_DEVICE_CONTROL)
		{
			PVOID TempBuffer = pIRP->AssociatedIrp.SystemBuffer;
			ULONG ulInLength = islIRP->Parameters.DeviceIoControl.InputBufferLength;
			ULONG ulOutLength = islIRP->Parameters.DeviceIoControl.OutputBufferLength;

			//Is Send?
			if (islIRP->Parameters.DeviceIoControl.IoControlCode == CWKSENDSTRTODEVICE)
			{
				ASSERT(TempBuffer != NULL);
				ASSERT(ulInLength < 512);
				ASSERT(ulOutLength == 0);
				DbgPrint((TCHAR*)TempBuffer);
				break;
			}
			if (islIRP->Parameters.DeviceIoControl.IoControlCode == CWKRECEIVESTRFROMDEVICE)
			{
				break;
			}
			else
			{
				status = STATUS_INVALID_PARAMETER;
				break;
			}
		}

		break;
	}

	pIRP->IoStatus.Information = ulReturnLength;
	pIRP->IoStatus.Status = status;
	IoCompleteRequest(pIRP, IO_NO_INCREMENT);
	return status;
}

VOID DriverUnload(PDRIVER_OBJECT Driver)
{
	UNREFERENCED_PARAMETER(Driver);
	UNICODE_STRING usCDOSymbolic = RTL_CONSTANT_STRING(cwkDeviceName);
	ASSERT(g_CDO != NULL);
	IoDeleteSymbolicLink(&usCDOSymbolic);
	IoDeleteDevice(g_CDO);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT Driver, UNICODE_STRING RegisterPath)
{
	UNREFERENCED_PARAMETER(RegisterPath);

	NTSTATUS status = STATUS_SUCCESS;

	UNICODE_STRING usSDDL = RTL_CONSTANT_STRING(L"D:P(A;;GA;;;WD)");
	UNICODE_STRING usCDOName = RTL_CONSTANT_STRING(L"\\Device\\cwk_3948d33e");
	UNICODE_STRING usCDOSymbolic = RTL_CONSTANT_STRING(cwkDeviceName);

	//Create CDO
	status = IoCreateDeviceSecure(Driver, 0, &usCDOName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &usSDDL, (LPCGUID)(&cwkGUID), &g_CDO);
	if (!NT_SUCCESS(status)) return status;

	//Create SymbolicLink
	IoDeleteSymbolicLink(&usCDOSymbolic);
	status = IoCreateSymbolicLink(&usCDOSymbolic, &usCDOName);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(g_CDO);
		return status;
	}

	//Dispatch
	int i = 0;
	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		Driver->MajorFunction[i] = CwkDispatch;
	}

	Driver->DriverUnload = DriverUnload;
	return status;
}



