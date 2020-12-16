#include <iostream>
#include <memory>
#include <readline/history.h>
#include <readline/readline.h>
#include "cpu.h"
#include "mem.h"
using namespace std;

struct A
{
    int : 2;
    uint32_t exccode : 5;
    int : 1;
    uint32_t ip : 8;
    int : 15;
    bool bd : 1;
};

int main()
{
    A x;
    cout << sizeof(regfile)<< endl;
    cpu::instruction y;
    sizeof(y);
    return 0;
}
