#pragma once

#include "common/types.hpp"
#include "common/utils.hpp"
#include "nes/memory.hpp"

#include <vector>

namespace nes {

// Forward declaration of bus class
class Bus;

/// The main Cpu abstraction for emulating the NES
class Cpu {
public:
    /// Create a new Cpu, with all values set to default values
    Cpu() = default;

    /// Enable bus communication with this CPU
    void connect_bus(Bus *bus);

    /// reset the current CPU to a known state
    void reset();

private:
    u8  A  = 0; // Accumulator Register, A
    u8  X  = 0; // Index Register, X
    u8  Y  = 0; // Index Register, Y
    u8  P  = 0; // Processor Status Register, P
    u8  SP = 0; // Stack Pointer, SP
    u16 PC = 0; // Program Counter, PC

    Bus *bus = nullptr; // The bus

    u8 cyclesRemaining = 0;

private:
    /// Index of each status flag in the status register, P
    enum StatusFlag {
        C = 0, // Carry
        Z = 1, // Zero
        I = 2, // Disable Interrupts
        D = 3, // Decimal Mode (unused)
        B = 4, // Break
        U = 5, // Unused
        V = 6, // Overflow
        N = 7, // Negative
    };

    /// Modify the status register. Set a flag in the status register to the given value
    void set_status(StatusFlag flag, bool status);

    /// Get the value of a status flag, in the status register, P
    bool get_status(StatusFlag status) const;

private:
    /// copy the program values into the memory
    /// TODO: remove this bit, we don't want to load programs directly onto our CPU
    void load_program(const std::vector<u8> &program);

    /// emulation loop
    void run();
};

} // namespace nes
