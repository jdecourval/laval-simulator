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


bool Cpu::handle_output(std::ostream& output)
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

            output << static_cast<int>(value->second);
        }
    }

    if (!first)
    {
        output << std::endl;
    }

    return !first;
}

void Cpu::handle_input(std::istream& input, std::atomic<bool>& stop_signal, std::exception_ptr& thread_exception)
{
    std::vector<uint8_t> queue;
    queue.reserve(inputs.size());

    while(!stop_signal)
    {
        try
        {
            queue.clear();

            while (true)
            {
                if (auto delimiter = input.get(); delimiter == std::char_traits<char>::eof())
                {
                    stop_signal = true;
                    break;
                }
                else if (delimiter == '\n')
                {
                    break;
                }

                input.unget();

                auto value = 0;

                input >> value;

                cpu_assert(input, "Input error");
                cpu_assert(value <= 0xff, "Too large input");
                cpu_assert(value >= 0, "Only unsigned values are supported");

                queue.push_back(static_cast<uint8_t>(value));
            }

            cpu_assert(queue.size() == inputs.size()|| queue.empty(), "Wrong number of parameters: " << queue.size() << ". Expected " << inputs.size());

            auto i = std::cbegin(queue);
            std::lock_guard lock(input_lock);
            for (auto& core_input: inputs)
            {
                core_input.second.put(*(i++));
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
        std::cerr << "starting cpu at " << 1000 / period.count() << " Hz" << std::endl;
    }
    else
    {
        std::cerr << "starting cpu at max speed" << std::endl;
    }

    std::atomic<bool> stop_signal{};
    std::exception_ptr thread_exception{};
    auto input_handler = std::thread([this, &input, &stop_signal, &thread_exception]() { return handle_input(input, stop_signal, thread_exception); });

    auto time_before_execution = BenchmarkClock::now();
    auto last_report_time = BenchmarkClock::now();
    auto loops = 0ull;

    try
    {

        while (true)
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

            if ((handle_output(output) || thread_exception) && stop_signal)
            {
                break;
            }

            std::for_each(std::begin(cores), std::end(cores), [](auto& core)
            { core.preload(); });

            auto not_staled = false;
            try
            {
                // Could be a little cleaner (and maybe even replace the thread pool) with std::transform_reduce
                // which is not yet available in GCC
                std::for_each(std::begin(cores), std::end(cores), [&not_staled](auto& core)
                { not_staled |= core.fetch(); });
            }
            catch (const Answer& answer)
            {
                stop_signal = true;
                input_handler.detach();

                if (thread_exception)
                {
                    std::rethrow_exception(thread_exception);
                }

                std::cerr << "Cycles simulated: " << loops << std::endl;
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

            loops = not_staled ? loops + 1 : loops;
        }
    }
    catch(...)
    {
        std::cerr << "Cycles simulated: " << loops << std::endl;
        stop_signal = true;
        throw;
    }

    std::cerr << "Cycles simulated: " << loops << std::endl;
    input_handler.join();

    if (thread_exception)
    {
        std::rethrow_exception(thread_exception);
    }

    return 0;
}

std::ostream& operator<<(std::ostream& os, const Cpu& cpu)
{
    os << "Instruction space: " << static_cast<size_t>(cpu.cores[0].get_factory().size()) << "/" << 256 << "\n";
    os << "Cores number: " << cpu.cores.size() << "\n";
    os << "Memory banks number: " << static_cast<size_t>(cpu.mem.banks_number()) << "\n";
    os << "Memory banks size: " << static_cast<size_t>(cpu.mem.banks_size()) << "\n";
    os << "Inputs on cores: ";
    for (auto& input: cpu.inputs)
    {
        os << input.first << " ";
    }

    os << "\n";

    os << "Outputs on cores: ";
    std::copy(cpu.outputs.begin(), cpu.outputs.end(), std::ostream_iterator<size_t>(os, " "));

    return os;
}


// TODO: Calibrate function
