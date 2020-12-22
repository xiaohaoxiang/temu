#include "processor.h"
#include "defs.h"
#include <cstring>
#include <limits>

const char *const regname[] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2",
                               "$t3",   "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5",
                               "$s6",   "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

void processor::exec_typei(instruction instr)
{
    auto cur = instr.i;
    switch (cur.op)
    {
    case 0b000100U: { // beq
        exec_beq(instr);
        break;
    }
    case 0b000101U: { // bne
        exec_bne(instr);
        break;
    }
    case 0b000001U: {
        switch (cur.rt)
        {
        case 0b00000U: { // bltz
            exec_bltz(instr);
            break;
        }
        case 0b00001U: { // bgez
            exec_bgez(instr);
            break;
        }
        case 0b10000U: { // bltzal
            exec_bltzal(instr);
            break;
        }
        case 0b10001U: { // bgezal
            exec_bgezal(instr);
            break;
        }
        }
        break;
    }
    case 0b000110U: { // blez
        exec_blez(instr);
        break;
    }
    case 0b000111U: { // bgtz
        exec_bgtz(instr);
        break;
    }
    case 0b001000U: { // addi
        exec_addi(instr);
        break;
    }
    case 0b001001U: { // addiu
        exec_addiu(instr);
        break;
    }
    case 0b001010U: { // slti
        exec_slti(instr);
        break;
    }
    case 0b001011U: { // sltiu
        exec_sltiu(instr);
        break;
    }
    case 0b001100U: { // andi
        exec_andi(instr);
        break;
    }
    case 0b001101U: { // ori
        exec_ori(instr);
        break;
    }
    case 0b001110U: { // xori
        exec_xori(instr);
        break;
    }
    case 0b001111U: { // lui
        exec_lui(instr);
        break;
    }
    case 0b100000U: { // lb
        exec_lb(instr);
        break;
    }
    case 0b100001U: { // lh
        exec_lh(instr);
        break;
    }
    case 0b100011U: { // lw
        exec_lw(instr);
        break;
    }
    case 0b100100U: { // lbu
        exec_lbu(instr);
        break;
    }
    case 0b100101U: { // lhu
        exec_lhu(instr);
        break;
    }
    case 0b101000U: { // sb
        exec_sb(instr);
        break;
    }
    case 0b101001U: { // sh
        exec_sh(instr);
        break;
    }
    case 0b101011U: { // sw
        exec_sw(instr);
        break;
    }
    }
}

void processor::exec_typer(instruction instr)
{
    auto cur = instr.r;
    if (cur.op == 0)
    {
        switch (cur.func)
        {
        case 0b000000U: { // sll
            exec_sll(instr);
            break;
        }
        case 0b000010U: { // srl
            exec_srl(instr);
            break;
        }
        case 0b000011U: { // sra
            exec_sra(instr);
            break;
        }
        case 0b000100U: { // sllv
            exec_sllv(instr);
            break;
        }
        case 0b000110U: { // srlv
            exec_srlv(instr);
            break;
        }
        case 0b000111U: { // srav
            exec_srav(instr);
            break;
        }
        case 0b001000U: { // jr
            exec_jr(instr);
            break;
        }
        case 0b001001U: { // jalr
            exec_jalr(instr);
            break;
        }
        case 0b001100U: { // syscall
            exec_syscall(instr);
            break;
        }
        case 0b010000U: { // mfhi
            exec_mfhi(instr);
            break;
        }
        case 0b010001U: { // mthi
            exec_mthi(instr);
            break;
        }
        case 0b010010U: { // mflo
            exec_mflo(instr);
            break;
        }
        case 0b010011U: { // mtlo
            exec_mtlo(instr);
            break;
        }
        case 0b011000U: { // mult
            exec_mult(instr);
            break;
        }
        case 0b011001U: { // multu
            exec_multu(instr);
            break;
        }
        case 0b011010U: { // div
            exec_div(instr);
            break;
        }
        case 0b011011U: { // divu
            exec_divu(instr);
            break;
        }
        case 0b100000U: { // add
            exec_add(instr);
            break;
        }
        case 0b100001U: { // addu
            exec_addu(instr);
            break;
        }
        case 0b100010U: { // sub
            exec_sub(instr);
            break;
        }
        case 0b100011U: { // subu
            exec_subu(instr);
            break;
        }
        case 0b100100U: { // and
            exec_and(instr);
            break;
        }
        case 0b100101U: { // or
            exec_or(instr);
            break;
        }
        case 0b100110U: { // xor
            exec_xor(instr);
            break;
        }
        case 0b100111U: { // nor
            exec_nor(instr);
            break;
        }
        case 0b101010U: { // slt
            exec_slt(instr);
            break;
        }
        case 0b101011U: { // sltu
            exec_sltu(instr);
            break;
        }
        }
    }
    else if (cur.op == 0b010000U)
    {
        switch (cur.rs)
        {
        case 0b00000U: { // mfc0
            exec_mfc0(instr);
            break;
        }
        case 0b00100U: { // mtc0
            exec_mtc0(instr);
            break;
        }
        case 0b10000U: { // eret
            exec_eret(instr);
            break;
        }
        }
    }
}

