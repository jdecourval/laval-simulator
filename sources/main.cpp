#include "cpu.h"
#include "settings.h"


int main()
{
    Cpu cpu(settings);
    cpu.Start();
}
