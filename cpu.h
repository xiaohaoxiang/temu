#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "mem.h"

enum : uint32_t
{
    $zero,
    $at,
    $v0,
    $v1,
    $a0,
    $a1,
    $a2,
    $a3,
    $t0,
    $t1,
    $t2,
    $t3,
    $t4,
    $t5,
    $t6,
    $t7,
    $s0,
    $s1,
    $s2,
    $s3,
    $s4,
    $s5,
    $s6,
    $s7,
    $t8,
    $t9,
    $k0,
    $k1,
    $gp,
    $sp,
    $fp,
    $ra
};

struct regfile
{
    union
    {
        uint32_t _32;
        uint16_t _16;
        uint8_t _8;
    } reg[32];
    uint32_t pc, hi, lo;
};

class cpu
{
public:
    union instruction
    {
        uint32_t val;
        struct
        {
            uint32_t func : 6;
            uint32_t sa : 5;
            uint32_t rd : 5;
            uint32_t rt : 5;
            uint32_t rs : 5;
            uint32_t op : 6;
        } r;
        struct
        {
            uint32_t imm : 16;
            uint32_t rt : 5;
            uint32_t rs : 5;
            uint32_t op : 6;
        } i;
        struct
        {
            uint32_t addr : 26;
            uint32_t op : 6;
        } j;

        instruction() = default;
        instruction(uint32_t val) : val(val) {}
    };

private:
    void exec_typei(instruction instr);
    void exec_typer(instruction instr);
    void exec_typej(instruction instr);

#define instr_declare(name) void exec_##name(instruction instr)

    // i-type
    instr_declare(beq);
    instr_declare(bne);
    instr_declare(bltz);
    instr_declare(bgez);
    instr_declare(bltzal);
    instr_declare(bgezal);
    instr_declare(blez);
    instr_declare(bgtz);
    instr_declare(addi);
    instr_declare(addiu);
    instr_declare(slti);
    instr_declare(sltiu);
    instr_declare(andi);
    instr_declare(ori);
    instr_declare(xori);
    instr_declare(lui);
    instr_declare(lb);
    instr_declare(lh);
    instr_declare(lw);
    instr_declare(lbu);
    instr_declare(lhu);
    instr_declare(sb);
    instr_declare(sh);
    instr_declare(sw);

    // r-type
    instr_declare(sll);
    instr_declare(srl);
    instr_declare(sra);
    instr_declare(sllv);
    instr_declare(srlv);
    instr_declare(srav);
    instr_declare(jr);
    instr_declare(jalr);
    instr_declare(syscall);
    instr_declare(mfhi);
    instr_declare(mthi);
    instr_declare(mflo);
    instr_declare(mtlo);
    instr_declare(mult);
    instr_declare(multu);
    instr_declare(div);
    instr_declare(divu);
    instr_declare(add);
    instr_declare(addu);
    instr_declare(sub);
    instr_declare(subu);
    instr_declare(and);
    instr_declare(or);
    instr_declare(xor);
    instr_declare(nor);
    instr_declare(slt);
    instr_declare(sltu);
    instr_declare(mfc0);
    instr_declare(mtc0);
    instr_declare(eret);

    // j-type
    instr_declare(j);
    instr_declare(jal);

#undef instr_declare

public:
    cpu(ram &mem);
    regfile &get_regs();
    const regfile &get_regs() const;
    ram &get_mem();
    const ram &get_mem() const;

    void exec(instruction instr);

private:
    regfile regs;
    ram &mem;
};

#endif // !CPU_H
