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
    union singlereg
    {
        uint32_t _32;
        uint16_t _16;
        uint8_t _8;
    };
    singlereg reg[32];
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

#define instr_decl(name) inline void exec_##name(instruction instr)

    // i-type
    instr_decl(beq);
    instr_decl(bne);
    instr_decl(bltz);
    instr_decl(bgez);
    instr_decl(bltzal);
    instr_decl(bgezal);
    instr_decl(blez);
    instr_decl(bgtz);
    instr_decl(addi);
    instr_decl(addiu);
    instr_decl(slti);
    instr_decl(sltiu);
    instr_decl(andi);
    instr_decl(ori);
    instr_decl(xori);
    instr_decl(lui);
    instr_decl(lb);
    instr_decl(lh);
    instr_decl(lw);
    instr_decl(lbu);
    instr_decl(lhu);
    instr_decl(sb);
    instr_decl(sh);
    instr_decl(sw);

    // r-type
    instr_decl(sll);
    instr_decl(srl);
    instr_decl(sra);
    instr_decl(sllv);
    instr_decl(srlv);
    instr_decl(srav);
    instr_decl(jr);
    instr_decl(jalr);
    instr_decl(syscall);
    instr_decl(mfhi);
    instr_decl(mthi);
    instr_decl(mflo);
    instr_decl(mtlo);
    instr_decl(mult);
    instr_decl(multu);
    instr_decl(div);
    instr_decl(divu);
    instr_decl(add);
    instr_decl(addu);
    instr_decl(sub);
    instr_decl(subu);
    instr_decl(and);
    instr_decl(or);
    instr_decl(xor);
    instr_decl(nor);
    instr_decl(slt);
    instr_decl(sltu);
    instr_decl(mfc0);
    instr_decl(mtc0);
    instr_decl(eret);

    // j-type
    instr_decl(j);
    instr_decl(jal);

#undef instr_decl

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
