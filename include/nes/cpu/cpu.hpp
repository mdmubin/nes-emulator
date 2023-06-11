#pragma once

#include "common/types.hpp"
#include "common/utils.hpp"
#include "nes/memory.hpp"

#include <vector>

namespace nes {

/// The main Cpu abstraction for emulating the NES
class Cpu {
public:
    /// Create a new Cpu, with all values set to default values
    Cpu();

    /// Interpret the program / memory slice, and run the emulator
    void load_and_run(const std::vector<u8> &program);

    void reset();

private:
    /// Accumulator Register, A
    u8 A;

    /// Index Register, X
    u8 X;

    /// Index Register, Y
    u8 Y;

    /// Processor Status Register, P
    u8 P;

    /// Stack Pointer, SP
    u8 SP;

    /// Program Counter, PC
    u16 PC;

    /// Memory
    Memory memory;


private:
    /// Index of each status flag in the status register, P
    enum Status {
        C = 0, // Carry
        Z = 1, // Zero
        I = 2, // Disable Interrupts
        D = 3, // Decimal Mode (unused)
        B = 4, // Break
        _ = 5, // Unused
        V = 6, // Overflow
        N = 7, // Negative
    };


    /// Modify the status register. Set the carry flag to the value of status
    void set_carry(bool status);

    /// Modify the status register. Set the zero flag to the value of status
    void set_zero(bool status);

    /// Modify the status register. Set the interrupt disable flag to the value of status
    void set_interrupt_disabled(bool status);

    /// Modify the status register. Set the decimal mode operations flag to the value of status
    /// NOTE: probably will remain unused in our implementation
    void set_decimal_mode(bool status);

    /// Modify the status register. Set the break flag to the value of status
    void set_break(bool status);

    /// Modify the status register. Set the overflow flag to the value of status
    void set_overflow(bool status);

    /// Modify the status register. Set the negative flag to the value of status
    void set_negative(bool status);

    /// Get the value of a status flag, in the status register, P
    bool get_status(Status status) const;


private:
    /// copy the program values into the memory
    void load_program(const std::vector<u8> &program);

    /// emulation loop
    void run();
};

} // namespace nes
