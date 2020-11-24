#pragma //once
#include <iostream>
#define no_init_all
#include <Windows.h>
#include <iomanip>

#define WriteLog(...) Logger::LogInternal(__func__, __FILE__, __VA_ARGS__)
#define WriteError(...) Logger::ErrorInternal(__func__, __FILE__, __VA_ARGS__)
#define WriteSeparator(len) Logger::SeparatorInternal(len);

namespace Logger
{
	inline void SeparatorInternal(int len)
	{
		std::cout << std::setfill('=') << std::setw(len) << "\n" << std::setfill(' ');
;	}

	template<char const*... Args_t>
	inline void ErrorInternal(const char* funcName, const char* fullFileName, const char* fmt, ...)
	{
		char fileName[255];
		char ext[20];
		_splitpath(fullFileName, NULL, NULL, fileName, ext);

		char buff[4096];
		va_list args;
		va_start(args, fmt);

		int rc = vsnprintf(buff, sizeof(buff), fmt, args);
		va_end(args);

		std::cout << "\033[30;41;1m[" << funcName << " @ " << fileName << ext << "]\033[0m " << buff << std::endl;
	}

	template<char const*... Args_t>
	inline void LogInternal(const char* funcName, const char* fullFileName, const char* fmt, ...)
	{
		char fileName[255];
		char ext[20];
		_splitpath(fullFileName, NULL, NULL, fileName, ext);

		char buff[4096];
		va_list args;
		va_start(args, fmt);

		int rc = vsnprintf(buff, sizeof(buff), fmt, args);
		va_end(args);

		std::cout << "\033[97;44;1m[" << funcName << " @ " << fileName << ext << "]\033[0m " << buff << std::endl;
	}
}