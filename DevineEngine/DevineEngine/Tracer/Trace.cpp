#include "Trace.hpp"
#include <iostream>
#include <Windows.h>
#ifdef _DEBUG
bool _trace(const wchar_t* const format, ...)
{
	////set console
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	//file stream for freopen_s
	FILE *stream;
	wchar_t buffer[1000];
	///*std::wcout << format;*/
	////write to output
	//char* argptr;
	//va_start(argptr, format);
	//wvsprintf(buffer, format, argptr);
	//va_end(argptr);
	/*OutputDebugString(buffer);*/

	freopen_s(&stream,"CON","w",stdout);
	//
	//std::wcout << buffer;
	std::wcout << format;
	return true;
}
#endif
