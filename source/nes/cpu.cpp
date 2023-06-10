#include "nes/cpu/cpu.hpp"
#include "nes/cpu/opcodes.hpp"

#include <iostream>
#include <string>

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
             << "P: "  << to_string(P)  << ", "
             << "X: "  << to_string(X)  << ", "
             << "A: "  << to_string(A)  << ", "
             << "PC: " << to_string(PC) << ", "
             << "\n";
#endif
    }
}