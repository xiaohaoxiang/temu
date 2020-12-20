#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <list>
#include <regex>
#include <utility>
#include <vector>
#include "express.h"

#define check_expr(test)  \
    do                    \
    {                     \
        if (test)         \
        {                 \
            elst.clear(); \
            return;       \
        }                 \
    } while (0)

watch::watch(const std::string &exprstr)
{
    const std::string s = std::regex_replace(exprstr, std::regex("\\s"), "") + '\0';

    std::vector<std::pair<long, std::list<express>::iterator>> opvec;
    long bcnt = 0;

    for (std::size_t i = 0;;)
    {
        static const std::function<void()> parse = [&]() {
            static const std::function<void(unary_op, binocular_op)> judged_insert = [&](unary_op op_unary, binocular_op op_binocular) {
                auto it = elst.insert(elst.end(), element_type{});
                ++i;
                parse();
                auto nit = std::next(it);
                check_expr(nit == elst.end());
                if (elst.empty() || std::holds_alternative<valtype_unary>(elst.back().val) || std::holds_alternative<valtype_binocular>(elst.back().val))
                {
                    it->val.emplace<valtype_unary>(*nit, op_unary);
                }
                else
                {
                    it->val.emplace<valtype_binocular>(*std::prev(it), *nit, op_binocular);
                }
                opvec.push_back(std::make_pair(bcnt, it));
            };

            static const std::function<void(binocular_op, binocular_op, char)> judged_insert2 = [&](binocular_op op1, binocular_op op2, char ch2) {
                binocular_op op = [&]() {
                    if (s[i + 1] == ch2)
                    {
                        return op1;
                    }
                    else
                    {
                        return op2;
                    }
                }();
                auto it = elst.insert(elst.end(), element_type{});
                ++i;
                parse();
                auto nit = std::next(it);
                check_expr(nit == elst.end());
                it->val.emplace<valtype_binocular>(*std::prev(it), *nit, op);
                opvec.push_back(std::make_pair(bcnt, it));
            };

            switch (s[i])
            {
            case '(':
            {
                ++bcnt;
                check_expr(!elst.empty() && std::holds_alternative<valtype_constant>(elst.back().val) && std::holds_alternative<valtype_regref>(elst.back().val));
                ++i;
                parse();
                break;
            }
            case ')':
            {
                check_expr(--bcnt < 0);
                break;
            }
            case '*':
            {
                judged_insert(op_deref, op_mult);
                break;
            }
            case '+':
            {
                judged_insert(op_posit, op_add);
                break;
            }
            case '-':
            {
                judged_insert(op_negat, op_sub);
                break;
            }
            case '&':
            {

                break;
            }
            case '|':
            {
                break;
            }
            case '!':
            {
                break;
            }
            case '<':
            {
                break;
            }
            case '>':
            {
                break;
            }
            case '/':
            {
                break;
            }
            case '%':
            {
                break;
            }
            case '~':
            {
                break;
            }
            case '$':
            {
                break;
            }
            default:
            {
                break;
            }
            }
        };
    }
}
