#include "cpu.h"
#include "settings.h"

using namespace std::chrono_literals;

int main()
{
    Cpu cpu(settings);
    cpu.start(0s);
}
