#include "express.h"
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

#define check_expr(test)                                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        if (test)                                                                                                      \
        {                                                                                                              \
            return;                                                                                                    \
        }                                                                                                              \
    } while (0)

#define set_priority(op, pr)                                                                                           \
    do                                                                                                                 \
    {                                                                                                                  \
        r[op] = (pr);                                                                                                  \
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
        r[regname[i] + 1] = regenum(i);
    }
    return r;
}();

express::express(const element_type &val) : val(val)
{
}

watch::watch(const std::string &exprstr)
{
    const std::string s = std::regex_replace(exprstr, std::regex("\\s"), "") + '\0';
    std::list<express> elst;
    std::vector<std::tuple<int32_t, int32_t, std::size_t, std::list<express>::iterator>> opvec;
    int32_t bcnt = 0;

    std::size_t i = 0;
    const std::function<void()> parse = [&]() {
        const auto insert = [&](auto op) {
            auto it = elst.insert(elst.end(), element_type(op));
            i++;
            opvec.push_back(std::make_tuple(bcnt, opprior[op], i, it));
        };

        const auto judged_insert = [&](unary_op op_unary, binocular_op op_binocular) {
            if (elst.empty() || std::holds_alternative<valtype_unary>(elst.back().val) ||
                std::holds_alternative<valtype_binocular>(elst.back().val))
            {
                insert(op_unary);
            }
            else
            {
                insert(op_binocular);
            }
        };

        const auto judged_insert2 = [&](binocular_op op1, binocular_op op2, char ch2) {
            if (s[i + 1] == ch2)
            {
                i++;
                insert(op2);
            }
            else
            {
                insert(op1);
            }
        };

        const auto is_idchar = [](const char ch) {
            return ('0' <= ch && ch <= '9') || ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
        };

        switch (s[i])
        {
        case '(': {
            bcnt++;
            check_expr(!elst.empty() && std::holds_alternative<valtype_constant>(elst.back().val) &&
                       std::holds_alternative<valtype_regref>(elst.back().val));
            i++;
            parse();
            break;
        }
        case ')': {
            check_expr(--bcnt < 0);
            i++;
            break;
        }
        case '*': {
            judged_insert(op_deref, op_mult);
            break;
        }
        case '+': {
            judged_insert(op_posit, op_add);
            break;
        }
        case '-': {
            judged_insert(op_negat, op_sub);
            break;
        }
        case '&': {
            judged_insert2(op_band, op_land, '&');
            break;
        }
        case '|': {
            judged_insert2(op_bor, op_lor, '|');
            break;
        }
        case '!': {
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
        case '<': {
            binocular_op op = [&]() {
                switch (s[i + 1])
                {
                case '<': {
                    ++i;
                    return op_sl;
                }
                case '=': {
                    ++i;
                    return op_le;
                }
                default: {
                    return op_lt;
                }
                }
            }();
            insert(op);
            break;
        }
        case '>': {
            binocular_op op = [&]() {
                switch (s[i + 1])
                {
                case '>': {
                    ++i;
                    return op_sr;
                }
                case '=': {
                    ++i;
                    return op_ge;
                }
                default: {
                    return op_gt;
                }
                }
            }();
            insert(op);
            break;
        }
        case '=': {
            i++;
            insert(op_eq);
            break;
        }
        case '/': {
            insert(op_div);
            break;
        }
        case '%': {
            insert(op_mod);
            break;
        }
        case '^': {
            insert(op_xor);
            break;
        }
        case '~': {
            insert(op_flip);
            break;
        }
        case '$': {
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
        default: {
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
    while (s[i])
    {
        parse();
    }

    std::sort(opvec.begin(), opvec.end(),
              [](const std::tuple<int32_t, int32_t, std::size_t, std::list<express>::iterator> &left,
                 const std::tuple<int32_t, int32_t, std::size_t, std::list<express>::iterator> &right) {
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

    for (auto &tp : opvec)
    {
        auto &curit = std::get<std::list<express>::iterator>(tp);
        std::visit(
            [&](auto &arg) {
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
                else if constexpr (std::is_same_v<argtype, valtype_constant>)
                {
                    std::abort();
                }
                else if constexpr (std::is_same_v<argtype, valtype_unary>)
                {
                    std::abort();
                }
            },
            curit->val);
    }
    buffer.splice(buffer.end(), elst, elst.begin());
}

watch::value_type watch::get_value(regfile &regs, ram &mem)
{
    const std::function<value_type(std::list<express>::const_iterator curit)> dfs =
        [&](std::list<express>::const_iterator curit) {
            return std::visit(
                [&](auto &arg) {
                    using argtype = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<argtype, valtype_constant>)
                    {
                        return std::get<valtype_constant>(curit->val);
                    }
                    else if constexpr (std::is_same_v<argtype, valtype_regref>)
                    {
                        return regs.reg[std::get<valtype_regref>(curit->val)]._32;
                    }
                    else if constexpr (std::is_same_v<argtype, valtype_unary>)
                    {
                        auto &cur = std::get<valtype_unary>(curit->val);
                        auto operand = dfs(cur.son);
                        switch (cur.op)
                        {
                        case op_deref: {
                            try
                            {
                                return mem.mem_read<4>(operand);
                            }
                            catch (...)
                            {
                                return 0U;
                            }
                        }
                        case op_posit: {
                            return operand;
                        }
                        case op_negat: {
                            return -operand;
                        }
                        case op_lnot: {
                            return value_type(!operand);
                        }
                        case op_flip: {
                            return ~operand;
                        }
                        }
                    }
                    else if constexpr (std::is_same_v<argtype, valtype_binocular>)
                    {
                        auto &cur = std::get<valtype_binocular>(curit->val);
                        auto [operand1, operand2] = std::make_pair(dfs(cur.left), dfs(cur.right));
                        switch (std::get<valtype_binocular>(curit->val).op)
                        {
                        case op_add: {
                            return operand1 + operand2;
                        }
                        case op_sub: {
                            return operand1 - operand2;
                        }
                        case op_mult: {
                            return operand1 * operand2;
                        }
                        case op_div: {
                            return operand1 / operand2;
                        }
                        case op_mod: {
                            return operand1 % operand2;
                        }
                        case op_xor: {
                            return operand1 ^ operand2;
                        }
                        case op_band: {
                            return operand1 & operand2;
                        }
                        case op_bor: {
                            return operand1 | operand2;
                        }
                        case op_sl: {
                            return operand1 << operand2;
                        }
                        case op_le: {
                            return value_type(operand1 <= operand2);
                        }
                        case op_lt: {
                            return value_type(operand1 < operand2);
                        }
                        case op_sr: {
                            return operand1 >> operand2;
                        }
                        case op_ge: {
                            return value_type(operand1 >= operand2);
                        }
                        case op_gt: {
                            return value_type(operand1 > operand2);
                        }
                        case op_eq: {
                            return value_type(operand1 == operand2);
                        }
                        case op_neq: {
                            return value_type(operand1 != operand2);
                        }
                        case op_land: {
                            return value_type(operand1 && operand2);
                        }
                        case op_lor: {
                            return value_type(operand1 || operand2);
                        }
                        }
                    }
                    return value_type(0);
                },
                curit->val);
        };

    return dfs(std::prev(buffer.end()));
}
