#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <cstdint>
#include "mem.h"

extern const char *const regname[];

enum regenum : uint32_t
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

enum cp0regnum : uint32_t
{
    $index,
    $random,
    $entrylo0,
    $entrylo1,
    $context,
    $pagemask,
    $wired,
    $reserved0,
    $badvaddr,
    $count,
    $entryhi,
    $compare,
    $status,
    $cause,
    $epc,
    $prid,
    $config,
    $lladdr,
    $watchlo,
    $watchhi,
    $reserved1,
    $reserved2,
    $reserved3,
    $debug,
    $depc,
    $reserved4,
    $errctl,
    $reserved5,
    $tagdatalo,
    $reserved6,
    $errorepc,
    $desave
};

enum : uint32_t
{
    exc_int = 0x00,
    exc_adel = 0x04,
    exc_ades = 0x05,
    exc_sys = 0x08,
    exc_ri = 0x0A,
    exc_ov = 0x0C
};

enum : uint32_t
{
    intaddr = 0x040,
    excaddr = 0x100
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

    uint32_t badvaddr;
    union
    {
        struct
        {
            bool ie : 1;
            bool exl : 1;
            int : 6;
            uint32_t im : 8;
            int : 16;

        } field;
        uint32_t val;
    } status;
    union
    {
        struct
        {
            int : 2;
            uint32_t exccode : 5;
            int : 1;
            uint32_t ip : 8;
            int : 15;
            bool bd : 1;
        } field;
        uint32_t val;
    } cause;
    uint32_t epc;
};

class processor
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

    void exception(uint32_t exccode);
    void jump(uint32_t target);

public:
    processor(ram &mem, bool delayed_branches = true);
    regfile &get_regs();
    const regfile &get_regs() const;
    ram &get_mem();
    const ram &get_mem() const;

    void exec(instruction instr);

private:
    regfile regs;
    ram &mem;
    bool in_delay_slot;
    bool exception_occurred;

public:
    const bool delayed_branches;
};

#endif // !PROCESSOR_H
