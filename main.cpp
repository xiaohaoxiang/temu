#include <iostream>
#include <memory>
#include <readline/history.h>
#include <readline/readline.h>
#include "cpu.h"
#include "mem.h"
using namespace std;

int main()
{
    regfile::singlereg x;
    x._32 = -1654963;
    cout << (int32_t(x._32) >> 1) << endl;
    return 0;
}
