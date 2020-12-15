#ifndef DEFS_H
#define DEFS_H

#define signedext(x, from, to) int##to##_t(int##from##_t(uint##from##_t(x)))
#define unsignedext(x, from, to) uint##to##_t(uint##from##_t(x))

#endif // !DEFS_H
