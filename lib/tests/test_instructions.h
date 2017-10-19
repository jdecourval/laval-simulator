#ifndef PROJECT_TEST_INSTRUCTIONS_H
#define PROJECT_TEST_INSTRUCTIONS_H


#include "instruction.h"

#include "registers.h"


class Debug: public InstructionBase
{
public:
    explicit Debug(Registers& output);

    bool operator()(Registers& registers) const override;

private:
    Registers& output;
};


#endif //PROJECT_TEST_INSTRUCTIONS_H
