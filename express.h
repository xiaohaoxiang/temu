#ifndef EXPRESS_H
#define EXPRESS_H

#include <string>
#include <variant>
#include <vector>
#include "processor.h"

enum unary_op
{
    op_deref, // *
    op_lnot,  // !
    op_flip   // ~
};

enum binocular_op
{
    op_add = op_flip + 1, // +
    op_sub,               // -
    op_mult,              // *
    op_div,               // /
    op_mod,               // %
    op_sl,                // <<
    op_sr,                // >>
    op_eq,                // ==
    op_neq,               // !=
    op_land,              // &&
    op_lor,               // ||
    op_band,              // &
    op_bor                // |
};

class watch;
class express
{
public:
    using value_type = uint32_t;

private:
    friend class watch;
    using valtype_constant = value_type;
    using valtype_regref = regenum;
    struct valtype_unary
    {
        const express &son;
        unary_op op;
        valtype_unary(const express &son, unary_op op) : son(son), op(op) {}
    };
    struct valtype_binocular
    {
        const express &left;
        const express &right;
        binocular_op op;
        valtype_binocular(const express &left, const express &right, binocular_op op) : left(left), right(right), op(op) {}
    };
    using element_type = std::variant<valtype_constant, valtype_regref, valtype_unary, valtype_binocular>;

    express(const element_type &val);

private:
    element_type val;
};

class watch
{
public:
    using value_type = express::value_type;

    watch(const std::string &exprstr);
    value_type get_value(const regfile &regs, const ram &mem) const;

private:
    std::vector<express> buffer;
};

#endif // !EXPRESS_H
