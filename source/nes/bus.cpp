#include "nes/bus.hpp"

using namespace nes;
using namespace std;

u8 Bus::read_u8(u16 address) {
    u16 mirroredAddr;
    if (address >= 0x0000 && address <= 0x1FFF) {
        mirroredAddr = address & 0x07FF;
        return mem->read_u8(mirroredAddr);
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        mirroredAddr = address & 0x2007;
        // PPU registers, not implemented yet
    } else if (address >= 0x4000 && address <=  0x401F) {
        // APU and I/O functionality
    } else if (address >= 4020 && address <= 0xFFFF) {
        // Rom space(PRG ROM, PRG RAM), not implemented yet
    }
    return 0;
}

u16 Bus::read_u16(u16 address) {
    u16 lo = read_u8(address);
    u16 hi = read_u8(address + 1);
    return (hi << 8) | lo;
}

void Bus::write_u8(u8 data, u16 address) {
    u16 mirroredAddr;
    if (address >= 0x0000 && address <= 0x1FFF) {
        mirroredAddr = address & 0x07FF;
        mem->write_u8(data, mirroredAddr);
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        // PPU registers, not implemented yet
    } else if (address >= 0x4000 && address <= 0x401F) {
        // APU and I/O functionality
    } else if (address >= 4020 && address <= 0xFFFF) {
        // Rom space(PRG ROM, PRG RAM), not implemented yet
    }
}

void Bus::write_u16(u16 data, u16 address) {
    u8 lo = (data & 0xFF);
    u8 hi = (data >> 8) & 0xFF;
    write_u8(lo, address);
    write_u8(hi, address + 1);
}

//

void Bus::attach_components(Cpu *c, Mem *m) {
    mem = m;
    c->connect_bus(this);
    m->reset();
}

void Bus::attach_rom(Rom *r) {
    rom = r;
}
