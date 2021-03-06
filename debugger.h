#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "express.h"
#include "mem.h"
#include "processor.h"
#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <limits>
#include <list>
#include <string>
#include <tuple>

class debugger
{
  public:
    debugger(std::string initdata);
    void add_watch(std::string exprstr);
    void remove_watch(std::size_t n);
    void print_watch(std::ostream &os);
    void print_regs(std::ostream &os) const;
    uint32_t express_clac(std::string exprstr);
    void print_memory(std::ostream &os, std::uint32_t n, std::string exprstr);
    void run(std::ostream &os, std::size_t n = std::numeric_limits<std::size_t>::max());

  private:
    std::list<std::tuple<std::string, watch, uint32_t>> watchlist;
    processor cpu;
    ram mem;
};

#endif // !DEBUGGER_H
