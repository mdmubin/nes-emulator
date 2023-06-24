#pragma once

#include "common/types.hpp"
#include "nes/instructions.hpp"
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

    /// request interrupt (can be disabled by setting disable interrupt bit)
    void irq();

private:
    u8  A  = 0; // Accumulator Register, A
    u8  X  = 0; // Index Register, X
    u8  Y  = 0; // Index Register, Y
    u8  P  = 0; // Processor Status Register, P
    u8  SP = 0; // Stack Pointer, SP
    u16 PC = 0; // Program Counter, PC

    Bus *bus = nullptr; // the bus, interface for communication

    const Instruction *instruction = nullptr; // currently executing instruction

    u8 cyclesRemaining = 0; // remaining number of cycles to simulate

    u64 cyclesExecuted = 0; // total number of cycles simulated

private:
    /// Index of each status flag in the status register, P
    enum StatusFlag {
        C = 1 << 0, // Carry
        Z = 1 << 1, // Zero
        I = 1 << 2, // Disable Interrupts
        D = 1 << 3, // Decimal Mode (unused)
        B = 1 << 4, // Break
        U = 1 << 5, // Unused
        V = 1 << 6, // Overflow
        N = 1 << 7, // Negative
    };

    // status register modifiers

    void set_C_status(bool status);
    void set_Z_status(bool status);
    void set_I_status(bool status);
    void set_D_status(bool status);
    void set_B_status(bool status);
    void set_V_status(bool status);
    void set_N_status(bool status);

    /// set the zero and negative bits in the status register based on the value of num
    void set_ZN_status(u8 num);

private:
    /// fetch instruction from memory and decode it
    void decode_instruction();

    u16 get_operand_address(AddressingMode mode);

    void execute_instruction(u16 operandAddr);

    void branch_to(u16 address);

    void handle_page_break(u16 a1, u16 a2);
};

} // namespace nes
