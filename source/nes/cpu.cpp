#include "nes/cpu.hpp"
#include "nes/bus.hpp"

#ifndef NDEBUG
  #include <iostream>
  #include <string>
#endif

using namespace nes;
using namespace std;

void Cpu::reset() {
    A  = 0;
    X  = 0;
    Y  = 0;
    P  = 0;
    SP = 0xFD;
    PC = bus->read_u16(0xFFFC);
}

void Cpu::connect_bus(Bus *b) {
    bus = b;
}

//

bool Cpu::get_status(Cpu::StatusFlag status) const {
    return get_bit_value(P, status);
}

void Cpu::set_status(Cpu::StatusFlag flag, bool status) {
    set_bit_value(P, flag, status);
}

//

void Cpu::execute() {
    auto opcode = bus->read_u8(PC++);

    currentInstruction = &INSTRUCTION_LOOKUP[opcode];

    auto address = get_address(currentInstruction->mode);
}


u16 Cpu::get_address(AddressingMode mode) {
    u16 address;

    // helper function to check if two address has a page break in between them
    auto handle_page_break = [&](u16 a1, u16 a2) {
        // if a page break found between the two addresses
        if ((a1 & 0xFF00) != (a2 & 0xFF00)) {
            switch (currentInstruction->op) {
            case op_STA: case op_ASL: case op_DEC: case op_INC: case op_LSR: case op_ROL: case op_ROR:
                break; // these instructions don't take page breaks into account
            default:
                cyclesRemaining++;
            }
        }
    };

    switch (mode) {
    case am_ABS: {
        address = bus->read_u16(PC);
        PC += 2; // 2 bytes read
        break;
    }
    case am_ABX: {
        address = bus->read_u16(PC);

        handle_page_break(address, address + X);

        address += X;
        PC += 2;
        break;
    }
    case am_ABY: {
        address = bus->read_u16(PC);

        handle_page_break(address, address + Y);

        address += Y;
        PC += 2;
        break;
    }
    case am_IMM: {
        address = PC;
        PC += 1;
        break;
    }
    case am_IMP: {
        address = 0; // no address value expected, we return default of zero
        break;
    }
    case am_IND: {
        u16 pointer = bus->read_u16(PC);

        // simulate the 6502 hardware bug where if the lower pointer byte is $FF (at page boundary),
        // lo byte is fetched from $XX as expected, but hi byte is fetched from $XX00
        u8 lo = bus->read_u8(pointer);
        u8 hi = bus->read_u8((pointer & 0xFF00) | ((pointer + 1) & 0x00FF));

        address = (hi << 8) | lo;
        PC += 2;
        break;
    }
    case am_INX: {
        u16 pointer = bus->read_u8(PC);

        u8 lo = bus->read_u8((pointer + X) & 0xFF);
        u8 hi = bus->read_u8((pointer + X + 1) & 0xFF);

        address = (hi << 8) | lo;
        PC += 1;
        break;
    }
    case am_INY: {
        u16 pointer = bus->read_u8(PC);

        u8 lo = bus->read_u8(pointer & 0xFF);
        u8 hi = bus->read_u8((pointer + 1) & 0xFF);

        address = (hi << 8) | lo;

        handle_page_break(address, address + Y);

        address += Y;
        PC += 2;
        break;
    }
    case am_REL: {
        auto offset = (int8_t) bus->read_u8(PC); // signed offset for branching
        address     = PC + offset;
        PC += 1;
        break;
    }
    case am_ZP0: {
        address = bus->read_u8(PC);
        address &= 0x00FF;
        PC += 1;
        break;
    }
    case am_ZPX: {
        address = bus->read_u8(PC) + X;
        address &= 0x00FF;
        PC += 1;
        break;
    }
    case am_ZPY: {
        address = bus->read_u8(PC) + Y;
        address &= 0x00FF;
        PC += 1;
        break;
    }
    }

    return address;
}
