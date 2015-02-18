#include "vesp/Log.hpp"
#include "vesp/Util.hpp"

#include <cstdarg>
#include <ctime>

namespace vesp
{
#define LOG_TYPE(type) #type
	const char* LogTypeStrings[] = { VESP_LOG_TYPES };
#undef LOG_TYPE

	Logger::Logger(StringPtr path)
	{
		this->logFile_ = FileSystem::Get()->Open(path, "a");
	}

	Logger::~Logger()
	{
		FileSystem::Get()->Close(this->logFile_);
	}

	void Logger::WriteLog(LogType type, StringPtr fmt, ...)
	{
		static StringByte tempBuffer[4096];
		static StringByte finalBuffer[4500];
		static StringByte timeBuffer[32];

		time_t rawTime;
		tm timeInfo;

		time(&rawTime);
		localtime_s(&timeInfo, &rawTime);

		va_list args;
		va_start(args, fmt);

		std::strftime(timeBuffer, util::SizeOfArray(timeBuffer), 
			"%H:%M:%S", &timeInfo);

		vsnprintf_s(tempBuffer, util::SizeOfArray(tempBuffer), fmt, args);

		sprintf_s(finalBuffer, "%s | %s | %s\n", 
			timeBuffer, LogTypeStrings[static_cast<U8>(type)], tempBuffer);

		auto len = strlen(finalBuffer);

		fwrite(finalBuffer, 1, len, stdout);
		this->logFile_.Write(ArrayView<U8>(reinterpret_cast<U8*>(finalBuffer), len));
		this->logFile_.Flush();
	}
}