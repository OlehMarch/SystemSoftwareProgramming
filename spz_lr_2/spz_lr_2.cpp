// spz_lr_2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "fstream"
#include "Windows.h"
#include "string"

using namespace std;


// File Copy By C
void CFileCopy(char* newFileName, bool empty = true)
{
	FILE *InFile, *OutFile;
	char *Buffer = new char[256];
	size_t bytesIn, bytesOut;

	fopen_s(&InFile, (empty) ? "stock.file" : "stock2.file", "rb");

	fopen_s(&OutFile, newFileName, "wb");

	while ((bytesIn = fread(Buffer, 1, 256, InFile)) > 0) {
		bytesOut = fwrite(Buffer, 1, bytesIn, OutFile);
		if (bytesOut != bytesIn) {
			perror("Fatal write error.");
			return;
		}
	}

	fclose(InFile);
	fclose(OutFile);
}
// File Copy by WinApi
void WAFileCopy(char* newFileName, bool empty = true)
{
	HANDLE InFIle, OutFile;
	DWORD InFileBytesRead, OutFileBytesRead;
	char* Buffer = new char[256];

	InFIle = CreateFile((empty) ? L"stock.file" : L"stock2.file", GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (InFIle == INVALID_HANDLE_VALUE) {
		printf("Cannot open input file. Error: %x\n", GetLastError());
		return;
	}

	OutFile = CreateFile((LPCWSTR)newFileName, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (OutFile == INVALID_HANDLE_VALUE) {
		printf("Cannot open output file. Error: %x\n", GetLastError());
		return;
	}

	while (ReadFile(InFIle, Buffer, 256, &InFileBytesRead, NULL) && InFileBytesRead > 0) {
		WriteFile(OutFile, Buffer, InFileBytesRead, &OutFileBytesRead, NULL);
		if (InFileBytesRead != OutFileBytesRead) {
			printf("Fatal write error: %x\n", GetLastError());
			return;
		}
	}

	CloseHandle(InFIle);
	CloseHandle(OutFile);
}
// File Copy by WinApi func CopyFile
void FileCopy(char* newFileName, bool empty = true)
{
	CopyFile((empty) ? L"stock.file" : L"stock2.file", (LPCWSTR)newFileName, false);
}

void AllCopyFileFunctions(int counter)
{
	/*
	if (counter < 3)
	{
		cout << "-- Small file --\n";
	}
	else
	{
		cout << "-- Large file --\n";
	}
	*/

	switch (counter)
	{
	case 0:
		cout << "C  ";
		CFileCopy("CFileCopyEmpty.file");
		break;
	case 1:
		cout << "WA ";
		WAFileCopy("WAFileCopyEmpty.file");
		break;
	case 2:
		cout << "CF ";
		FileCopy("FileCopyEmpty.file");
		break;
	case 3:
		cout << "C  ";
		CFileCopy("CFileCopy.file", false);
		break;
	case 4:
		cout << "WA ";
		WAFileCopy("WAFileCopy.file", false);
		break;
	case 5:
		cout << "CF ";
		FileCopy("FileCopy.file", false);
		break;
	}
}

double get_wall_time() {
	LARGE_INTEGER time, freq;
	if (!QueryPerformanceFrequency(&freq)) {
		//  Handle error
		return 0;
	}
	if (!QueryPerformanceCounter(&time)) {
		//  Handle error
		return 0;
	}
	return (double)time.QuadPart / freq.QuadPart;
}
double get_cpu_time() {
	FILETIME a, b, c, d;
	if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
		//  Returns total user time.
		//  Can be tweaked to include kernel times as well.
		return
			(double)(d.dwLowDateTime |
			((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
	}
	else {
		//  Handle error
		return 0;
	}
}

void main()
{
	/*double start = .0, end = .0;
	cout << "-- file with few data --" << endl;

	start = (double)timeGetTime();
	CFileCopy("CFileCopyEmpty.file");
	end = (double)timeGetTime() - start;
	cout << "C  Time: " << end / 1000 << endl;

	start = (double)timeGetTime();
	WAFileCopy("WAFileCopyEmpty.file");
	end = (double)timeGetTime() - start;
	cout << "WA Time: " << end / 1000 << endl;

	start = (double)timeGetTime();
	FileCopy("FileCopyEmpty.file");
	end = (double)timeGetTime() - start;
	cout << "FC Time: " << end / 1000 << endl;
	////////////////////////////////////////////////////
	cout << "-- file with large data --" << endl;

	start = (double)timeGetTime();
	CFileCopy("CFileCopy.file", false);
	end = (double)timeGetTime() - start;
	cout << "C  Time: " << end / 1000 << endl;

	start = (double)timeGetTime();
	WAFileCopy("WAFileCopy.file", false);
	end = (double)timeGetTime() - start;
	cout << "WA Time: " << end / 1000 << endl;

	start = (double)timeGetTime();
	FileCopy("FileCopy.file", false);
	end = (double)timeGetTime() - start;
	cout << "FC Time: " << end / 1000 << endl;
	*/

	//  Start Timers
	double wall0 = get_wall_time();
	double cpu0 = get_cpu_time();

	AllCopyFileFunctions(4);

	//  Stop timers
	double wall1 = get_wall_time();
	double cpu1 = get_cpu_time();

	cout << "Wall Time = " << (wall1 - wall0) * 1000 << " ms" << endl;
	cout << "   CPU Time  = " << (cpu1 - cpu0) * 1000 << " ms" << endl;

	system("pause");
}
