#include "Time.h"
#include <Windows.h>
#include <string>


Time::Time(): start(0)
{
}


Time::~Time()
{
}

void Time::StartTime()
{
	Start = std::chrono::high_resolution_clock::now();
	//OutputDebugString(L"Start Time \n");
	//OutputDebugString(std::to_wstring(Start.time_since_epoch().count()).c_str());

}
//
//void Time::StartQuery()
//{
//	QueryPerformanceCounter(&li);
//	freq = double(li.QuadPart);
//
//	QueryPerformanceCounter(&li);
//
//	start = li.QuadPart;
//
//	OutputDebugString(L"Start Time \n");
//	OutputDebugString(std::to_wstring(start).c_str());
//	OutputDebugString(L"\n");
//}

//void Time::EndQuery()
//{
//	QueryPerformanceCounter(&li);
//	__int64 stop = li.QuadPart;
//	double elapsedTime = double(stop - start);
//
//	OutputDebugString(L"Stop Time \n");
//	OutputDebugString(std::to_wstring(stop).c_str());
//	OutputDebugString(L"\n");
//
//	OutputDebugString(L"Elapsed Time \n");
//	OutputDebugString(std::to_wstring(elapsedTime).c_str());
//	OutputDebugString(L"\n");
//}

void Time::EndTime()
{
	End = std::chrono::high_resolution_clock::now();

	//OutputDebugString(L" \n End Time \n");
	/*OutputDebugString(std::to_wstring(End.time_since_epoch().count()).c_str());
	OutputDebugString(L"\n");*/
}
//gets the elapsed time the application is running for
void Time::Elapsed()
{
	elapsed_seconds = End - Start;
	OutputDebugString(L" \n Total Elapsed Time \n");
	OutputDebugString(std::to_wstring(elapsed_seconds.count()).c_str());
}
//returns the total time the application has been running for
void Time::TotalRunningTime()
{
	Current = std::chrono::high_resolution_clock::now() - Start;
	OutputDebugString(L" \n Total Running Time \n");
	OutputDebugString(std::to_wstring(Current.count()).c_str());
}

void Time::DeltaTime()
{
	/*auto elapsed = (End - Start) / 1000;*/
	auto elapsed = (End - Start) / 1000; 
	//sets delta time
	dt = elapsed;
	OutputDebugString(L" \n DELTATIME \n");
	OutputDebugString(std::to_wstring(dt.count()).c_str());
	OutputDebugString(std::to_wstring(elapsed.count()).c_str());
	End = Start;
}
//returns the deltatime as a float
float Time::GetDeltaTime() const
{
	//returns a normal value 
	return std::chrono::high_resolution_clock::now().time_since_epoch().count() - Start.time_since_epoch().count();
}

float Time::GetTotalRunningTime() const
{
	double total = Current.count();
	return total;
}
