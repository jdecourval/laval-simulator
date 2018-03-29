#ifndef SIMULATOR_CORE_H
#define SIMULATOR_CORE_H

#include "fetchable.h"
#include "memory.h"
#include "registers.h"

#include "impl/instruction_factory.h"


class CoreArray;


class Core final : public Fetchable
{
public:
    Core();

    Core(CoreArray& cores, size_t id, const MemoryInterface& mem);

    void wire(uint8_t membank);

    void initialize();

    void preload(bool force = false);

    /// @return true if core have not blocked
    bool fetch();

    bool execute(const InstructionBase& raw_instruction);

    /// @return true if core have not blocked
    bool step();

    std::optional<std::pair<bool, uint8_t>> get_from(bool carry) override final
    {
        if (registers.status1.sync)
        {
            registers.status2.unlock = true;

            return {{ registers.status2.negative, carry ? registers.status2.carry : registers.val }};
        }
        else
        {
            return {};
        }
    }

    const InstructionFactory& get_factory() const;

    bool operator==(const Core&) const;

    Core(Core&& core) = default;

    Core(Core&) = delete;

    Core& operator=(Core&) = delete;

private:
    Registers registers;
    InstructionFactory factory;

    const MemoryInterface* mem;
    CoreArray* cores;  // TODO: make const

    friend std::ostream& operator<<(std::ostream& os, const Core& core);
};


#endif //SIMULATOR_CORE_H
