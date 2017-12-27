#include "test_instructions.h"


Debug::Debug(Registers& output)
    : output(output)
{

}

bool Debug::operator()(Registers& registers) const
{
    output = registers;

    return true;
}

uint8_t Debug::dump_args() const
{
    return 0;
}

void Debug::load_args(const std::vector<uint8_t>&)
{

}