void processor::exec_typej(instruction instr)
{
    auto cur = instr.j;
    switch (cur.op)
    {
    case 0b000010U: { // j
        exec_j(instr);
        break;
    }
    case 0b000011U: { // jal
        exec_jal(instr);
        break;
    }
    }
}

#define instr_def(name) inline void processor::exec_##name(instruction instr)

instr_def(beq)
{
    if (regs.reg[instr.i.rs]._32 == regs.reg[instr.i.rt]._32)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        jump(regs.pc + (offset << 2));
    }
}

instr_def(bne)
{
    if (regs.reg[instr.i.rs]._32 != regs.reg[instr.i.rt]._32)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        jump(regs.pc + (offset << 2));
    }
}

instr_def(bltz)
{
    if (uint32_t(regs.reg[instr.i.rs]._32) < 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        jump(regs.pc + (offset << 2));
    }
}

instr_def(bgez)
{
    if (uint32_t(regs.reg[instr.i.rs]._32) >= 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        jump(regs.pc + (offset << 2));
    }
}

instr_def(bltzal)
{
    regs.reg[$ra]._32 = regs.pc + 4U;
    if (uint32_t(regs.reg[instr.i.rs]._32) < 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        jump(regs.pc + (offset << 2));
    }
}

instr_def(bgezal)
{
    regs.reg[$ra]._32 = regs.pc + 4U;
    if (uint32_t(regs.reg[instr.i.rs]._32) >= 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        jump(regs.pc + (offset << 2));
    }
}

instr_def(blez)
{
    if (uint32_t(regs.reg[instr.i.rs]._32) <= 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        jump(regs.pc + (offset << 2));
    }
}

instr_def(bgtz)
{
    if (uint32_t(regs.reg[instr.i.rs]._32) > 0)
    {
        const int32_t offset = signedext(instr.i.imm, 16, 32);
        jump(regs.pc + (offset << 2));
    }
}

