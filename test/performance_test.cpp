#include "logging.h"
#include "test.h"

#include <thread>

TEST_CASE("logger single-thread performance")
{
    TRACE_EXECUTION("Logging single thread performance");

    for (int i = 0; i < 10000; ++i)
    {
        LOG_INFO("string %s int %d", "string", 100);

        Log(Logging::LogLevel::ERROR, "error sample %s", "string");
    }
}

TEST_CASE("logger multi-thread performance")
{
    TRACE_EXECUTION("Logging multi-thread performance");

    unsigned int threadCount = std::thread::hardware_concurrency();

    auto logFunction = [](unsigned int threadNumber) {
        for (int i = 0; i < 1000; ++i)
        {
            LOG_INFO("[%d] string %s int %d", threadNumber, "string", 100);

            Log(Logging::LogLevel::ERROR, "[%d] error sample %s", threadNumber, "string");
        }
    };

    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    for (unsigned int i = 0; i < threadCount; ++i)
    {
        threads.emplace_back(std::thread(logFunction, threadCount));
    }

    for (unsigned int i = 0; i < threadCount; ++i)
    {
        threads[i].join();
    }
}
