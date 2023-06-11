#include "nes/cpu/cpu.hpp"
#include "nes/cpu/opcodes.hpp"

#ifndef NDEBUG
  #include <iostream>
  #include <string>
#endif

using namespace nes;
using namespace std;


void Cpu::interpret(const vector<u8> &program) {
    bool interrupt = false;

    while (!interrupt) {
        u8 opcode = program[PC++];

        switch (opcode) {
        case Opcode::BRK: {
            interrupt = true;
            break;
        }

        case Opcode::LDA: {
            A = program[PC++];
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
