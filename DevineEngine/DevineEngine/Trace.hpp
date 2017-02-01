#pragma once
//My trace Header
#ifdef _DEBUG
bool _trace(const wchar_t* const format,...);
#define TRACE _trace
#else
#define TRACE false && _trace
#endif