instr_def(addi)
{
    int64_t sum = int64_t(regs.reg[instr.i.rs]._32) + signedext(instr.i.imm, 16, 64);
    if (sum < std::numeric_limits<int32_t>::min() || std::numeric_limits<int32_t>::max() < sum)
    {
        // signal exception(integer overflow)
        exception(exc_ov);
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
        exception(exc_adel);
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
        exception(exc_adel);
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
        exception(exc_adel);
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
        exception(exc_ades);
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
        exception(exc_ades);
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

instr_def(srlv)
{
    regs.reg[instr.r.rd]._32 = regs.reg[instr.r.rt]._32 >> (regs.reg[instr.r.rs]._32 & 0b11111U);
}

instr_def(srav)
{
    regs.reg[instr.r.rd]._32 = int32_t(regs.reg[instr.r.rt]._32) >> (regs.reg[instr.r.rs]._32 & 0b11111U);
}

instr_def(jr)
{
    jump(regs.reg[instr.r.rs]._32);
}

instr_def(jalr)
{
    regs.reg[instr.r.rd]._32 = regs.pc + 4U;
    jump(regs.reg[instr.r.rs]._32);
}

instr_def(syscall)
{
    // signal exception(system call)
    exception(exc_sys);
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
    if (b == 0)
    {
        regs.lo = 0U;
        regs.hi = 0U;
    }
    else
    {
        int32_t q = a / b, r = a % b;
        regs.lo = uint32_t(q);
        regs.hi = uint32_t(r);
    }
}

instr_def(divu)
{
    uint32_t a = regs.reg[instr.r.rs]._32, b = regs.reg[instr.r.rt]._32;
    if (b == 0)
    {
        regs.lo = 0U;
        regs.hi = 0U;
    }
    else
    {
        uint32_t q = a / b, r = a % b;
        regs.lo = q;
        regs.hi = r;
    }
}

instr_def(add)
{
    int64_t sum = int64_t(regs.reg[instr.r.rs]._32) + int64_t(regs.reg[instr.r.rt]._32);
    if (sum < std::numeric_limits<int32_t>::min() || std::numeric_limits<int32_t>::max() < sum)
    {
        // signal exception(integer overflow)
        exception(exc_ov);
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
        exception(exc_ov);
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
    uint32_t val;
    switch (instr.r.rd)
    {
    case $badvaddr: {
        val = regs.badvaddr;
        break;
    }
    case $status: {
        val = regs.status.val;
        break;
    }
    case $cause: {
        val = regs.cause.val;
        break;
    }
    case $epc: {
        val = regs.epc;
        break;
    }
    default: {
        val = 0;
        break;
    }
    }
    regs.reg[instr.r.rt]._32 = val;
}

instr_def(mtc0)
{
    static uint32_t nulldst;
    uint32_t &ref = [&]() -> uint32_t & {
        switch (instr.r.rd)
        {
        case $badvaddr: {
            return regs.badvaddr;
        }
        case $status: {
            return regs.status.val;
        }
        case $cause: {
            return regs.cause.val;
        }
        case $epc: {
            return regs.epc;
        }
        default: {
            return nulldst;
        }
        }
    }();
    ref = regs.reg[instr.r.rt]._32;
}

instr_def(eret)
{
    regs.status.field.exl = false;
    jump(regs.epc);
}

instr_def(j)
{
    jump((regs.pc & 0xF0000000) | (instr.j.addr << 2));
}

instr_def(jal)
{
    regs.reg[$ra]._32 = regs.pc + 4U;
    jump((regs.pc & 0xF0000000) | (instr.j.addr << 2));
}

#undef instr_def

void processor::exception(uint32_t exccode)
{
    if (!regs.status.field.exl)
    {
        regs.status.field.exl = true;
        if (in_delay_slot)
        {
            regs.epc = regs.pc - 8U;
            regs.cause.field.bd = true;
        }
        else
        {
            regs.epc = regs.pc - 4U;
            regs.cause.field.bd = false;
        }
    }
    else if (exccode == exc_int)
    {
        return;
    }
    exception_occurred = true;
    regs.cause.field.exccode = exccode;
    regs.badvaddr = regs.pc - 4U;
    if (exccode == exc_int)
    {
        regs.pc = intaddr;
    }
    else
    {
        regs.pc = excaddr;
    }
}

void processor::jump(uint32_t target)
{
    if (delayed_branches)
    {
        in_delay_slot = true;
        exec(mem.mem_read<4>(regs.pc));
        in_delay_slot = false;
        if (!exception_occurred)
        {
            regs.pc = target;
        }
    }
    else
    {
        regs.pc = target;
    }
}

processor::processor(ram &mem, bool delayed_branches)
    : mem(mem), in_delay_slot(false), exception_occurred(false), delayed_branches(delayed_branches)
{
    std::memset(&regs, 0, sizeof(regs));
}

regfile &processor::get_regs()
{
    return regs;
}

const regfile &processor::get_regs() const
{
    return regs;
}

ram &processor::get_mem()
{
    return mem;
}

const ram &processor::get_mem() const
{
    return mem;
}

void processor::exec(instruction instr)
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
