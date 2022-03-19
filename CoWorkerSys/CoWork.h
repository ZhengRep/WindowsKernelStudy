#pragma once
#include<ntddk.h>
#include<wdmsec.h>
#include<tchar.h>

//Device Name OriginalName:slbkcdo_3948d33e
#define cwkDeviceName L"\\??\\slbkcdo_3948d33e"
const GUID cwkGUID = { 0x17a0d1e0L, 0x3249, 0x12e1, {0x92,0x16, 0x45, 0x1a, 0x21, 0x30, 0x29, 0x06} };

//Define Request Functional Number
#define CWKSENDSTRTODEVICE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x911, METHOD_BUFFERED, FILE_WRITE_DATA)
#define CWKRECEIVESTRFROMDEVICE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x912, METHOD_BUFFERED, FILE_READ_DATA)

//////////////////////////////////////////////////////////////////////////
PDEVICE_OBJECT g_CDO = NULL;

////////////////////////////////////////////////////////////////////////////
////Function Declaration
NTSTATUS DriverEntry(PDRIVER_OBJECT Driver, UNICODE_STRING RegisterPath);
VOID DriverUnload(PDRIVER_OBJECT Driver);
NTSTATUS CwkDispatch(PDEVICE_OBJECT pDriver,PIRP pIRP);