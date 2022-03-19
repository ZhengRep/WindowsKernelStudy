#include<windows.h>
#include<tchar.h>
#include<stdio.h>
//Device Name OriginalName:slbkcdo_3948d33e
#define cwkDeviceName L"\\??\\slbkcdo_3948d33e"

//Define Request Functional Number
#define CWKSENDSTRTODEVICE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x911, METHOD_BUFFERED, FILE_WRITE_DATA)
#define CWKRECEIVESTRFROMDEVICE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x912, METHOD_BUFFERED, FILE_READ_DATA)

int _tmain(int argc, TCHAR* argv)
{
	HANDLE hDevice = NULL;
	UINT nStrLength = 0;
	ULONG nReturnStrLength = 0;
	const char* szMsg = "I am Message to Device!";
	//const TCHAR* szMsg = _T("I am Message to Device!");
	getchar();
	//FILE_ATTRIBUTE_SYSTEM This file is part of or used exclusively by an operation system
	hDevice = CreateFile(cwkDeviceName, GENERIC_WRITE | GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("CreateFile is Wrong!"));
		return 0;
	}
	else
	{
		_tprintf(_T("CreateFile is Success!"));
	}

	if (!DeviceIoControl(hDevice, CWKSENDSTRTODEVICE, &szMsg, strlen(szMsg) + 1, NULL, 0, &nReturnStrLength, 0))
	{
		_tprintf(_T("CTL is Wrong!"));
		return 0;
	}
	else
	{
		_tprintf(_T("CTL is Success!"));
	}

	CloseHandle(hDevice);
	system("pause");

	return 0;
}