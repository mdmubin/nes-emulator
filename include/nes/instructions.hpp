#pragma once

namespace nes {

/// Valid opcode values that are interpreted by the CPU
enum Opcode {
    /// Break / Interrupt
    BRK = 0x00,
    /// Load to register A
    LDA = 0xA9,
    /// Transfer A to X
    TAX = 0xAA,
};

} // namespace nes
