#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H

#include "memory.h"
#include "impl/core_array.h"

#include "throw_assert.h"

#include <chrono>
#include <map>
#include <memory>


class CoreArray;

/// Class responsible of initializing and running the system
class Cpu
{
    using BenchmarkClock = std::chrono::high_resolution_clock;

public:
    explicit Cpu(const Settings&);
    explicit Cpu(const Settings&, Memory&& memory);

    void link_memory(Memory&& memory, const Settings& settings);

    template <typename Instruction_t>
    uint8_t dump(const Instruction_t& instruction)
    {
        cpu_assert(cores.size() > 0, "CPU have no core");

        return cores[0].get_factory().dump(instruction);
    }

    uint8_t start(std::istream& input, std::ostream& output, const std::chrono::milliseconds& period = std::chrono::milliseconds(0));

    Cpu(Cpu&) = delete;

    Cpu& operator=(Cpu&) = delete;

    friend std::ostream& operator<<(std::ostream& os, const Cpu& cpu);

private:
    bool handle_output(std::ostream&);
    void handle_input(std::istream&, std::atomic<bool>& stop_signal, std::exception_ptr& thread_exception);


    Memory mem;
    CoreArray cores;
    std::vector<MemoryInterface::size_type> core_to_mem;

    /// Map core_id to their input. The map is willingly ordered by the argument position.
    std::map<size_t, Input> inputs;
    std::vector<size_t> outputs;

    std::mutex input_lock;
};


#endif //SIMULATOR_CPU_H
