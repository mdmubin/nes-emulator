#pragma once

namespace nes {

/// Operation / Instruction types for the CPU
enum OperationType {
    // legal instructions
    op_ADC, // addition with carry
    op_AND, // logical AND
    op_ASL, // arithmetic shift left
    op_BCC, // branch on carry clear
    op_BCS, // branch on carry set
    op_BEQ, // branch on equal
    op_BIT, // bit test
    op_BMI, // branch on negative
    op_BNE, // branch on not equal
    op_BPL, // branch on positive
    op_BRK, // force interrupt
    op_BVC, // branch on overflow clear
    op_BVS, // branch on overflow set
    op_CLC, // clear carry
    op_CLD, // clear decimal mode
    op_CLI, // clear interrupt disable
    op_CLV, // clear overflow
    op_CMP, // compare with A
    op_CPX, // compare with X
    op_CPY, // compare with Y
    op_DEC, // decrement value at memory
    op_DEX, // decrement value at X
    op_DEY, // decrement value at Y
    op_EOR, // exclusive OR with A
    op_INC, // increment value at memory
    op_INX, // increment value at X
    op_INY, // increment value at Y
    op_JMP, // jump to address
    op_JSR, // jump to subroutine
    op_LDA, // load to A
    op_LDX, // load to X
    op_LDY, // load to Y
    op_LSR, // logical shift right
    op_NOP, // no operation
    op_ORA, // logical OR with A
    op_PHA, // push A to stack
    op_PHP, // push P to stack
    op_PLA, // pull A from stack
    op_PLP, // pull P from stack
    op_ROL, // rotate left
    op_ROR, // rotate right
    op_RTI, // return from interrupt
    op_RTS, // return from subroutine
    op_SBC, // subtraction with carry
    op_SEC, // set carry
    op_SED, // set decimal mode
    op_SEI, // set interrupt disable
    op_STA, // store A to memory
    op_STX, // store X to memory
    op_STY, // store Y to memory
    op_TAX, // transfer A to X
    op_TAY, // transfer A to Y
    op_TSX, // transfer SP to X
    op_TXA, // transfer X to A
    op_TXS, // transfer X to SP
    op_TYA, // transfer Y to A

