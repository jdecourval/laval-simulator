#include "cpu.h"

#include <thread>
#include <iostream>


using namespace std::chrono_literals;

Cpu::Cpu(const Settings& settings)
    : mem{settings.bank_number, settings.bank_size}
      , cores(settings.dimensions, mem)
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


        std::this_thread::sleep_for(1s);
    }
}
