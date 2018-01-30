#include "cpu.h"

#include "answer.h"
#include "opcodes.h"
#include "thread_pool.h"


using namespace std::chrono_literals;

Cpu::Cpu(const Settings& settings)
    : Cpu(settings, Memory{settings})
{

}

Cpu::Cpu(const Settings& settings, Memory&& memory)
    : mem{std::move(memory)}
    , cores{std::vector<std::remove_cv_t<std::remove_reference_t<decltype(settings.dimensions.at(0))>>>(std::cbegin(settings.dimensions), std::cend(settings.dimensions)), mem, inputs}
    , core_to_mem_map{settings.core_to_mem_map}
    , outputs{settings.outputs}
{
    cpu_assert(settings.dimensions.size() == 3, "Incorrect number of dimensions");

    // TODO: Check parameters

    for(auto i: settings.inputs)
    {
        inputs.emplace(i, Input());
    }
}

void Cpu::link_memory(Memory&& memory, const Settings& settings)
{
    mem = std::move(memory);
    this->core_to_mem_map = settings.core_to_mem_map;
    // TODO: Add inputs
}

uint8_t Cpu::start(const std::chrono::milliseconds& period)
{
    cpu_assert(period.count() >= 0, "Invalid period");
    cpu_assert(cores.size() == core_to_mem_map.size(), "Non-matching number of cores and number of entries in core to memory map");

    // Wire cores
    for(auto i = 0ull; i < core_to_mem_map.size(); i++)
    {
        cpu_assert(core_to_mem_map[i] < mem.banks_number(), "Core to memory map contains an out of range core");
        cores[i].wire(core_to_mem_map[i]);      // TODO: Not yet initialized
    }

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
    ThreadPool<1> pool;

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

        for(auto& output_id: outputs)
        {
            if (auto value = cores[output_id].get_from(false))
            {
                std::cout << static_cast<int>(value->second) << std::endl;
            }
        }

        pool.apply(std::begin(cores), std::end(cores), [](auto& core){ core.preload(); });

        try
        {
            pool.apply(std::begin(cores), std::end(cores), [](auto& core){ core.fetch(); });
        }
        catch (const Answer& answer)
        {
            return answer.content;
        }

        if (period > 0s)
        {
            // TODO: Replace OpCodes::DBG by a local implementation
            std::for_each(std::begin(cores), std::end(cores), [](auto& core) { core.execute(OpCodes::DBG()); });

            auto time_to_sleep = period - (BenchmarkClock::now() - time_before_execution);

            if (time_to_sleep > 0ns)
            {
                std::this_thread::sleep_for(time_to_sleep);
            }
        }

        loops++;
    }
}


// TODO: Calibrate function
