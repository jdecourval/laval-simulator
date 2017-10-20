#include "cpu.h"

#include <thread>


using namespace std::chrono_literals;

Cpu::Cpu(const Settings& settings)
    : mem{settings.bank_number, settings.bank_size}
      , cores{settings.dimensions, mem}
{
    running = false;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void Cpu::Start(const std::chrono::milliseconds& period)
{
    assert(period.count() >= 0);

    if (period.count() != 0)
    {
        std::cout << "starting cpu at " << 1000 / period.count() << " Hz" << std::endl;
    }

    auto time_before_execution = BenchmarkClock::now();
    auto last_report_time = BenchmarkClock::now();
    auto loops = 0ull;

    while (true)
    {
        time_before_execution = BenchmarkClock::now();
        auto time_since_report = time_before_execution - last_report_time;

        if (time_since_report >= 3s)
        {
            std::cout << "Real period: " << 1000.0 * 1000 * 1000 * loops / time_since_report.count() << " Hz" << std::endl;
            last_report_time = time_before_execution;
            loops = 0;
        }

        for (auto& core: cores)
        {
            core.preload();
        }

        for (auto& core: cores)
        {
            core.fetch();
        }

        if (period > 0s)
        {
            auto time_to_sleep = period - (BenchmarkClock::now() - time_before_execution);

            if (time_to_sleep > 0ns)
            {
                std::this_thread::sleep_for(time_to_sleep);
            }
        }

        loops++;
    }
}

#pragma clang diagnostic pop

// TODO: Calibrate function
