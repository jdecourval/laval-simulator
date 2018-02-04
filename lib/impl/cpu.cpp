#include "cpu.h"

#include "answer.h"
#include "opcodes.h"
#include "thread_pool.h"

#include <future>


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
        inputs.emplace(i, input_lock);
    }
}

void Cpu::link_memory(Memory&& memory, const Settings& settings)
{
    mem = std::move(memory);
    this->core_to_mem_map = settings.core_to_mem_map;
    // TODO: Add inputs
}


void Cpu::handle_output(std::ostream& output)
{
    auto first = true;

    for (auto& output_id: outputs)
    {
        if (auto value = cores[output_id].get_from(false))
        {
            if (first)
            {
                first = false;
            }
            else
            {
                output << " ";
            }
            output << output_id << "," << static_cast<int>(value->second);
        }
    }

    if (!first)
    {
        output << std::endl;
    }
}

void Cpu::handle_input(std::istream& input, std::atomic<bool>& stop_signal, std::exception_ptr& thread_exception)
{

    while(!stop_signal)
    {
        try
        {
            std::unordered_map<size_t, uint8_t> queue;

            while (true)
            {
                auto core_id = 0ull;
                auto value = 0;
                auto comma = '\0';

                if (!(input >> core_id >> comma >> value))
                {
                    cpu_assert(false, "Input error: " << std::strerror(errno));
                }

                auto delimiter = input.get();

                cpu_assert(inputs.count(core_id), "No such core: " << core_id);
                cpu_assert(comma == ',', "Unexpected input");

                cpu_assert(value <= 0xff, "Too large input");
                cpu_assert(value >= 0, "Only unsigned values are supported");
                queue.emplace(core_id, value);

                if (delimiter == '\n')
                {
                    break;
                }

                if (input.eof())
                {
                    stop_signal = true;
                    return;
                }

                cpu_assert(delimiter == ' ', "Unexpected input");
            }

            std::lock_guard lock(input_lock);
            for (auto [core_id, value]: queue)
            {
                inputs.at(core_id).put(value);
            }
        }
        catch(...)
        {
            thread_exception = std::current_exception();
            stop_signal = true;
        }
    }

}

uint8_t Cpu::start(std::istream& input, std::ostream& output, const std::chrono::milliseconds& period)
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

    std::atomic<bool> stop_signal{};
    std::exception_ptr thread_exception{};
    auto input_handler = std::thread([this, &input, &stop_signal, &thread_exception]() { return handle_input(input, stop_signal, thread_exception); });

    auto time_before_execution = BenchmarkClock::now();
    auto last_report_time = BenchmarkClock::now();
    auto loops = 0ull;

    try
    {

        while (!stop_signal)
        {
            time_before_execution = BenchmarkClock::now();
            auto time_since_report = time_before_execution - last_report_time;

            if (time_since_report >= 3s)
            {
//                std::cerr << "Real period: " << 1000.0 * 1000 * 1000 * loops / time_since_report.count() << " Hz"
//                          << std::endl;
                last_report_time = time_before_execution;
                loops = 0;
            }

            handle_output(output);

            std::for_each(std::begin(cores), std::end(cores), [](auto& core)
            { core.preload(); });

            try
            {
                std::for_each(std::begin(cores), std::end(cores), [](auto& core)
                { core.fetch(); });
            }
            catch (const Answer& answer)
            {
                stop_signal = true;
                input_handler.detach();

                if (thread_exception)
                {
                    std::rethrow_exception(thread_exception);
                }

                return answer.content;
            }

            if (period > 0s)
            {
                // TODO: Replace OpCodes::DBG by a local implementation
                std::for_each(std::begin(cores), std::end(cores), [](auto& core)
                { core.execute(OpCodes::DBG()); });

                auto time_to_sleep = period - (BenchmarkClock::now() - time_before_execution);

                if (time_to_sleep > 0ns)
                {
                    std::this_thread::sleep_for(time_to_sleep);
                }
            }

            loops++;
        }
    }
    catch(...)
    {
        stop_signal = true;
        throw;
    }

    input_handler.join();

    if (thread_exception)
    {
        std::rethrow_exception(thread_exception);
    }

    return 0;
}


// TODO: Calibrate function
