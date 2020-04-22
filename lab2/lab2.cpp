#include <stdlib.h>
#include <locale.h>  
#include <stdio.h>
#include <windows.h>
#include <tchar.h>


BOOL FindFileByClaster(TCHAR* volume)
{
	HANDLE hDevice = CreateFile(volume,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		wprintf(L"Handle error\n");
		return FALSE;
	}

	STARTING_VCN_INPUT_BUFFER input = { 0 };
	input.StartingVcn.QuadPart = 0L;

	DWORD output[5000] = {};
	DWORD dwRes = 0;
	RETRIEVAL_POINTERS_BUFFER  result = { 0 };

	BOOL bRes = DeviceIoControl((HANDLE)hDevice,
		FSCTL_GET_RETRIEVAL_POINTERS,
		(LPVOID)&input,
		(DWORD)sizeof(input),
		(LPVOID)output,
		(DWORD)5000,
		(LPDWORD)&dwRes,
		NULL);

	if (bRes == FALSE) {
		wprintf(L"Retreiving file info error\n");
		return FALSE;
	}

	memcpy(&result, &output, sizeof(RETRIEVAL_POINTERS_BUFFER));

	if (result.ExtentCount == 0) {
		wprintf(L"File not found\n");
		return FALSE;
	}
	wprintf(L"Extent count: %d\n", result.ExtentCount);
	wprintf(L"File information:\n");
	wprintf(L"\tStart cluster: %llu ", result.Extents[0].Lcn.QuadPart);
	wprintf(L"\r\n\tLength: %llu \n\n", result.Extents[0].NextVcn.QuadPart - result.StartingVcn.QuadPart);
	for (int i = 1; i < result.ExtentCount; i++)
	{
		wprintf(L"File information:\n");
		wprintf(L"\tStart cluster: %llu ", result.Extents[i].Lcn.QuadPart);
		wprintf(L"\r\n\tLength: %llu \n\n", result.Extents[i].NextVcn.QuadPart - result.Extents[i - 1].NextVcn.QuadPart);
	}
}

int main()
{
	FindFileByClaster((TCHAR*)L"pic.bmp");

	system("PAUSE");
	return 0;
}