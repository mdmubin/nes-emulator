#include "nes/memory.hpp"

using namespace nes;
// using namespace std;

void Mem::reset() {
    std::fill(memory.begin(), memory.end(), 0);
}

u8 Mem::read_u8(u16 address) {
    return memory[address];
}

u16 Mem::read_u16(u16 address) {
    u16 lo = memory[address];
    u16 hi = memory[address + 1];
    return (hi << 8) | lo;
}

void Mem::write_u8(u8 data, u16 address) {
    memory[address] = data;
}

void Mem::write_u16(u16 data, u16 address) {
    u8 lo = (u8) (data & 0xFF);
    u8 hi = (u8) (data >> 8);

    memory[address]     = lo;
    memory[address + 1] = hi;
}
