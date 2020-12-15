#include "cpu.h"
#include "defs.h"

void cpu::exec_typei(instruction instr)
{
    auto cur = instr.i;
    switch (cur.op)
    {
    case 0b000100U:
    { // beq
        break;
    }
    case 0b000101U:
    { // bne
        break;
    }
    case 0b000001U:
    {
        switch (cur.rt)
        {
        case 0b00000U:
        { // bltz
            break;
        }
        case 0b00001U:
        { // bgez
            break;
        }
        case 0b10000U:
        { // bltzal
            break;
        }
        case 0b10001U:
        { // bgezal
            break;
        }
        }
        break;
    }
    case 0b000110U:
    { // blez
        break;
    }
    case 0b000111U:
    { // bgtz
        break;
    }
    case 0b001000U:
    { // addi
        break;
    }
    case 0b001001U:
    { // addiu
        break;
    }
    case 0b001010U:
    { // slti
        break;
    }
    case 0b001011U:
    { // sltiu
        break;
    }
    case 0b001100U:
    { // andi
        break;
    }
    case 0b001101U:
    { // ori
        break;
    }
    case 0b001110U:
    { // xori
        break;
    }
    case 0b001111U:
    { // lui
        break;
    }
    case 0b100000U:
    { // lb
        break;
    }
    case 0b100001U:
    { // lh
        break;
    }
    case 0b100011U:
    { // lw
        break;
    }
    case 0b100100U:
    { // lbu
        break;
    }
    case 0b100101U:
    { // lhu
        break;
    }
    case 0b101000U:
    { // sb
        break;
    }
    case 0b101001U:
    { // sh
        break;
    }
    case 0b101011U:
    { // sw
        break;
    }
    }
}

void cpu::exec_typer(instruction instr)
{
    auto cur = instr.r;
    if (cur.op == 0)
    {
        switch (cur.func)
        {
        case 0b000000U:
        { // sll
            break;
        }
        case 0b000010U:
        { // srl
            break;
        }
        case 0b000011U:
        { // sra
            break;
        }
        case 0b000100U:
        { // sllv
            break;
        }
        case 0b000110U:
        { // srlv
            break;
        }
        case 0b000111U:
        { // srav
            break;
        }
        case 0b001000U:
        { // jr
            break;
        }
        case 0b001001U:
        { // jalr
            break;
        }
        case 0b001100U:
        { // syscall
            break;
        }
        case 0b010000U:
        { // mfhi
            break;
        }
        case 0b010001U:
        { // mthi
            break;
        }
        case 0b010010U:
        { // mflo
            break;
        }
        case 0b010011U:
        { // mtlo
            break;
        }
        case 0b011000U:
        { // mult
            break;
        }
        case 0b011001U:
        { // multu
            break;
        }
        case 0b011010U:
        { // div
            break;
        }
        case 0b011011U:
        { // divu
            break;
        }
        case 0b100000U:
        { // add
            break;
        }
        case 0b100001U:
        { // addu
            break;
        }
        case 0b100010U:
        { // sub
            break;
        }
        case 0b100011U:
        { // subu
            break;
        }
        case 0b100100U:
        { // and
            break;
        }
        case 0b100101U:
        { // or
            break;
        }
        case 0b100110U:
        { // xor
            break;
        }
        case 0b100111U:
        { // nor
            break;
        }
        case 0b101010U:
        { // slt
            break;
        }
        case 0b101011U:
        { // sltu
            break;
        }
        }
    }
    else if (cur.op == 0b010000U)
    {
        switch (cur.rs)
        {
        case 0b00000U:
        { // mfc0
            break;
        }
        case 0b00100U:
        { // mtc0
            break;
        }
        case 0b10000U:
        { // eret
            break;
        }
        }
    }
}

void cpu::exec_typej(instruction instr)
{
    auto cur = instr.j;
    switch (cur.op)
    {
    case 0b000010U:
    { // j
        regs.pc = ((regs.pc + 4U) & 0xF0000000) | (cur.addr << 2);
        break;
    }
    case 0b000011U:
    { // jal
        break;
    }
    }
}

#define instr_define(name) inline void cpu::exec_##name(instruction instr)

instr_define(beq)
{
    regs.pc += 4U;
    if (regs.reg[instr.i.rs]._32 == regs.reg[instr.i.rt]._32)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_define(bne)
{
    regs.pc += 4U;
    if (regs.reg[instr.i.rs]._32 != regs.reg[instr.i.rt]._32)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_define(bltz)
{
    regs.pc += 4U;
    if (uint32_t(regs.reg[instr.i.rs]._32) < 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_define(bgez)
{
    regs.pc += 4U;
    if (uint32_t(regs.reg[instr.i.rs]._32) >= 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_define(bltzal)
{
    regs.reg[$ra]._32 = regs.pc + 8U;
    regs.pc += 4U;
    if (uint32_t(regs.reg[instr.i.rs]._32) < 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_define(bgezal)
{
    regs.reg[$ra]._32 = regs.pc + 8U;
    regs.pc += 4U;
    if (uint32_t(regs.reg[instr.i.rs]._32) >= 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_define(blez)
{
    regs.pc += 4U;
    if (uint32_t(regs.reg[instr.i.rs]._32) <= 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_define(bgtz)
{
    regs.pc += 4U;
    if (uint32_t(regs.reg[instr.i.rs]._32) > 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_define(addi)
{
    
}

#undef instr_define

cpu::cpu(ram &mem)
    : mem(mem)
{
}

regfile &cpu::get_regs()
{
    return regs;
}

const regfile &cpu::get_regs() const
{
    return regs;
}

ram &cpu::get_mem()
{
    return mem;
}

const ram &cpu::get_mem() const
{
    return mem;
}

void cpu::exec(instruction instr)
{
    uint32_t op = instr.r.op;
    if (op == 0b000000U || op == 0b010000U)
    { // r type
        exec_typer(instr);
    }
    else if (op == 0b000010U || op == 0b000011U)
    { // j type
        exec_typej(instr);
    }
    else
    { // i type
        exec_typei(instr);
    }
}
