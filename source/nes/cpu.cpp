#include "nes/cpu.hpp"
#include "nes/bus.hpp"

#ifndef NDEBUG
  #include <iostream>
  #include <string>
#endif

using namespace nes;

//

#define STACK_START 0x0100

//

void Cpu::connect_bus(Bus *b) {
    bus = b;
}

void Cpu::reset() {
    A  = 0;
    X  = 0;
    Y  = 0;
    P  = 0x34;
    SP = 0xFD;
    PC = bus->read_u16(0xFFFC);
}

void Cpu::irq() {
    if ((P & I) == 0) {
        bus->write_u8(PC >> 8, STACK_START + SP);
        SP -= 1;
        bus->write_u8(PC & 0xFF, STACK_START + SP);
        SP -= 1;
    }
}

//

void Cpu::decode_instruction() {
    auto opcode = bus->read_u8(PC);
    PC += 1;

    instruction = &INSTRUCTION_LOOKUP[opcode];
    auto operandAddr = get_operand_address(instruction->mode);

    execute_instruction(operandAddr);
}

void Cpu::execute_instruction(u16 addr) {
    switch (instruction->op) {
    case op_ADC: {
        u8 M = bus->read_u8(addr);

        u16 sum = A + M + ((P & C) ? 1 : 0);
        set_C_status((sum & 0xFF00) != 0);
        set_ZN_status(sum & 0xFF);
        // overflow bit set, this thing is evil and I cba to explain it :P
        set_V_status(((A ^ sum) & ~(A ^ M) & 0x80) != 0);

        A = sum & 0xFF;
        break;
    }
    case op_AND: {
        A &= bus->read_u8(addr);
        set_ZN_status(A);
        break;
    }
    case op_ASL: {
        u16 M = instruction->mode == am_IMP ? A : bus->read_u8(addr);

        M <<= 1;
        set_C_status((M & 0xFF00) != 0);
        set_ZN_status(M & 0x00FF);

        if (instruction->mode == am_IMP) {
            A = M & 0xFF;
        } else {
            bus->write_u8(M & 0xFF, addr);
        }

        break;
    }
    case op_BCC: {
        if ((P & C) == 0) {
            branch_to(PC + addr);
        }
        break;
    }
    case op_BCS: {
        if ((P & C) != 0) {
            branch_to(PC + addr);
        }
        break;
    }
    case op_BEQ: {
        if ((P & Z) != 0) {
            branch_to(PC + addr);
        }
        break;
    }
    case op_BIT: {
        u8 M = bus->read_u8(addr);

        u8 tmp = A & M;

        set_Z_status(tmp == 0);
        set_N_status((M & 0x80) != 0);
        set_V_status((M & 0x40) != 0);

        break;
    }
    case op_BMI: {
        if ((P & N) != 0) {
            branch_to(PC + addr);
        }
        break;
    }
    case op_BNE: {
        if ((P & Z) == 0) {
            branch_to(PC + addr);
        }
        break;
    }
    case op_BPL: {
        if ((P & N) == 0) {
            branch_to(PC + addr);
        }
        break;
    }
    case op_BRK: {
        set_I_status(true);
        set_B_status(true);

        // push hi byte first, then lo byte
        bus->write_u8(PC >> 8, STACK_START + SP);
        SP -= 1;
        bus->write_u8(PC & 0xFF, STACK_START + SP);
        SP -= 1;

        bus->write_u8(P | B, STACK_START + SP);
        SP -= 1;

        PC = bus->read_u16(0xFFFE); // load interrupt vector

        set_B_status(false);
        break;
    }
    case op_BVC: {
        if ((P & V) == 0) {
            branch_to(PC + addr);
        }
        break;
    }
    case op_BVS: {
        if ((P & V) != 0) {
            branch_to(PC + addr);
        }
        break;
    }
    case op_CLC: {
        set_C_status(false);
        break;
    }
    case op_CLD: {
        set_D_status(false);
        break;
    }
    case op_CLI: {
        set_I_status(false);
        break;
    }
    case op_CLV: {
        set_V_status(false);
        break;
    }
    case op_CMP: {
        u8 M = bus->read_u8(addr);

        set_C_status(A >= M);
        set_Z_status(A == M);
        set_N_status((A - M) & 0x80);
        break;
    }
    case op_CPX: {
        u8 M = bus->read_u8(addr);

        set_C_status(X >= M);
        set_Z_status(X == M);
        set_N_status((X - M) & 0x80);
        break;
    }
    case op_CPY: {
        u8 M = bus->read_u8(addr);

        set_C_status(Y >= M);
        set_Z_status(Y == M);
        set_N_status((Y - M) & 0x80);
        break;
    }
    case op_DEC: {
        u8 M = bus->read_u8(addr) - 1;
        set_ZN_status(M);
        bus->write_u8(M, addr);
        break;
    }
    case op_DEX: {
        X -= 1;
        set_ZN_status(X);
        break;
    }
    case op_DEY: {
        Y -= 1;
        set_ZN_status(Y);
        break;
    }
    case op_EOR: {
        A ^= bus->read_u8(addr);
        set_ZN_status(A);
        break;
    }
    case op_INC: {
        u8 M = bus->read_u8(addr) + 1;
        set_ZN_status(M);
        bus->write_u8(M, addr);
        break;
    }
    case op_INX: {
        X += 1;
        set_ZN_status(X);
        break;
    }
    case op_INY: {
        Y += 1;
        set_ZN_status(Y);
        break;
    }
    case op_JMP: {
        PC = addr;
        break;
    }
    case op_JSR: {
        PC -= 1;

        bus->write_u8(PC >> 8, STACK_START + SP);
        SP -= 1;
        bus->write_u8(PC & 0xFF, STACK_START + SP);
        SP -= 1;

        PC = addr;
        break;
    }
    case op_LDA: {
        A = bus->read_u8(addr);
        set_ZN_status(A);
        break;
    }
    case op_LDX: {
        X = bus->read_u8(addr);
        set_ZN_status(X);
        break;
    }
    case op_LDY: {
        Y = bus->read_u8(addr);
        set_ZN_status(Y);
        break;
    }
    case op_LSR: {
        u8 M = instruction->mode == am_IMP ? A : bus->read_u8(addr);

        set_C_status((M & 0x01) != 0);
        M >>= 1;
        set_ZN_status(M);

        if (instruction->mode == am_IMP) {
            A = M;
        } else {
            bus->write_u8(M, addr);
        }
        break;
    }
    case op_NOP: {
        // nothing changed
        break;
    }
    case op_ORA: {
        A |= bus->read_u8(addr);
        set_ZN_status(A);
        break;
    }
    case op_PHA: {
        bus->write_u8(A, STACK_START + SP);
        SP -= 1;
        break;
    }
    case op_PHP: {
        set_B_status(true);
        bus->write_u8(P, STACK_START + SP);
        SP -= 1;
        set_B_status(false);
        break;
    }
    case op_PLA: {
        SP += 1;
        A = bus->read_u8(STACK_START + SP);
        set_ZN_status(A);
        break;
    }
    case op_PLP: {
        SP += 1;
        P = bus->read_u8(STACK_START + SP);
        break;
    }
    case op_ROL: {
        u8 M = instruction->mode == am_IMP ? A : bus->read_u8(addr);

        u8 rotated = M << 1;
        rotated |= ((P & C) != 0) ? 1 : 0;

        set_C_status((M & 0x80) != 0);
        set_ZN_status(rotated);

        if (instruction->mode == am_IMP) {
            A = rotated;
        } else {
            bus->write_u8(rotated, addr);
        }
        break;
    }
    case op_ROR: {
        u8 M = instruction->mode == am_IMP ? A : bus->read_u8(addr);

        u8 rotated = M >> 1;
        rotated |= ((P & C) != 0) ? 0x80 : 0;

        set_C_status((M & 1) != 0);
        set_ZN_status(rotated);

        if (instruction->mode == am_IMP) {
            A = rotated;
        } else {
            bus->write_u8(rotated, addr);
        }
        break;
    }
    case op_RTI: {
        // pull P-register first, then PC
        SP += 1;
        P  = bus->read_u8(STACK_START + SP);
        PC = bus->read_u16(STACK_START + SP + 1);
        SP += 2;
        break;
    }
    case op_RTS: {
        PC = bus->read_u16(STACK_START + SP + 1);
        SP += 2;
        PC += 1;
        break;
    }
    case op_SBC: {
        u16 M = bus->read_u8(addr) ^ 0xFF;

        u16 sum = A + M + ((P & C) ? 1 : 0);
        set_C_status((sum & 0xFF00) != 0);
        set_ZN_status(sum & 0xFF);

        // overflow bit set, this part is evil and I cba to explain it :P
        set_V_status(((A ^ sum) & ~(A ^ M) & 0x80) != 0);

        A = sum & 0xFF;
        break;
    }
    case op_SEC: {
        set_C_status(true);
        break;
    }
    case op_SED: {
        set_D_status(true);
        break;
    }
    case op_SEI: {
        set_I_status(true);
        break;
    }
    case op_STA: {
        bus->write_u8(A, addr);
        break;
    }
    case op_STX: {
        bus->write_u8(X, addr);
        break;
    }
    case op_STY: {
        bus->write_u8(Y, addr);
        break;
    }
    case op_TAX: {
        X = A;
        set_ZN_status(X);
        break;
    }
    case op_TAY: {
        Y = A;
        set_ZN_status(Y);
        break;
    }
    case op_TSX: {
        X = SP;
        set_ZN_status(X);
        break;
    }
    case op_TXA: {
        A = X;
        set_ZN_status(A);
        break;
    }
    case op_TXS: {
        SP = X;
        set_ZN_status(SP);
        break;
    }
    case op_TYA: {
        A = Y;
        set_ZN_status(A);
        break;
    }
    case op_XXX: {
        // unknown instruction, do nothing
        break;
    }
    }
}

