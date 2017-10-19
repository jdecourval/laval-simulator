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
