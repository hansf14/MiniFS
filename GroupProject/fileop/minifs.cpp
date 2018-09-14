#include "fileoperate.h"

int MiniFS::create()
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
			_tprintf(_T("Cannot create the operating space %d\n"), dwErr);
		}
		return -1;
	}

	//hFMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	hFMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 1024 * 1024 * 1024, NULL);
	if (!hFMap)
	{
		_tprintf(_T("Create file mapping failed./n"));
		CloseHandle(hFile);
		return -1;
	}

	return 0;
}

void MiniFS::close()
{
	CloseHandle(hFile);
	CloseHandle(hFMap);

	//free other stuff on ram
}