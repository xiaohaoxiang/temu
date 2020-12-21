#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <list>
#include <regex>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include "express.h"

#define check_expr(test) \
    do                   \
    {                    \
        if (test)        \
        {                \
            return;      \
        }                \
    } while (0)

#define set_priority(op, pr) \
    do                       \
    {                        \
        r[op] = (pr);        \
    } while (0)

static const std::array<int32_t, op_lor + 1> opprior = []() {
    std::array<int32_t, op_lor + 1> r;

    set_priority(op_deref, 1);
    set_priority(op_posit, 1);
    set_priority(op_negat, 1);
    set_priority(op_lnot, 1);
    set_priority(op_flip, 1);

    set_priority(op_mult, 2);
    set_priority(op_div, 2);
    set_priority(op_mod, 2);

    set_priority(op_add, 3);
    set_priority(op_sub, 3);

    set_priority(op_sl, 4);
    set_priority(op_sr, 4);

    set_priority(op_lt, 5);
    set_priority(op_le, 5);
    set_priority(op_gt, 5);
    set_priority(op_ge, 5);

    set_priority(op_eq, 6);
    set_priority(op_neq, 6);

    set_priority(op_band, 7);

    set_priority(op_xor, 8);

    set_priority(op_bor, 9);

    set_priority(op_land, 10);

    set_priority(op_lor, 11);

    return r;
}();

#undef set_priority

static const std::unordered_map<std::string, regenum> regmap = []() {
    std::unordered_map<std::string, regenum> r;
    for (uint32_t i = 0U; i < 32U; i++)
    {
        r[std::to_string(i)] = regenum(i);
        r[regname[i]] = regenum(i);
    }
    return r;
}();

watch::watch(const std::string &exprstr)
{
    const std::string s = std::regex_replace(exprstr, std::regex("\\s"), "") + '\0';
    std::list<express> elst;
    std::vector<std::tuple<int32_t, int32_t, std::size_t, std::list<express>::iterator>> opvec;
    int32_t bcnt = 0;

    for (std::size_t i = 0; s[i];)
    {
        static const std::function<void()> parse = [&]() {
            static const auto insert = [&](auto op) {
                auto it = elst.insert(elst.end(), element_type(op));
                ++i;
                parse();
                auto nit = std::next(it);
                check_expr(nit == elst.end());
                opvec.push_back(std::make_tuple(bcnt, opprior[op], i, it));
            };

            static const auto judged_insert = [&](unary_op op_unary, binocular_op op_binocular) {
                if (elst.empty() || std::holds_alternative<valtype_unary>(elst.back().val) || std::holds_alternative<valtype_binocular>(elst.back().val))
                {
                    insert(op_unary);
                }
                else
                {
                    insert(op_binocular);
                }
            };

            static const auto judged_insert2 = [&](binocular_op op1, binocular_op op2, char ch2) {
                return insert(s[++i] == ch2 ? op2 : op1);
            };

            static const auto is_idchar = [](const char ch) {
                return ('0' <= ch && ch <= '9') || ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
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
                judged_insert2(op_band, op_land, '&');
                break;
            }
            case '|':
            {
                judged_insert2(op_bor, op_lor, '|');
                break;
            }
            case '!':
            {
                if (s[i + 1] == '=')
                {
                    ++i;
                    insert(op_neq);
                }
                else
                {
                    insert(op_lnot);
                }
                break;
            }
            case '<':
            {
                binocular_op op = [&]() {
                    switch (s[i + 1])
                    {
                    case '<':
                    {
                        ++i;
                        return op_sl;
                    }
                    case '=':
                    {
                        ++i;
                        return op_le;
                    }
                    default:
                    {
                        return op_lt;
                    }
                    }
                }();
                insert(op);
                break;
            }
            case '>':
            {
                binocular_op op = [&]() {
                    switch (s[i + 1])
                    {
                    case '>':
                    {
                        ++i;
                        return op_sr;
                    }
                    case '=':
                    {
                        ++i;
                        return op_ge;
                    }
                    default:
                    {
                        return op_gt;
                    }
                    }
                }();
                insert(op);
                break;
            }
            case '/':
            {
                insert(op_div);
                break;
            }
            case '%':
            {
                insert(op_mod);
                break;
            }
            case '^':
            {
                insert(op_xor);
                break;
            }
            case '~':
            {
                insert(op_flip);
                break;
            }
            case '$':
            {
                ++i;
                auto j = std::find_if_not(s.begin() + i, s.end(), is_idchar) - s.begin();
                try
                {
                    elst.push_back(express(element_type(regmap.at(s.substr(i, j - i)))));
                }
                catch (...)
                {
                    return;
                }

                i = j;
                break;
            }
            default:
            {
                auto j = std::find_if_not(s.begin() + i, s.end(), is_idchar) - s.begin();
                try
                {
                    elst.push_back(express(element_type(uint32_t(std::stoul(s.substr(i, j - i), nullptr, 0)))));
                }
                catch (...)
                {
                    return;
                }
                i = j;
                break;
            }
            }
        };
        parse();
    }

    std::sort(opvec.begin(), opvec.end(), [](const std::tuple<int32_t, int32_t, std::size_t, std::list<express>::iterator> &left, const std::tuple<int32_t, int32_t, std::size_t, std::list<express>::iterator> &right) {
        if (std::get<0>(left) != std::get<0>(right))
        {
            return std::get<0>(left) > std::get<0>(right);
        }
        else if (std::get<1>(left) != std::get<1>(right))
        {
            return std::get<1>(left) < std::get<1>(right);
        }
        else if (std::get<1>(left) == 1)
        {
            return std::get<2>(left) > std::get<2>(right);
        }
        else
        {
            return std::get<2>(left) < std::get<2>(right);
        }
    });

    for (const auto &tp : opvec)
    {
        auto &curit = std::get<std::list<express>::iterator>(tp);
        std::visit([&](auto &arg) {
            using argtype = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<argtype, valtype_unary>)
            {
                auto nit = std::next(curit);
                std::get<valtype_unary>(curit->val).son = nit;
                buffer.splice(buffer.end(), elst, nit);
            }
            else if constexpr (std::is_same_v<argtype, valtype_binocular>)
            {
                auto [lit, rit] = std::make_pair(std::prev(curit), std::next(curit));
                auto &cur = std::get<valtype_binocular>(curit->val);
                std::tie(cur.left, cur.right) = std::tie(lit, rit);
                buffer.splice(buffer.end(), elst, lit);
                buffer.splice(buffer.end(), elst, rit);
            }
            else
            {
                // never reach
            }
        },
                   curit->val);
    }
}
