#ifndef PROJECT_TEST_INSTRUCTIONS_H
#define PROJECT_TEST_INSTRUCTIONS_H


#include "instruction.h"

#include "registers.h"


class Debug: public InstructionBase
{
public:
    explicit Debug(Registers& output);

    bool operator()(Registers& registers) const override;

    uint8_t dump_args() const override;

    void load_args(const std::vector<uint8_t>& args) override;

private:
    Registers& output;
};


#endif //PROJECT_TEST_INSTRUCTIONS_H
