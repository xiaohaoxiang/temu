#include <iostream>
#include <memory>
#include <readline/history.h>
#include <readline/readline.h>
#include "cpu.h"
#include "mem.h"
using namespace std;

struct cfree
{
    template <typename T>
    void operator()(T p)
    {
        return free((void *)(p));
    }
};

int main()
{
    for (;;)
    {
        unique_ptr<char[], cfree> p(readline("temu$ "));
        cout << p.get() << endl;
    }
    return 0;
}
