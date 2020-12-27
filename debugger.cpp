#include "debugger.h"
#include <algorithm>
#include <iomanip>
#include <ostream>

debugger::debugger(std::string initdata) : cpu(mem)
{
    for (std::size_t i = 0; i < initdata.size(); i += ram::block_size)
    {
        auto &blo = mem.get_block(uint32_t(i));
        std::copy_n(initdata.begin() + i, ram::block_size, blo.begin());
    }
}

void debugger::add_watch(std::string exprstr)
{
    watchlist.emplace_back(exprstr, exprstr, 0U);
    std::get<uint32_t>(watchlist.back()) = std::get<watch>(watchlist.back()).get_value(cpu.get_regs(), mem);
}

void debugger::remove_watch(std::size_t n)
{
    if (n >= watchlist.size())
        return;
    auto it = watchlist.begin();
    for (std::size_t i = 1; i < n; i++)
    {
        ++it;
    }
    watchlist.erase(it);
}

void debugger::print_watch(std::ostream &os)
{
    std::size_t i = 0;
    for (auto &w : watchlist)
    {
        os << i++ << ". " << std::get<std::string>(w) << ' ' << std::get<watch>(w).get_value(cpu.get_regs(), mem)
           << std::endl;
    }
}

void debugger::print_regs(std::ostream &os) const
{
    auto &r = cpu.get_regs();
    os << std::left << std::setfill(' ');
    for (uint32_t i = 0; i < 32U; i++)
    {
        os << std::dec << std::setw(3) << i << std::setw(6) << regname[i] << std::hex << r.reg[i]._32 << std::endl;
    }
    os << std::setw(9) << "pc" << r.pc << std::endl;
    os << std::setw(9) << "hi" << r.hi << std::endl;
    os << std::setw(9) << "lo" << r.lo << std::endl;
    os << std::setw(9) << "badvaddr" << r.badvaddr << std::endl;
    os << std::setw(9) << "status" << r.status.val << std::endl;
    os << std::setw(9) << "cause" << r.status.val << std::endl;
    os << std::setw(9) << "epc" << r.epc << std::endl;
}

uint32_t debugger::express_clac(std::string exprstr)
{
    return watch(exprstr).get_value(cpu.get_regs(), mem);
}

void debugger::print_memory(std::ostream &os, std::uint32_t n, std::string exprstr)
{
    os << std::left << std::hex << std::setfill('0');
    uint32_t addr = express_clac(exprstr);
    for (uint32_t i = 0; i < n; i++, addr += 4U)
    {
        os << std::setw(9) << addr << std::setw(8) << mem.mem_read<4>(addr) << std::endl;
    }
}

void debugger::run(std::ostream &os, std::size_t n)
{
    while (n--)
    {
        cpu.exec(mem.mem_read<4>(cpu.get_regs().pc));
        std::size_t i = 0;
        for (auto &w : watchlist)
        {
            uint32_t val = std::get<watch>(w).get_value(cpu.get_regs(), mem);
            if (val != std::get<uint32_t>(w))
            {
                os << std::dec << i++ << ". " << std::get<std::string>(w) << ' ' << std::get<uint32_t>(w) << " -> "
                   << val << std::endl;
                std::get<uint32_t>(w) = val;
                n = 0;
            }
        }
    }
}
