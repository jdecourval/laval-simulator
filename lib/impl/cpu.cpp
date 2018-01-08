#include "cpu.h"

#include "answer.h"
#include "opcodes.h"
#include "thread_pool.h"


using namespace std::chrono_literals;

Cpu::Cpu(const Settings& settings)
    : Cpu(settings, Memory{settings}, std::vector<MemoryInterface::size_type>(std::accumulate(settings.dimensions.begin(), settings.dimensions.end(), 1ul, std::multiplies<>())))
{
    assert(settings.dimensions.size() == 3);
}

Cpu::Cpu(const Settings& settings, Memory&& memory, std::vector<MemoryInterface::size_type>&& core_to_mem_map, std::vector<std::vector<std::pair<int, int>>>&& parameters)
    : mem{std::move(memory)}
    , cores{std::vector<std::remove_cv_t<std::remove_reference_t<decltype(settings.dimensions.at(0))>>>(std::cbegin(settings.dimensions), std::cend(settings.dimensions)), mem}
    , core_to_mem_map{std::move(core_to_mem_map)}
    , parameters{std::move(parameters)}
{
    assert(settings.dimensions.size() == 3);

    // TODO: Check parameters
}

void Cpu::link_memory(Memory&& memory, std::vector<MemoryInterface::size_type>&& core_to_mem_map)
{
    mem = std::move(memory);
    this->core_to_mem_map = std::move(core_to_mem_map);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

uint8_t Cpu::start(const std::chrono::milliseconds& period, const std::vector<uint8_t>& args)
{
    assert(period.count() >= 0);
    assert(cores.size() == core_to_mem_map.size());
    assert(args.size() == parameters.size());

    // Load args
    for (auto i = 0u; i < args.size(); i++)
    {
        for (auto& [block, instruction_index]: parameters.at(i))
        {
            auto& instruction_raw = mem.at(block).at(instruction_index);
            assert(cores.size() > 0);
            auto instruction = cores[0].get_factory().create(instruction_raw);

            if (dynamic_cast<OpCodes::LCL*>(instruction.get()))
            {
//                instruction->load_args(std::vector{static_cast<uint8_t>(args.at(i) & 0xff)});
                instruction_raw += args.at(i) & 0xff;
            }
            else if (dynamic_cast<OpCodes::LCH*>(instruction.get()))
            {
//                instruction->load_args(std::vector{static_cast<uint8_t>(args.at(i) >> 4)});
                instruction_raw += args.at(i) >> 4;
            }
            else
            {
                throw std::runtime_error("This version only support arguments on load instructions");
            }
        }
    }

    // Wire cores
    for(auto i = 0ull; i < core_to_mem_map.size(); i++)
    {
        assert(core_to_mem_map[i] < mem.banks_number());
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

#pragma clang diagnostic pop

// TODO: Calibrate function
