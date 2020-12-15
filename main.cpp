#include <iostream>
#include <memory>
#include <readline/history.h>
#include <readline/readline.h>
#include "cpu.h"
#include "mem.h"
using namespace std;

struct A
{
    uint32_t lo : 16;
    uint32_t hi : 16;
};

int main()
{
    A a{};
    a.lo = 0xFFFF;
    auto b = a.lo << 16;
    cout << b << endl;
    auto c = uint16_t(0xFF) >> 16;
    auto d = uint8_t(0xFF) >> 1;
    decltype(a.lo) t;
    return 0;
}
