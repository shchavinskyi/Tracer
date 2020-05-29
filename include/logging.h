#ifndef LOGGER_H
#define LOGGER_H

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include <array>
#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace Logging {

enum class LogLevel : uint8_t
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR
};

constexpr std::array<const char*, 5> levelStrings = {" \x1b[37;1m[TRACE]\x1b[0m ", " \x1b[34;1m[DEBUG]\x1b[0m ",
                                                     " \x1b[32;1m[INFO]\x1b[0m  ", " \x1b[33;1m[WARN]\x1b[0m  ",
                                                     " \x1b[31;1m[ERROR]\x1b[0m "};
constexpr std::size_t levelStringLength = 20;

// All, something in between, none or default to info
#if defined(LOGGING_LEVEL_DEBUG)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::DEBUG;
#elif defined(LOGGING_LEVEL_WARN)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::WARN;
#elif defined(LOGGING_LEVEL_ERROR)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::ERROR;
#elif defined(LOGGING_LEVEL_NONE)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::ERROR + 1;
#elif defined(LOGGING_LEVEL_INFO)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::INFO;
#else
#define LOGGING_LEVEL_ALL
#endif

inline std::size_t FillTimestamp(char* buffer)
{
    using namespace std::chrono;

    //
    // >>|23/05/20 19:34:13 997 |<< 22 characters + '\0'
    //
    constexpr std::size_t timestampLenght = 23;
    constexpr std::size_t fractionLenght = 5;
    constexpr std::size_t timeLenght = timestampLenght - fractionLenght;

    system_clock::time_point tp = system_clock::now();

    milliseconds ms = duration_cast<milliseconds>(tp.time_since_epoch());
    int32_t fractionalMS = ms.count() % 1000;
    std::time_t tt = system_clock::to_time_t(tp);

    tm newtime{};

#ifdef _MSC_VER
    localtime_s(&newtime, &tt);
#else
    localtime_r(&tt, &newtime);
#endif

    strftime(buffer, timeLenght, "%d/%m/%y %H:%M:%S", &newtime);
    snprintf(&buffer[timeLenght - 1], fractionLenght, " %03d", fractionalMS);

    return timestampLenght - 1;
}

template <typename... Args>
void Log(const LogLevel level, const char* message, Args... args)
{
#if !defined(LOGGING_LEVEL_ALL)
    // Cut off only if needed
    if (level < LOG_LEVEL_CUTOFF)
        return;
#endif

    constexpr uint32_t maxLogLength = 256;
    std::array<char, maxLogLength> buffer = {'\0'};

    std::size_t bufferIndex = FillTimestamp(&buffer.front()) - 1;

    strncpy(&buffer[bufferIndex], levelStrings[static_cast<std::size_t>(level)], levelStringLength);
    bufferIndex += levelStringLength;

    bufferIndex += std::size_t(snprintf(&buffer[bufferIndex], maxLogLength - bufferIndex - 2, message, args...));

    buffer[bufferIndex] = '\n';
    buffer[bufferIndex + 1] = '\0';

    std::cout << &buffer.front();
    std::cout.flush();
}

} // namespace Logging

template <typename... Args>
inline void TRACE(const char* message, Args... args)
{
    Logging::Log(Logging::LogLevel::TRACE, message, args...);
}

template <typename... Args>
inline void DEBUG(const char* message, Args... args)
{
    Logging::Log(Logging::LogLevel::DEBUG, message, args...);
}

template <typename... Args>
inline void LOG_INFO(const char* message, Args... args)
{
    Logging::Log(Logging::LogLevel::INFO, message, args...);
}

template <typename... Args>
inline void WARN(const char* message, Args... args)
{
    Logging::Log(Logging::LogLevel::WARN, message, args...);
}

template <typename... Args>
inline void ERROR(const char* message, Args... args)
{
    Logging::Log(Logging::LogLevel::ERROR, message, args...);
}

class ExecutionLogger
{
private:
    const char* message;
    std::chrono::high_resolution_clock::time_point start;

public:
    ExecutionLogger(const ExecutionLogger&) = delete;
    ExecutionLogger& operator=(const ExecutionLogger&) = delete;
    ExecutionLogger(ExecutionLogger&&) = delete;
    ExecutionLogger& operator=(ExecutionLogger&&) = delete;

    ExecutionLogger(const char* inMessage)
        : message(inMessage)
        , start(std::chrono::high_resolution_clock::now())
    {
    }

    ~ExecutionLogger()
    {
        using namespace std::chrono;

        high_resolution_clock::time_point stop = high_resolution_clock::now();

        milliseconds duration = duration_cast<milliseconds>(stop - start);

        TRACE("%s takes %ld msec", message, duration.count());
    }
};

#define TRACE_EXECUTION(message) const ExecutionLogger executionLogger(message)

#endif // LOGGER_H
