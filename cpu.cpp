#include "cpu.h"
#include "defs.h"
#include <limits>

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

#define instr_def(name) inline void cpu::exec_##name(instruction instr)

instr_def(beq)
{
    if (regs.reg[instr.i.rs]._32 == regs.reg[instr.i.rt]._32)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_def(bne)
{
    if (regs.reg[instr.i.rs]._32 != regs.reg[instr.i.rt]._32)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_def(bltz)
{
    if (uint32_t(regs.reg[instr.i.rs]._32) < 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_def(bgez)
{
    if (uint32_t(regs.reg[instr.i.rs]._32) >= 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_def(bltzal)
{
    regs.reg[$ra]._32 = regs.pc + 4U;
    if (uint32_t(regs.reg[instr.i.rs]._32) < 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_def(bgezal)
{
    regs.reg[$ra]._32 = regs.pc + 4U;
    if (uint32_t(regs.reg[instr.i.rs]._32) >= 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_def(blez)
{
    if (uint32_t(regs.reg[instr.i.rs]._32) <= 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_def(bgtz)
{
    if (uint32_t(regs.reg[instr.i.rs]._32) > 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        regs.pc += offset << 2;
    }
}

instr_def(addi)
{
    int64_t sum = int64_t(regs.reg[instr.i.rs]._32) + signedext(instr.i.imm, 16, 64);
    if (sum < std::numeric_limits<int32_t>::min() || std::numeric_limits<int32_t>::max() < sum)
    {
        // signal exception(integer overflow)
    }
    else
    {
        regs.reg[instr.i.rt]._32 = uint32_t(sum);
    }
}

instr_def(addiu)
{
    regs.reg[instr.i.rt]._32 = regs.reg[instr.i.rs]._32 + signedext(instr.i.imm, 16, 32);
}

instr_def(slti)
{
    regs.reg[instr.i.rt]._32 = int32_t(regs.reg[instr.i.rs]._32) < signedext(instr.i.imm, 16, 32);
}

instr_def(sltiu)
{
    regs.reg[instr.i.rt]._32 = regs.reg[instr.i.rs]._32 < uint32_t(signedext(instr.i.imm, 16, 32));
}

instr_def(andi)
{
    regs.reg[instr.i.rt]._32 = regs.reg[instr.i.rs]._32 & unsignedext(instr.i.imm, 16, 32);
}

instr_def(ori)
{
    regs.reg[instr.i.rt]._32 = regs.reg[instr.i.rs]._32 | unsignedext(instr.i.imm, 16, 32);
}

instr_def(xori)
{
    regs.reg[instr.i.rt]._32 = regs.reg[instr.i.rs]._32 ^ unsignedext(instr.i.imm, 16, 32);
}

instr_def(lui)
{
    regs.reg[instr.i.rt]._32 = uint32_t(instr.i.imm) << 16;
}

instr_def(lb)
{
    uint32_t addr = uint32_t(signedext(instr.i.imm, 16, 32) + regs.reg[instr.i.rs]._32);
    uint8_t membyte = uint8_t(mem.mem_read<1>(addr));
    regs.reg[instr.i.rt]._32 = signedext(membyte, 8, 32);
}

instr_def(lh)
{
    uint32_t addr = uint32_t(signedext(instr.i.imm, 16, 32) + regs.reg[instr.i.rs]._32);
    if (addr & 1U)
    {
        // signal exception(address error)
    }
    else
    {
        uint16_t memhalfword = uint16_t(mem.mem_read<2>(addr));
        regs.reg[instr.i.rt]._32 = signedext(memhalfword, 16, 32);
    }
}

instr_def(lw)
{
    uint32_t addr = uint32_t(signedext(instr.i.imm, 16, 32) + regs.reg[instr.i.rs]._32);
    if (addr & 3U)
    {
        // signal exception(address error)
    }
    else
    {
        uint32_t memword = mem.mem_read<4>(addr);
        regs.reg[instr.i.rt]._32 = memword;
    }
}

instr_def(lbu)
{
    uint32_t addr = uint32_t(signedext(instr.i.imm, 16, 32) + regs.reg[instr.i.rs]._32);
    regs.reg[instr.i.rt]._32 = mem.mem_read<1>(addr);
}

instr_def(lhu)
{
    uint32_t addr = uint32_t(signedext(instr.i.imm, 16, 32) + regs.reg[instr.i.rs]._32);
    if (addr & 1U)
    {
        // signal exception(address error)
    }
    else
    {
        regs.reg[instr.i.rt]._32 = mem.mem_read<2>(addr);
    }
}

instr_def(sb)
{
    uint32_t addr = uint32_t(signedext(instr.i.imm, 16, 32) + regs.reg[instr.i.rs]._32);
    mem.mem_write<1>(addr, unsignedext(regs.reg[instr.i.rt]._8, 8, 32));
}

instr_def(sh)
{
    uint32_t addr = uint32_t(signedext(instr.i.imm, 16, 32) + regs.reg[instr.i.rs]._32);
    if (addr & 1U)
    {
        // signal exception(address error)
    }
    else
    {
        mem.mem_write<2>(addr, unsignedext(regs.reg[instr.i.rt]._16, 16, 32));
    }
}

instr_def(sw)
{
    uint32_t addr = uint32_t(signedext(instr.i.imm, 16, 32) + regs.reg[instr.i.rs]._32);
    if (addr & 3U)
    {
        // signal exception(address error)
    }
    else
    {
        mem.mem_write<4>(addr, regs.reg[instr.i.rt]._32);
    }
}

instr_def(sll)
{
    regs.reg[instr.r.rd]._32 = regs.reg[instr.r.rt]._32 << instr.r.sa;
}

instr_def(srl)
{
    regs.reg[instr.r.rd]._32 = regs.reg[instr.r.rt]._32 >> instr.r.sa;
}

instr_def(sra)
{
    regs.reg[instr.r.rd]._32 = int32_t(regs.reg[instr.r.rt]._32) >> instr.r.sa;
}

instr_def(sllv)
{
    regs.reg[instr.r.rd]._32 = regs.reg[instr.r.rt]._32 << (regs.reg[instr.r.rs]._32 & 0b11111U);
}

instr_def(srav)
{
    regs.reg[instr.r.rd]._32 = regs.reg[instr.r.rt]._32 >> (regs.reg[instr.r.rs]._32 & 0b11111U);
}

instr_def(jr)
{
    regs.pc = regs.reg[instr.r.rs]._32;
}

instr_def(jalr)
{
    regs.reg[instr.r.rd]._32 = regs.pc + 4U;
    regs.pc = regs.reg[instr.r.rs]._32;
}

instr_def(syscall)
{
    // signal exception(system call)
}

instr_def(mfhi)
{
    regs.reg[instr.r.rd]._32 = regs.hi;
}

instr_def(mthi)
{
    regs.hi = regs.reg[instr.r.rs]._32;
}

instr_def(mflo)
{
    regs.reg[instr.r.rd]._32 = regs.lo;
}

instr_def(mtlo)
{
    regs.lo = regs.reg[instr.r.rs]._32;
}

instr_def(mult)
{
    int64_t prod = int64_t(regs.reg[instr.r.rs]._32) * int64_t(regs.reg[instr.r.rt]._32);
    regs.lo = uint32_t(prod);
    regs.hi = uint32_t(prod >> 32);
}

instr_def(multu)
{
    uint64_t prod = regs.reg[instr.r.rs]._32 * regs.reg[instr.r.rt]._32;
    regs.lo = uint32_t(prod);
    regs.hi = uint32_t(prod >> 32);
}

instr_def(div)
{
    int32_t a = int32_t(regs.reg[instr.r.rs]._32), b = int32_t(regs.reg[instr.r.rt]._32);
    int32_t q = a / b, r = a % b;
    regs.lo = uint32_t(q);
    regs.hi = uint32_t(r);
}

instr_def(divu)
{
    uint32_t a = regs.reg[instr.r.rs]._32, b = regs.reg[instr.r.rt]._32;
    uint32_t q = a / b, r = a % b;
    regs.lo = q;
    regs.hi = r;
}

instr_def(add)
{
    int64_t sum = int64_t(regs.reg[instr.r.rs]._32) + int64_t(regs.reg[instr.r.rt]._32);
    if (sum < std::numeric_limits<int32_t>::min() || std::numeric_limits<int32_t>::max() < sum)
    {
        // signal exception(integer overflow)
    }
    else
    {
        regs.reg[instr.r.rd]._32 = uint32_t(sum);
    }
}

instr_def(addu)
{
    regs.reg[instr.r.rd]._32 = regs.reg[instr.r.rs]._32 + regs.reg[instr.r.rt]._32;
}

instr_def(sub)
{
    int64_t diff = int64_t(regs.reg[instr.r.rs]._32) - int64_t(regs.reg[instr.r.rt]._32);
    if (diff < std::numeric_limits<int32_t>::min() || std::numeric_limits<int32_t>::max() < diff)
    {
        // signal exception(integer overflow)
    }
    else
    {
        regs.reg[instr.r.rd]._32 = uint32_t(diff);
    }
}

instr_def(subu)
{
    regs.reg[instr.r.rd]._32 = regs.reg[instr.r.rs]._32 - regs.reg[instr.r.rt]._32;
}

instr_def(and)
{
    regs.reg[instr.r.rd]._32 = regs.reg[instr.r.rs]._32 & regs.reg[instr.r.rt]._32;
}

instr_def(or)
{
    regs.reg[instr.r.rd]._32 = regs.reg[instr.r.rs]._32 | regs.reg[instr.r.rt]._32;
}

instr_def(xor)
{
    regs.reg[instr.r.rd]._32 = regs.reg[instr.r.rs]._32 ^ regs.reg[instr.r.rt]._32;
}

instr_def(nor)
{
    regs.reg[instr.r.rd]._32 = ~(regs.reg[instr.r.rs]._32 | regs.reg[instr.r.rt]._32);
}

instr_def(slt)
{
    regs.reg[instr.r.rd]._32 = int32_t(regs.reg[instr.r.rs]._32) < int32_t(regs.reg[instr.r.rt]._32);
}

instr_def(sltu)
{
    regs.reg[instr.r.rd]._32 = regs.reg[instr.r.rs]._32 < regs.reg[instr.r.rt]._32;
}

instr_def(mfc0)
{
    
}

#undef instr_def

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
    regs.pc += 4U;
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