    op_XXX, // unknown op
};

/// Addressing mode for the CPU
enum AddressingMode {
    am_ABS, // absolute
    am_ABX, // absolute with offset x
    am_ABY, // absolute with offset y
    am_IMM, // immediate
    am_IMP, // implied
    am_IND, // indirect
    am_INX, // indirect with offset x
    am_INY, // indirect with offset y
    am_REL, // relative
    am_ZP0, // zero page
    am_ZPX, // zero page with offset x
    am_ZPY, // zero page with offset y
};

struct Instruction {
    OperationType op;
    AddressingMode mode;
};

static const Instruction INSTRUCTION_LOOKUP[256] = {
    // HI\LO    0x0              0x1              0x2              0x3              0x4              0x5              0x6              0x7              0x8              0x9              0xA              0xB              0xC              0xD              0xE              0xF
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    /* 0x0 */  {op_BRK,am_IMP}, {op_ORA,am_INX}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_ORA,am_ZP0}, {op_ASL,am_ZP0}, {op_XXX,am_IMP}, {op_PHP,am_IMP}, {op_ORA,am_IMM}, {op_ASL,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_ORA,am_ABS}, {op_ASL,am_ABS}, {op_XXX,am_IMP},
    /* 0x1 */  {op_BPL,am_REL}, {op_ORA,am_INY}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_ORA,am_ZPX}, {op_ASL,am_ZPX}, {op_XXX,am_IMP}, {op_CLC,am_IMP}, {op_ORA,am_ABY}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_ORA,am_ABX}, {op_ASL,am_ABX}, {op_XXX,am_IMP},
    /* 0x2 */  {op_JSR,am_ABS}, {op_AND,am_INX}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_BIT,am_ZP0}, {op_AND,am_ZP0}, {op_ROL,am_ZP0}, {op_XXX,am_IMP}, {op_PLP,am_IMP}, {op_AND,am_IMM}, {op_ROL,am_IMP}, {op_XXX,am_IMP}, {op_BIT,am_ABS}, {op_AND,am_ABS}, {op_ROL,am_ABS}, {op_XXX,am_IMP},
    /* 0x3 */  {op_BMI,am_REL}, {op_AND,am_INY}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_AND,am_ZPX}, {op_ROL,am_ZPX}, {op_XXX,am_IMP}, {op_SEC,am_IMP}, {op_AND,am_ABY}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_AND,am_ABX}, {op_ROL,am_ABX}, {op_XXX,am_IMP},
    /* 0x4 */  {op_RTI,am_IMP}, {op_EOR,am_INX}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_EOR,am_ZP0}, {op_LSR,am_ZP0}, {op_XXX,am_IMP}, {op_PHA,am_IMP}, {op_EOR,am_IMM}, {op_LSR,am_IMP}, {op_XXX,am_IMP}, {op_JMP,am_ABS}, {op_EOR,am_ABS}, {op_LSR,am_ABS}, {op_XXX,am_IMP},
    /* 0x5 */  {op_BVC,am_REL}, {op_EOR,am_INY}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_EOR,am_ZPX}, {op_LSR,am_ZPX}, {op_XXX,am_IMP}, {op_CLI,am_IMP}, {op_EOR,am_ABY}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_EOR,am_ABX}, {op_LSR,am_ABX}, {op_XXX,am_IMP},
    /* 0x6 */  {op_RTS,am_IMP}, {op_ADC,am_INX}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_ADC,am_ZP0}, {op_ROR,am_ZP0}, {op_XXX,am_IMP}, {op_PLA,am_IMP}, {op_ADC,am_IMM}, {op_ROR,am_IMP}, {op_XXX,am_IMP}, {op_JMP,am_IND}, {op_ADC,am_ABS}, {op_ROR,am_ABS}, {op_XXX,am_IMP},
    /* 0x7 */  {op_BVS,am_REL}, {op_ADC,am_INY}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_ADC,am_ZPX}, {op_ROR,am_ZPX}, {op_XXX,am_IMP}, {op_SEI,am_IMP}, {op_ADC,am_ABY}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_ADC,am_ABX}, {op_ROR,am_ABX}, {op_XXX,am_IMP},
    /* 0x8 */  {op_XXX,am_IMP}, {op_STA,am_INX}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_STY,am_ZP0}, {op_STA,am_ZP0}, {op_STX,am_ZP0}, {op_XXX,am_IMP}, {op_DEY,am_IMP}, {op_XXX,am_IMP}, {op_TXA,am_IMP}, {op_XXX,am_IMP}, {op_STY,am_ABS}, {op_STA,am_ABS}, {op_STX,am_ABS}, {op_XXX,am_IMP},
    /* 0x9 */  {op_BCC,am_REL}, {op_STA,am_INY}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_STY,am_ZPX}, {op_STA,am_ZPX}, {op_STX,am_ZPY}, {op_XXX,am_IMP}, {op_TYA,am_IMP}, {op_STA,am_ABY}, {op_TXS,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_STA,am_ABX}, {op_XXX,am_IMP}, {op_XXX,am_IMP},
    /* 0xA */  {op_LDY,am_IMM}, {op_LDA,am_INX}, {op_LDX,am_IMM}, {op_XXX,am_IMP}, {op_LDY,am_ZP0}, {op_LDA,am_ZP0}, {op_LDX,am_ZP0}, {op_XXX,am_IMP}, {op_TAY,am_IMP}, {op_LDA,am_IMM}, {op_TAX,am_IMP}, {op_XXX,am_IMP}, {op_LDY,am_ABS}, {op_LDA,am_ABS}, {op_LDX,am_ABS}, {op_XXX,am_IMP},
    /* 0xB */  {op_BCS,am_REL}, {op_LDA,am_INY}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_LDY,am_ZPX}, {op_LDA,am_ZPX}, {op_LDX,am_ZPY}, {op_XXX,am_IMP}, {op_CLV,am_IMP}, {op_LDA,am_ABY}, {op_TSX,am_IMP}, {op_XXX,am_IMP}, {op_LDY,am_ABX}, {op_LDA,am_ABX}, {op_LDX,am_ABY}, {op_XXX,am_IMP},
    /* 0xC */  {op_CPY,am_IMM}, {op_CMP,am_INX}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_CPY,am_ZP0}, {op_CMP,am_ZP0}, {op_DEC,am_ZP0}, {op_XXX,am_IMP}, {op_INY,am_IMP}, {op_CMP,am_IMM}, {op_DEX,am_IMP}, {op_XXX,am_IMP}, {op_CPY,am_ABS}, {op_CMP,am_ABS}, {op_DEC,am_ABS}, {op_XXX,am_IMP},
    /* 0xD */  {op_BNE,am_REL}, {op_CMP,am_INY}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_CMP,am_ZPX}, {op_DEC,am_ZPX}, {op_XXX,am_IMP}, {op_CLD,am_IMP}, {op_CMP,am_ABY}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_CMP,am_ABX}, {op_DEC,am_ABX}, {op_XXX,am_IMP},
    /* 0xE */  {op_CPX,am_IMM}, {op_SBC,am_INX}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_CPX,am_ZP0}, {op_SBC,am_ZP0}, {op_INC,am_ZP0}, {op_XXX,am_IMP}, {op_INX,am_IMP}, {op_SBC,am_IMM}, {op_NOP,am_IMP}, {op_XXX,am_IMP}, {op_CPX,am_ABS}, {op_SBC,am_ABS}, {op_INC,am_ABS}, {op_XXX,am_IMP},
    /* 0xF */  {op_BEQ,am_REL}, {op_SBC,am_INY}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_SBC,am_ZPX}, {op_INC,am_ZPX}, {op_XXX,am_IMP}, {op_SED,am_IMP}, {op_SBC,am_ABY}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_XXX,am_IMP}, {op_SBC,am_ABX}, {op_INC,am_ABX}, {op_XXX,am_IMP},
};

} // namespace nes