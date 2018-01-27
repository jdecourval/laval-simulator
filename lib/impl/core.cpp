#include "core.h"

#include "core_array.h"
#include "opcodes.h"


Core::Core()
        : registers()
          , mem()
          , cores(nullptr)
{
    // This simulator rely on the fact that the simulating CPU uses two's complement notation
    static_assert(-1 == ~0, "Your CPU architecture is too weird to use this simulator");

    initialize();
}

Core::Core(CoreArray& cores, size_t id, const MemoryInterface& mem)
    : registers()
      , mem(&mem)
      , cores(&cores)
{
    // This simulator rely on the fact that the simulating CPU uses two's complement notation
    static_assert(-1 == ~0, "Your CPU architecture is too weird to use this simulator");

    registers.id = id;

    initialize();
}

void Core::wire(uint8_t membank)
{
    registers.status2.membank = membank;
}

void Core::initialize()
{
    cpu_assert(mem == nullptr || mem->banks_size() <= std::numeric_limits<decltype(registers.pc)>::max() + 1, "Too many memory banks");

    factory.register_instruction<OpCodes::NOP>();
    factory.register_instruction<OpCodes::SYN>();
    factory.register_instruction<OpCodes::CTC>();
    factory.register_instruction<OpCodes::CTV>();
    factory.register_instruction<OpCodes::DBG>();
    factory.register_instruction<OpCodes::HCF>();
    factory.register_instruction<OpCodes::HLT>();
    factory.register_instruction<OpCodes::MXD>();
    factory.register_instruction<OpCodes::MXL>();
    factory.register_instruction<OpCodes::MXA>();
    factory.register_instruction<OpCodes::MXS>();
    factory.register_instruction<OpCodes::MUX>();
    factory.register_instruction<OpCodes::LCL>();
    factory.register_instruction<OpCodes::LCH>();
    factory.register_instruction<OpCodes::JLZ>();
    factory.register_instruction<OpCodes::JEZ>();
    factory.register_instruction<OpCodes::JGZ>();
    factory.register_instruction<OpCodes::JMP>();
    factory.register_instruction<OpCodes::LLS>();
    factory.register_instruction<OpCodes::RLS>();
    factory.register_instruction<OpCodes::CAD>();
    factory.register_instruction<OpCodes::CSU>();
    factory.register_instruction<OpCodes::CAN>();
    factory.register_instruction<OpCodes::COR>();

    std::cout << "Instructions space: " << static_cast<int>(factory.size()) << " / 256" << std::endl;
}

void Core::preload()
{
    auto direction_complex = Direction::decode(registers.status1.mux);

    try
    {
        cpu_assert(cores, "CPU have no cores");

        auto direction = std::get<Direction::CoreDirection>(direction_complex);

        auto& pointed_core = cores->offset(registers.id, direction);

        if (pointed_core.registers.status1.sync)
        {
            registers.preload = registers.status1.ctc ? pointed_core.registers.status2.carry
                                                      : pointed_core.registers.val;
            registers.preload_negative = pointed_core.registers.status2.negative;

            // TODO: This is duplicated
            auto raw_instruction = mem->at(registers.status2.membank).at(registers.pc);
            auto instruction = factory.create(raw_instruction);

            // TODO: Should be in the instructions
            if (dynamic_cast<OpCodes::MXL*>(instruction.get()) || dynamic_cast<OpCodes::MXA*>(instruction.get()) || dynamic_cast<OpCodes::MXS*>(instruction.get()) ||
                dynamic_cast<OpCodes::MXD*>(instruction.get()))
            {
                pointed_core.registers.status2.unlock = true;
            }
        }
        else
        {
            registers.preload = {};
        }
    }
    catch (const std::bad_variant_access&)
    {
        auto direction = std::get<Direction::SpecialDirection>(direction_complex);

        switch (direction)
        {
            case Direction::SpecialDirection::PC:
                registers.preload = registers.pc;
                break;

            case Direction::SpecialDirection::MEMBANK:
                registers.preload = registers.status2.membank;
                break;

            default:
                throw std::logic_error("Invalid direction");
        }
    }
    catch (CpuException& exception)
    {
        exception.add_registers(registers);
        throw;
    }
}

void Core::fetch()
{
    try
    {
#ifndef NDEBUG
        if (!mem)
        {
            throw std::logic_error{"Memory is not linked"};
        }
#endif

        registers.status1.sync = false;

        auto raw_instruction = mem->at(registers.status2.membank).at(registers.pc);

        auto instruction = factory.create(raw_instruction);

        // Increment PC only if the instruction does not needs to stall the pipeline
        if ((*instruction)(registers))
        {
            // Safe to cast since the modulo limits the value and a check about that is done in initialize.
            registers.pc = static_cast<uint8_t>((registers.pc + 1) % mem->banks_size());
        }

        registers.status2.unlock = false;
    }
    catch (CpuException& exception)
    {
        exception.add_registers(registers);
        throw;
    }
}

bool Core::execute(const InstructionBase& raw_instruction)
{
    return raw_instruction(registers);
}

bool Core::operator==(const Core& core) const
{
    return registers.id == core.registers.id;
}

std::ostream& operator<<(std::ostream& os, const Core& core)
{
    os << core.registers.id;
    return os;
}

InstructionFactory& Core::get_factory()
{
    return factory;
}

void Core::step()
{
    preload();
    fetch();
}
