#pragma once

class Timing {
public:
	void StartTiming();
	void StopTiming();
	double GetUserSeconds() const {
		return double(m_userTime) / 100000.0;
	}
	double GetCoreTime() const;

private:
	__int64 GetUserTime() const;
	__int64 m_userTime;
};