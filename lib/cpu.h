#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H

#include <memory>
#include <chrono>

#include "memory.h"
#include "impl/core_array.h"


class CoreArray;

/// Class responsible of initializing and running the system
class Cpu
{
    using BenchmarkClock = std::chrono::high_resolution_clock;

public:
    explicit Cpu(const Settings&);
    explicit Cpu(const Settings&, Memory&& memory, std::vector<MemoryInterface::size_type>&& core_to_mem_map, std::vector<std::vector<std::pair<int, int>>>&& parameters = {});

    void link_memory(Memory&& memory, std::vector<MemoryInterface::size_type>&& core_to_mem_map);

    template <typename Instruction_t>
    uint8_t dump(const Instruction_t& instruction)
    {
        assert(cores.size() > 0);

        return cores[0].get_factory().dump(instruction);
    }

    uint8_t start(const std::chrono::milliseconds& period = std::chrono::milliseconds(0), const std::vector<uint8_t>& args = {});

    Cpu(Cpu&) = delete;

    Cpu& operator=(Cpu&) = delete;

private:
    Memory mem;
    CoreArray cores;
    std::vector<MemoryInterface::size_type> core_to_mem_map;
    std::vector<std::vector<std::pair<int, int>>> parameters;
};


#endif //SIMULATOR_CPU_H
