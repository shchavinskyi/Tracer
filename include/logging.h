
#ifndef LOGGER_H
#define LOGGER_H

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

constexpr std::array<const char*, 5> levelStrings = {{" \x1b[37;1m[TRACE]\x1b[0m ", " \x1b[34;1m[DEBUG]\x1b[0m ",
                                                      " \x1b[32;1m[INFO]\x1b[0m ", " \x1b[33;1m[WARN]\x1b[0m ",
                                                      " \x1b[31;1m[ERROR]\x1b[0m "}};

#define LOGGING_LEVEL_ALL

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
#endif

inline std::string GetTimestamp()
{
    using namespace std::chrono;

    std::string timestamp;
    constexpr size_t timestampLenght = 23;
    constexpr size_t fractionLenght = 5;

    timestamp.resize(timestampLenght - fractionLenght);

    system_clock::time_point tp = system_clock::now();

    milliseconds ms = duration_cast<milliseconds>(tp.time_since_epoch());
    std::size_t fractionalMS = ms.count() % 1000;
    std::time_t tt = system_clock::to_time_t(tp);

    tm newtime;

#ifdef _MSC_VER
    localtime_s(&newtime, &tt);
#else
    localtime_r(&tt, &newtime);
#endif

    strftime(&timestamp.front(), timestamp.size(), "%d/%m/%y %H:%M:%S", &newtime);

    timestamp.append(std::to_string(fractionalMS));

    return timestamp;
}

class Logger
{
public:
    Logger() {}

    template <typename... Args>
    void Log(const LogLevel level, const std::string& message, Args... args)
    {
#if !defined(LOGGING_LEVEL_ALL) && !defined(LOGGING_LEVEL_TRACE)
        // Cut off only if needed
        if (level < LOG_LEVEL_CUTOFF)
            return;
#endif

        std::string format("%s%s");
        format.append(message);

        constexpr size_t maxLogLength = 256;
        char buffer[maxLogLength] = {'\0'};

#ifdef _MSC_VER
        int len = sprintf_s(buffer, maxLogLength - 2, format.c_str(), GetTimestamp().c_str(),
                            levelStrings[static_cast<std::size_t>(level)], args...);
#else
        int len = sprintf(buffer, format.c_str(), GetTimestamp().c_str(), levelStrings[static_cast<std::size_t>(level)],
                          args...);
#endif

        buffer[len] = '\n';
        buffer[len + 1] = '\0';

        std::cout << buffer;
        std::cout.flush();
    }
};

inline Logger& GetLogger()
{
    static Logger loggerSingleton;
    return loggerSingleton;
}

} // namespace Logging

template <typename... Args>
inline void Log(const Logging::LogLevel level, const std::string& message, Args... args)
{
    Logging::GetLogger().Log(level, message, args...);
}

template <typename... Args>
inline void TRACE(const std::string& message, Args... args)
{
    Logging::GetLogger().Log(Logging::LogLevel::TRACE, message, args...);
}

template <typename... Args>
inline void DEBUG(const std::string& message, Args... args)
{
    Logging::GetLogger().Log(Logging::LogLevel::DEBUG, message, args...);
}

template <typename... Args>
inline void INFO(const std::string& message, Args... args)
{
    Logging::GetLogger().Log(Logging::LogLevel::INFO, message, args...);
}

template <typename... Args>
inline void WARN(const std::string& message, Args... args)
{
    Logging::GetLogger().Log(Logging::LogLevel::WARN, message, args...);
}

template <typename... Args>
inline void ERROR(const std::string& message, Args... args)
{
    Logging::GetLogger().Log(Logging::LogLevel::ERROR, message, args...);
}

class ExecutionLogger
{
private:
    std::string message;
    std::chrono::high_resolution_clock::time_point start;

public:
    ExecutionLogger(const std::string& inMessage)
        : message(inMessage)
        , start(std::chrono::high_resolution_clock::now())
    {
    }
    ~ExecutionLogger()
    {
        using namespace std::chrono;

        high_resolution_clock::time_point stop = high_resolution_clock::now();

        milliseconds duration = duration_cast<milliseconds>(stop - start);

        TRACE("%s takes %ld msec", message.c_str(), duration.count());
    }
};

#define TRACE_EXECUTION(message) const ExecutionLogger executionLogger(message)

#endif // LOGGER_H
