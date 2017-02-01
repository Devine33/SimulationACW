#pragma once
#include <Windows.h>
#include <chrono>
class Time
{
public:
	Time();
	~Time();
	void StartTime();
	void StartQuery();
	void EndQuery();
	void EndTime();
	void GetElapsed();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> Start, End;
	std::chrono::duration<double> elapsed_seconds;

	
	LARGE_INTEGER li;
	double freq = 0.0;
	__int64 start;
};

