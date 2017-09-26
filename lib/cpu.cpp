#include "cpu.h"

#include <thread>
#include <iostream>

using namespace std::chrono_literals;

Cpu::Cpu()
        : cores(Settings::DIMENSIONS)
{
//    cores.insert(cores.end(), {typeid(Types)...});
    running = false;
    linkCores();


//    cores.at(0).at(1).at(1).mem.at(0) = Instruction{Op::CAD, 2};
//    cores.at(0).at(1).at(1).mem.at(1) = Instruction{Op::CAD, 2};
//
//    cores.at(0).at(0).at(0).mem.at(0) = Instruction{Op::MXH, 1};
//    cores.at(0).at(0).at(0).mem.at(1) = Instruction{Op::LCL, 5};
//    cores.at(0).at(0).at(0).mem.at(2) = Instruction{Op::MAD, 0};
//    cores.at(0).at(0).at(0).mem.at(3) = Instruction{Op::DBG, 0};

//    cores.data[1].mem[0] = Instruction{Op::LCL, 0xf};
//    cores.data[1].mem[1] = Instruction{Op::LCH, 0xd};
//    auto dir = Direction(1, 1, 1).compress();
//    cores[1].mem[2] = Instruction{Op::MUX, dir};
}

void Cpu::linkCores()
{
//    size_t count = 0;
//    auto cores_ptr = &cores;
//    auto mem_ptr = &mem;
//    cores.apply([cores_ptr, mem_ptr, &count](auto& core){core.link(cores_ptr, count++, mem_ptr);});
}

void Cpu::Start()
{
    running = true;

    std::cout << "starting" << std::endl;

    while (running)
    {
        std::cout << "loop" << std::endl;

        for(auto i=0; i<Settings::CORES; i++)
        {
            cores[i].step1();
        }

        for(auto i=0; i<Settings::CORES; i++)
        {
            cores[i].step2();
        }

        std::this_thread::sleep_for(1s);
    }
}
