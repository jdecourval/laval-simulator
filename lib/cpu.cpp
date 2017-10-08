#include "cpu.h"

#include <thread>
#include <iostream>


using namespace std::chrono_literals;

Cpu::Cpu()
        : cores(ComputedSettings::DIMENSIONS)
{
    running = false;
    linkCores();
}

void Cpu::linkCores()
{

}

void Cpu::Start()
{
    running = true;

    std::cout << "starting" << std::endl;

    while (running)
    {
        std::cout << "loop" << std::endl;

        for (auto i = 0; i < ComputedSettings::CORES; i++)
        {
            cores[i].preload();
        }

        for (auto i = 0; i < ComputedSettings::CORES; i++)
        {
            cores[i].fetch();
        }

        std::this_thread::sleep_for(1s);
    }
}
