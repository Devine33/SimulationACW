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
	double GetTime();
	double CalcFrameRate();
	int getDelta();
	void anotherDelta();
	double RetDT();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> Start, End;
	std::chrono::duration<double> elapsed_seconds,Current,dt;
	using ms = std::chrono::duration<float, std::milli>;
	double DT = 0;
	double D_T = 0;
	// This is the current time in seconds
	double CurrentTime = 0;
	LARGE_INTEGER li;
	/*float dt;*/
	double freq = 0.0;
	__int64 start;
};

