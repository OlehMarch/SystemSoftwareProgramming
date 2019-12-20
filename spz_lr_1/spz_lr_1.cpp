// spz_lr_1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include <windows.h>

using namespace std;

int ExceptionFilter(int code)
{
	if (code == EXCEPTION_FLT_DIVIDE_BY_ZERO)
		return EXCEPTION_EXECUTE_HANDLER;
	return EXCEPTION_CONTINUE_EXECUTION;
}


void main()
{
	__try
	{
		__try
		{
			RaiseException(EXCEPTION_FLT_DIVIDE_BY_ZERO, NULL, NULL, NULL);
		}
		__finally
		{
			cout << "Block finally was executed!\n";
		}
	}
	__except (ExceptionFilter(GetExceptionCode()))
	{
		cout << "Exception was generated!" << endl;
	}

	system("pause");
}

