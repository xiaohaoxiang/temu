#ifndef EXPRESS_H
#define EXPRESS_H

#include <string>
#include <variant>
#include <list>
#include "processor.h"

enum unary_op
{
    op_deref, // *
    op_posit, // +
    op_negat, // -
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
    op_xor,               // ^
    op_band,              // &
    op_bor,               // |
    op_sl,                // <<
    op_le,                // <=
    op_lt,                // <
    op_sr,                // >>
    op_ge,                // >=
    op_gt,                // >
    op_eq,                // ==
    op_neq,               // !=
    op_land,              // &&
    op_lor                // ||
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
        std::list<express>::const_iterator son;
        unary_op op;
        valtype_unary(unary_op op, std::list<express>::const_iterator son = std::list<express>::const_iterator{nullptr}) : son(son), op(op) {}
    };
    struct valtype_binocular
    {
        std::list<express>::const_iterator left;
        std::list<express>::const_iterator right;
        binocular_op op;
        valtype_binocular(binocular_op op, std::list<express>::const_iterator left = std::list<express>::const_iterator{nullptr},
                          std::list<express>::const_iterator right = std::list<express>::const_iterator{nullptr}) : left(left), right(right), op(op) {}
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

private:
    using valtype_constant = express::valtype_constant;
    using valtype_regref = express::valtype_regref;
    using valtype_unary = express::valtype_unary;
    using valtype_binocular = express::valtype_binocular;
    using element_type = express::element_type;

public:
    watch(const std::string &exprstr);
    value_type get_value(const regfile &regs, const ram &mem) const;
    bool valid();

private:
    std::list<express> buffer;
};

#endif // !EXPRESS_H
