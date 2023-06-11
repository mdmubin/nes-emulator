#include "nes/memory.hpp"

using namespace nes;
// using namespace std;

void Memory::reset() {
    std::fill(memory.begin(), memory.end(), 0);
}

u8 Memory::read_u8(u16 address) {
    return memory[address];
}

u16 Memory::read_u16(u16 address) {
    u16 lo = memory[address];
    u16 hi = memory[address + 1];
    return (hi << 8) | lo;
}

void Memory::write_u8(u8 data, u16 address) {
    memory[address] = data;
}

void Memory::write_u16(u16 data, u16 address) {
    u8 lo = (u8) (data & 0xFF);
    u8 hi = (u8) (data >> 8);

    memory[address]     = lo;
    memory[address + 1] = hi;
}

void Memory::load_program(const std::vector<u8> &program) {
    std::copy(program.begin(), program.end(), memory.begin() + 0x8000);
}
