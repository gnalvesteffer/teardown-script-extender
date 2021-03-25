#include <iostream>

#define no_init_all
#include <Windows.h>
#include <iomanip>

#include "Config.h"

#define WriteLog(logType, ...) Logger::LogInternal(__func__, __FILE__, logType, __VA_ARGS__)
#define WriteSeparator(len) Logger::SeparatorInternal(len);

enum class LogType
{
	Generic = 1,
	Address = 2,
	Error = 3,
	Special = 4 // Don't get removed by ERRORS_ONLY (Only used by the loader message)
};

namespace Logger
{
	inline void SeparatorInternal(int len)
	{
		std::cout << std::setfill('=') << std::setw(len) << "\n" << std::setfill(' ');
;	}

	template<char const*... Args_t>
	inline void LogInternal(const char* funcName, const char* fullFileName, LogType type, const char* fmt, ...)
	{
		char fileName[255];
		char ext[20];
		_splitpath(fullFileName, NULL, NULL, fileName, ext);

		char buff[4096];
		va_list args;
		va_start(args, fmt);

		int rc = vsnprintf(buff, sizeof(buff), fmt, args);
		va_end(args);

		switch (type)
		{
#if !PRINT_ERRORS_ONLY
		case LogType::Generic:
			std::cout << "\033[97;44;1m[" << funcName << " @ " << fileName << ext << "]\033[0m " << buff << std::endl;
			break;
	#if PRINT_ADDRESSES
		case LogType::Address:
			std::cout << "\033[30;103;1m[" << "ADDRESS" << " @ " << fileName << ext << "]\033[0m " << buff << std::endl;
			break;
	#endif
		case LogType::Error:
			std::cout << "\033[30;41;1m[" << funcName << " @ " << fileName << ext << "]\033[0m " << buff << std::endl;
			break;
#endif
		case LogType::Special:
			std::cout << "\033[97;44;1m[" << funcName << " @ " << fileName << ext << "]\033[0m " << buff << std::endl;
			break;
		}
	}
}