#include "nes/bus.hpp"

int main() {
    nes::Bus bus;
    bus.cpu.load_and_run({0xa9, 0xc2, 0xaa, 0xe8, 0x00});
}
