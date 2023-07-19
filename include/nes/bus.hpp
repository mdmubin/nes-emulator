#pragma once

#include "common/types.hpp"
#include "nes/cpu.hpp"
#include "nes/rom.hpp"
#include "nes/memory.hpp"

#include <array>

namespace nes {

class Bus {
public:
    Bus() = default;

    u8 read_u8(u16 address);

    u16 read_u16(u16 address);

    void write_u8(u8 data, u16 address);

    void write_u16(u16 data, u16 address);

    void attach_components(Cpu *cpu, Mem *m);

    void attach_rom(Rom* rom);

private:
    Mem *mem = nullptr;
    Rom *rom = nullptr;
};

} // namespace nes
