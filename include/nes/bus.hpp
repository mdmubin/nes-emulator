#pragma once

#include "common/types.hpp"
#include "nes/cpu.hpp"
#include "nes/memory.hpp"

#include <array>

namespace nes {


class Bus {
public:
    Bus();

    u8 read_u8(u16 address);

    u16 read_u16(u16 address);

    void write_u8(u8 data, u16 address);

    void write_u16(u16 data, u16 address);

//private:
    Memory mem;

    Cpu cpu;
};

} // namespace nes
