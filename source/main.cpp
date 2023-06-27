#include "nes/system.hpp"

int main() {
    nes::System system;
    system.load_program({
        0xA2, 0x0A,       // LDX #10
        0x8E, 0x00, 0x00, // STX $0000
        0xA2, 0x03,       // LDX #03
        0x8E, 0x01, 0x00, // STX $0001
        0xAC, 0x00, 0x00, // LDY $0000
        0xA9, 0x00,       // LDA #0
        0x18,             // CLC
        0x6D, 0x01, 0x00, // ADC $0001
        0x88,             // DEY
        0xD0, 0xFA,       // BNE $fa
        0x8D, 0x02, 0x00, // STA $0002
        0xEA,             // NOP
        0xEA,             // NOP
        0xEA,             // NOP
    });
    system.run();
}
