// spz_lr_4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "ctime"
#include "Windows.h"

using namespace std;


#define BUFFER_LENGTH 64 * 1024
unsigned char buffer[BUFFER_LENGTH];


void FillBuffer()
{
	for (int i = 0; i < BUFFER_LENGTH; ++i) buffer[i] = (unsigned char)(rand() % (122 - 65) + 65);
}

#pragma region Async
clock_t AsyncFileWrite()
{
	HANDLE hFile;
	OVERLAPPED ovl;

	clock_t start = clock();

	//������������� ������� ��������� OVERLAPPED
	ovl.Offset = 0;
	ovl.OffsetHigh = 0;
	ovl.hEvent = 0;

	hFile = CreateFile(
		L"async.txt",
		GENERIC_WRITE,
		FILE_SHARE_WRITE,		//���������� ������
		NULL,					// SD (�����. ������)
		CREATE_ALWAYS,			// ������� ����� ����
		FILE_FLAG_OVERLAPPED,	//�������� �����
		NULL					//�����.������� �����
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		cout << "Error.Code = " << GetLastError() << endl;
		cin.get();
		return 0;
	}

	for (int i = 0; i < 1024; i++)
	{
		DWORD dwBytesWrite;

		if (!WriteFile(hFile, buffer, BUFFER_LENGTH, &dwBytesWrite, &ovl))
		{
			DWORD dwRet = GetLastError();
			if (dwRet == ERROR_IO_PENDING)
			{
				//cout << "Write file pending \n";
			}
			else
			{
				cout << "Write file failed.Code error" << dwRet << endl;
				return 0;
			}
		}

		WaitForSingleObject(hFile, INFINITE);
		ovl.Offset += BUFFER_LENGTH;
	}

	//��������� ���������� �����
	CloseHandle(hFile);
	cout << "The file is written\n";

	clock_t finish = clock();
	return (finish - start);
}

clock_t AsyncFileRead()
{
	HANDLE hFile;
	OVERLAPPED ovl;

	clock_t start = clock();

	//������������� ������� ��������� OVERLAPPED
	ovl.Offset = 0;
	ovl.OffsetHigh = 0;
	ovl.hEvent = 0;

	hFile = CreateFile(
		L"async.txt",
		GENERIC_READ,
		FILE_SHARE_READ,		//���������� ������
		NULL,					// SD (�����. ������)
		OPEN_ALWAYS,			// ��������� ��� ������� ����� ����
		FILE_FLAG_OVERLAPPED,	//�������� �����
		NULL					//�����.������� �����
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		cout << "Error.Code = " << GetLastError() << endl;
		cin.get();
		return 0;
	}

	for (int i = 0; i < 1024; i++)
	{
		DWORD dwBytesWrite;

		if (!ReadFile(hFile, buffer, BUFFER_LENGTH, &dwBytesWrite, &ovl))
		{
			DWORD dwRet = GetLastError();
			if (dwRet == ERROR_IO_PENDING)
			{
				//cout << "Read file pending \n";
			}
			else
			{
				cout << "Read file failed.Code error" << dwRet << endl;
				return 0;
			}
		}

		WaitForSingleObject(hFile, INFINITE);
		ovl.Offset += BUFFER_LENGTH;
	}

	//��������� ���������� �����
	CloseHandle(hFile);
	cout << "The file is read\n";

	clock_t finish = clock();
	return (finish - start);
}
#pragma endregion

#pragma region Sync
clock_t SyncFileWrite()
{
	HANDLE hFile;

	clock_t start = clock();

	hFile = CreateFile(
		L"sync.txt",
		GENERIC_WRITE,
		FILE_SHARE_WRITE,		//���������� ������
		NULL,					// SD (�����. ������)
		CREATE_ALWAYS,			// ������� ����� ����
		FILE_ATTRIBUTE_NORMAL,	//�������� �����
		NULL					//�����.������� �����
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		cout << "Error.Code = " << GetLastError() << endl;
		cin.get();
		return 0;
	}

	for (int i = 0; i < 1024; i++)
	{
		DWORD dwBytesWrite;

		if (!WriteFile(hFile, buffer, BUFFER_LENGTH, &dwBytesWrite, NULL))
		{
			DWORD dwRet = GetLastError();
			if (dwRet == ERROR_IO_PENDING)
			{
				//cout << "Write file pending \n";
			}
			else
			{
				cout << "Write file failed.Code error" << dwRet << endl;
				return 0;
			}
		}
	}

	//��������� ���������� �����
	CloseHandle(hFile);
	cout << "The file is written\n";

	clock_t finish = clock();
	return (finish - start);
}

clock_t SyncFileRead()
{
	HANDLE hFile;

	clock_t start = clock();

	hFile = CreateFile(
		L"sync.txt",
		GENERIC_READ,
		FILE_SHARE_READ,		//���������� ������
		NULL,					// SD (�����. ������)
		OPEN_ALWAYS,			// ��������� ��� ������� ����� ����
		FILE_ATTRIBUTE_NORMAL,	//�������� �����
		NULL					//�����.������� �����
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		cout << "Error.Code = " << GetLastError() << endl;
		cin.get();
		return 0;
	}

	for (int i = 0; i < 1024; i++)
	{
		DWORD dwBytesWrite;

		if (!ReadFile(hFile, buffer, BUFFER_LENGTH, &dwBytesWrite, NULL))
		{
			DWORD dwRet = GetLastError();
			if (dwRet == ERROR_IO_PENDING)
			{
				//cout << "Read file pending \n";
			}
			else
			{
				cout << "Read file failed.Code error" << dwRet << endl;
				return 0;
			}
		}
	}

	//��������� ���������� �����
	CloseHandle(hFile);
	cout << "The file is read\n";

	clock_t finish = clock();
	return (finish - start);
}
#pragma endregion

int main()
{
	FillBuffer();

	cout << "Async write: " << AsyncFileWrite() << " ms\n";
	cout << "Async read: " << AsyncFileRead() << " ms\n";

	cout << "Sync write: " << SyncFileWrite() << " ms\n";
	cout << "Sync read: " << SyncFileRead() << " ms\n";

	system("pause");
	return 0;
}
