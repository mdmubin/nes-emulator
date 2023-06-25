#pragma once

#include <array>
#include <vector>

#include "common/types.hpp"

namespace nes {

class Mem {
public:
    Mem() = default;

    /// clear the mem
    void reset();

    /// read one byte from mem at the given address
    u8 read_u8(u16 address);

    /// read two bytes from mem at the given address
    u16 read_u16(u16 address);

    /// write one byte to mem at the given address
    void write_u8(u8 data, u16 address);

    /// write two bytes to mem at the given address
    void write_u16(u16 data, u16 address);

    void load_program(const std::vector<u8>& program);

private:
    std::array<u8, 0xFFFF> memory = {};
};

} // namespace nes