u16 Cpu::get_operand_address(AddressingMode mode) {
    u16 address;

    switch (mode) {
    case am_ABS: {
        address = bus->read_u16(PC);
        PC += 2; // 2 bytes read
        break;
    }
    case am_ABX: {
        address = bus->read_u16(PC);

        switch (instruction->op) {
        case op_ASL: case op_DEC: case op_INC: case op_LSR: case op_ROL: case op_ROR: case op_STA:
            // no change occurs to cycle times
            break;
        default:
            handle_page_break(address, address + X);
            break;
        }

        address += X;
        PC += 2;
        break;
    }
    case am_ABY: {
        address = bus->read_u16(PC);

        switch (instruction->op) {
        case op_STA:
            // no change occurs to cycle times
            break;
        default:
            handle_page_break(address, address + X);
            break;
        }

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
        u16 lo = bus->read_u8(pointer);
        u16 hi = bus->read_u8((pointer & 0xFF00) | ((pointer + 1) & 0x00FF));

        address = (hi << 8) | lo;
        PC += 2;
        break;
    }
    case am_INX: {
        u16 pointer = bus->read_u8(PC);

        u16 lo = bus->read_u8((pointer + X) & 0xFF);
        u16 hi = bus->read_u8((pointer + X + 1) & 0xFF);

        address = (hi << 8) | lo;
        PC += 1;
        break;
    }
    case am_INY: {
        u16 pointer = bus->read_u8(PC);

        u16 lo = bus->read_u8(pointer & 0xFF);
        u16 hi = bus->read_u8((pointer + 1) & 0xFF);

        address = (hi << 8) | lo;

        switch (instruction->op) {
        case op_STA:
            // no change occurs to cycle times
            break;
        default:
            handle_page_break(address, address + X);
            break;
        }

        address += Y;
        PC += 1;
        break;
    }
    case am_REL: {
        auto offset = (int8_t) bus->read_u8(PC); // signed offset for branching

        address = PC + offset;
        PC += 1;
        break;
    }
    case am_ZP0: {
        address = bus->read_u8(PC);
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

//

void Cpu::set_C_status(bool status) {
    P &= ~C;
    P |= (status ? C : 0);
}
void Cpu::set_Z_status(bool status) {
    P &= ~Z;
    P |= (status ? Z : 0);
}
void Cpu::set_I_status(bool status) {
    P &= ~I;
    P |= (status ? I : 0);
}
void Cpu::set_D_status(bool status) {
    P &= ~D;
    P |= (status ? D : 0);
}
void Cpu::set_B_status(bool status) {
    P &= ~B;
    P |= (status ? B : 0);
}
void Cpu::set_V_status(bool status) {
    P &= ~V;
    P |= (status ? V : 0);
}
void Cpu::set_N_status(bool status) {
    P &= ~N;
    P |= (status ? N : 0);
}

void Cpu::set_ZN_status(u8 num) {
    set_Z_status(num == 0);
    set_N_status((num & 0x80) == 0x80);
}

//

void Cpu::branch_to(u16 address) {
    handle_page_break(PC, address);
    PC = address;
}

void Cpu::handle_page_break(u16 a1, u16 a2) {
    // if two addresses have different hi-bytes, they are in different pages, i.e. page break.
    if ((a1 & 0xFF00) != (a2 & 0xFF00)) {
        cyclesRemaining++;
    }
}
