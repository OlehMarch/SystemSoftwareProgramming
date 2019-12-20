#include "stdafx.h"
#include "Timing.h"
#include "Windows.h"

__int64 Timing::GetUserTime() const {
	FILETIME creationTime;
	FILETIME exitTime;
	FILETIME kernelTime;
	FILETIME userTime;
	GetThreadTimes(GetCurrentThread(),
		&creationTime, &exitTime,
		&kernelTime, &userTime);
	__int64 curTime;
	curTime = userTime.dwHighDateTime;
	curTime <<= 32;
	curTime += userTime.dwLowDateTime;
	return curTime;
}

double Timing::GetCoreTime() const {
	FILETIME creationTime;
	FILETIME exitTime;
	FILETIME kernelTime;
	FILETIME userTime;
	GetThreadTimes(GetCurrentThread(),
		&creationTime, &exitTime,
		&kernelTime, &userTime);
	__int64 temp;
	temp = kernelTime.dwHighDateTime;
	temp <<= 32;
	temp += kernelTime.dwLowDateTime;
	double curTime = temp / 100000.0;
	return curTime;
}

void Timing::StartTiming() {
	m_userTime = GetUserTime();
}

void Timing::StopTiming() {
	__int64 curUserTime = GetUserTime();
	m_userTime = curUserTime - m_userTime;
}