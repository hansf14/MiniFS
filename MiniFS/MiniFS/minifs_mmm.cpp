#include "minifs_mmm.h"
#include <stdio.h>

int MiniFS_MMM::create()
{
	hFile = CreateFile(_T("c:\\SName.dat"), GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	//FILE_SHARE_READ or 0
	//NULL or CREATE_ALWAYS

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwErr = GetLastError();
		if (dwErr > 0) {
			//MessageBox(NULL, _T("Cannot create the operating space\n"),
			//_T("Error"), MB_OK);
			wprintf(L"Cannot create the operating space %d\n", dwErr);
		}
		return -1;
	}

	//hFMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	hFMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 1024 * 1024 * 1024, NULL);
	if (!hFMap)
	{
		wprintf(L"Create file mapping failed./n");
		CloseHandle(hFile);
		return -1;
	}

	return 0;
}

void MiniFS_MMM::close()
{
	CloseHandle(hFile);
	CloseHandle(hFMap);

	//free other stuff on ram
}