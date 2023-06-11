#include "nes/cpu/cpu.hpp"

int main() {
    nes::Cpu cpu;
    cpu.load_and_run({0xa9, 0xc2, 0xaa, 0xe8, 0x00});
}
