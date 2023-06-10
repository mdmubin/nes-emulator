#include "nes/cpu/cpu.hpp"

int main() {
    nes::Cpu cpu;
    cpu.interpret({0xa9, 0xC2, 0xaa, 0xe8, 0x00});
}
