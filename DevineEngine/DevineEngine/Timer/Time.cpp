#include "Time.h"
#include <string>
#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;

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

double Time::GetTime()
{
	// Grab the current system time since 1/1/1970, otherwise know as the Unix Timestamp or Epoch
	auto beginningOfTime = std::chrono::system_clock::now().time_since_epoch();

	// Convert the system time to milliseconds
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(beginningOfTime).count();

	// Return the time in seconds and give us a fractional value (important!)
	return ms * 0.001;
}

double Time::CalcFrameRate()
{
	// Below we create a bunch of static variables to track the deltas of the time.

	static double framesPerSecond = 0.0f;								// This will store our fps
	static double startTime = GetTime();								// This will hold the time per second to test against
	static double lastTime = GetTime();									// This will hold the time from the last frame
	static char strFrameRate[50] = { 0 };								// We will store the string here for the window title
	static double currentFPS = 0.0f;									// This stores the current frames per second

																		// Grab the current time in seconds from our environment's time function wrapped in our WindowManager	
	CurrentTime = GetTime();

	// Calculate our delta time, which is the time that has elapsed since the last time we checked the time
	DT = CurrentTime - lastTime;

	// Now store the lastTime as the currentTime to then get the time passed since the last frame
	lastTime = CurrentTime;

	// Increase the frame counter
	++framesPerSecond;

	// Now we want to subtract the current time by the start time that was stored.  If it is greater than 1
	// that means a second has passed and we need to display the new frame rate.  The 1.0 represents 1 second.  
	// Let's say we got 12031 (12.031) from GetTime() for the CurrentTime, and the startTime had 11230 (11.230).  
	// Well, 12.031 - 11.230 = 0.801, which is NOT a full second.  So we try again the next frame.  Once the 
	// CurrentTime - startTime comes out to be > 1 second, we calculate the frames for this last second.
	if (CurrentTime - startTime > 1.0f)
	{
		// Here we set the startTime to the currentTime.  This will be used as the starting point for the next second.
		// This is because GetTime() counts up, so we need to create a delta that subtract the current time from.
		startTime = CurrentTime;

		// Show the frames per second in the console window if desired
		// Store the current FPS since we reset it down below and need to store it to return it
		currentFPS = framesPerSecond;

		// Reset the frames per second
		framesPerSecond = 0;
	}

	// Return the most recent FPS
	//std::cout << currentFPS << "\n";
	return currentFPS;
}

int Time::getDelta()
{
	std::cout << "Measurement resolution: " <<
		duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::duration(1)).count()
		<< "ns" << std::endl;

	std::cout << "Will now tell thread to sleep for 1ms and measure that in actual time" << std::endl;

	auto start = std::chrono::high_resolution_clock::now();

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	auto end = std::chrono::high_resolution_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

	std::cout << "Measured time: " << dur.count() << "ns" << std::endl;
	D_T = std::chrono::duration_cast<std::chrono::nanoseconds>(dur - std::chrono::milliseconds(1)).count();
	std::cout << "Delta: " <<
	D_T
		<< "ns" << std::endl;

	return 0;
}

void Time::anotherDelta()
{
	using steady_clock = std::chrono::steady_clock;
	using delta_t = std::chrono::milliseconds;

	static_assert(steady_clock::is_steady, "");
	static_assert(delta_t::period::den <= steady_clock::period::den, "");
	auto delta = delta_t::rep{};

	std::this_thread::sleep_for(1s);
	auto const start = steady_clock::now();
	auto const duration = (steady_clock::now() - start);
	delta = std::chrono::duration_cast<delta_t>(duration).count();
	std::cout << "this delta = " << delta;
}

double Time::RetDT()
{
	return D_T;
}
