#include "cpu.h"

#include "thread_pool.h"


using namespace std::chrono_literals;

Cpu::Cpu(const Settings& settings)
    : Cpu(settings, Memory{settings.bank_number, settings.bank_size})
{

}

Cpu::Cpu(const Cpu::Settings& settings, Memory&& memory)
    : mem(memory)
      , cores{settings.dimensions, mem}
{

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
    else
    {
        std::cout << "starting cpu at max speed" << std::endl;
    }

    auto time_before_execution = BenchmarkClock::now();
    auto last_report_time = BenchmarkClock::now();
    auto loops = 0ull;
    ThreadPool<16> pool;

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

        pool.apply(std::begin(cores), std::end(cores), [](auto& core){ core.preload(); });
        pool.apply(std::begin(cores), std::end(cores), [](auto& core){ core.fetch(); });

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
