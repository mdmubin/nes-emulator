#include "nes/cpu.hpp"
#include "nes/bus.hpp"
#include "nes/instructions.hpp"

#ifndef NDEBUG
  #include <iostream>
  #include <string>
#endif

using namespace nes;
using namespace std;

void Cpu::load_and_run(const vector<u8> &program) {
    load_program(program);
    reset();
    run();
}

void Cpu::reset() {
    A  = 0;
    X  = 0;
    Y  = 0;
    P  = 0;
    SP = 0;
    PC = bus->read_u16(0xFFFC);
}

void Cpu::connect_bus(Bus *b) {
    bus = b;
}

void Cpu::load_program(const vector<u8> &program) {
    bus->mem.load_program(program);
    bus->write_u16(0x8000, 0xFFFC);
}

void Cpu::run() {
    bool interrupt = false;

    while (!interrupt) {
        u8 opcode = bus->read_u8(PC++);

        switch (opcode) {
        case Opcode::BRK: {
            interrupt = true;
            break;
        }

        case Opcode::LDA: {
            A = bus->read_u8(PC++);
            set_zero(A == 0);
            set_negative(A & 0x80);
            break;
        }

        case Opcode::TAX: {
            X = A;
            set_zero(X == 0x00);
            set_negative(X & 0x80);
            break;
        }

        default:
            // unknown opcode
            break;
        }

#ifndef NDEBUG
        // debug status registers
        cout << std::hex
             << "A: "  << (int) A  << ", "
             << "X: "  << (int) X  << ", "
             << "Y: "  << (int) Y  << ", "
             << "P: "  << (int) P  << ", "
             << "SP: " << (int) SP << ", "
             << "PC: " << (int) PC << ", "
             << "\n";
#endif
    }
}


void Cpu::set_carry(bool status) {
    set_bit_value(P, Status::C, status);
}

void Cpu::set_zero(bool status) {
    set_bit_value(P, Status::Z, status);
}

void Cpu::set_interrupt_disabled(bool status) {
    set_bit_value(P, Status::I, status);
}

void Cpu::set_decimal_mode(bool status) {
    set_bit_value(P, Status::D, status);
}

void Cpu::set_break(bool status) {
    set_bit_value(P, Status::B, status);
}

void Cpu::set_overflow(bool status) {
    set_bit_value(P, Status::V, status);
}

void Cpu::set_negative(bool status) {
    set_bit_value(P, Status::N, status);
}

bool Cpu::get_status(Cpu::Status status) const {
    return get_bit_value(P, status);
}
