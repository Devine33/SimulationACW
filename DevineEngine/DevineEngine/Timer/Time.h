#pragma once
#include <Windows.h>
#include <chrono>
typedef std::chrono::duration<long, std::ratio<1, 60>> sixtieths_of_a_sec;
constexpr auto kMaxDeltatime = sixtieths_of_a_sec{ 1 };
class Time
{
public:
	Time();
	~Time();
	void StartTime();
	/*void StartQuery();*/
	/*void EndQuery();*/
	void EndTime();
	void Elapsed();
	void TotalRunningTime();
	void DeltaTime();
	float GetDeltaTime() const;
	float GetTotalRunningTime() const;
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> Start, End;
	std::chrono::duration<double> elapsed_seconds,Current,dt;
	
	LARGE_INTEGER li;
	/*float dt;*/
	double freq = 0.0;
	__int64 start;
};

