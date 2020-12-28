#include "debugger.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

#if __has_include(<readline/history.h>) && __has_include(<readline/readline.h>)

#define has_readline 1
#include <memory>
#include <readline/history.h>
#include <readline/readline.h>

#endif

static const std::string help_info = "help     - print this help info.\n"
                                     "c        - begin or continue to run.\n"
                                     "q        - quit temu.\n"
                                     "si [n]   - run n(default=1) step.\n"
                                     "info r   - print information of registers.\n"
                                     "info w   - print information of watch points.\n"
                                     "p expr   - calculate the result of expression expr\n"
                                     "x n expr - print n word(s) at the address calculated from the expression expr\n"
                                     "w expr   - add a watch point of expr\n"
                                     "d n      - delete the nth watch point\n";

int main(int argc, char const *argv[])
{
    static const char prompt[] = "(temu)";
    std::string initdata;
    try
    {
        if (argc < 2)
        {
            throw 0;
        }
        std::ifstream instrm(argv[1]);
        initdata.assign(std::istreambuf_iterator<char>(instrm), std::istreambuf_iterator<char>());
    }
    catch (...)
    {
        std::cout << help_info << std::endl;
        return 0;
    }
    debugger dbg(initdata);

    for (;;)
    {
        std::string cmdstr = []() {
#ifdef has_readline
            std::unique_ptr<char[]> pstr(readline(prompt));
            add_history(pstr.get());
            return std::string(pstr.get());
#else
            std::cout << prompt;
            std::string tmp;
            std::getline(std::cin, tmp);
            return tmp;
#endif // has_readline
        }();
        [](std::string &s, char ch = ' ') {
            std::string::size_type p = 0;
            for (auto it = s.begin(); it != s.end(); ++it)
            {
                if (*it != ch)
                {
                    if (p && *std::prev(it) == ch)
                    {
                        s[p++] = ch;
                    }
                    s[p++] = *it;
                }
            }
            s.resize(p);
        }(cmdstr);
        std::istringstream instrm(cmdstr);
        std::string tmp;
        instrm >> tmp;
        if (tmp == "help")
        {
            std::cout << help_info << std::endl;
        }
        else if (tmp == "c")
        {
            dbg.run(std::cout);
        }
        else if (tmp == "q")
        {
            break;
        }
        else if (tmp == "si")
        {
            instrm >> tmp;
            std::size_t n;
            try
            {
                n = std::stoul(tmp, nullptr, 0);
            }
            catch (...)
            {
                n = 1;
            }
            dbg.run(std::cout, n);
        }
        else if (tmp == "info")
        {
            instrm >> tmp;
            if (tmp == "r")
            {
                dbg.print_regs(std::cout);
            }
            else if (tmp == "w")
            {
                dbg.print_watch(std::cout);
            }
        }
        else if (tmp == "p")
        {
            if (instrm.good())
            {
                std::cout << dbg.express_clac(instrm.str().substr(instrm.tellg())) << std::endl;
            }
        }
        else if (tmp == "x")
        {
            std::size_t n;
            try
            {
                instrm >> n;
                dbg.print_memory(std::cout, n, instrm.str().substr(instrm.tellg()));
            }
            catch (...)
            {
            }
        }
        else if (tmp == "w")
        {
            dbg.add_watch(instrm.str().substr(instrm.tellg()));
        }
        else if (tmp == "d")
        {
            std::size_t n;
            try
            {
                instrm >> n;
                dbg.remove_watch(n);
            }
            catch (...)
            {
            }
        }
    }
    return 0;
}
