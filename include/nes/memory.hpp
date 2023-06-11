#pragma once

#include <array>
#include <vector>

#include "common/types.hpp"

namespace nes {

class Memory {
public:
    Memory() = default;

    /// clear the memory
    void reset();

    /// read one byte from memory at the given address
    u8 read_u8(u16 address);

    /// read two bytes from memory at the given address
    u16 read_u16(u16 address);

    /// write one byte to memory at the given address
    void write_u8(u8 data, u16 address);

    /// write two bytes to memory at the given address
    void write_u16(u16 data, u16 address);

    void load_program(const std::vector<u8>& program);

private:
    std::array<u8, 0xFFFF> memory = {};
};

} // namespace nes