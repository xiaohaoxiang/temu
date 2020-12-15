#include <iostream>
#include <memory>
#include <readline/history.h>
#include <readline/readline.h>
#include "cpu.h"
#include "mem.h"
using namespace std;

int main(int argc, char const *argv[])
{
    cpu::instruction x;
    x.val = 0;
    for (int i = 0; i < 70000; i++)
    {
        cout << i << ' ' << (int32_t(int16_t(x.i.imm)) << 2) << endl;
        x.i.imm++;
    }

    return 0;
}
