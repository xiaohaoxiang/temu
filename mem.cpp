#include "mem.h"

template <> uint32_t ram::mem_read<1>(uint32_t addr) const
{
    return uint32_t(M.at(addr >> MASK_SIZE)[addr & MASK]);
}

template <> uint32_t ram::mem_read<2>(uint32_t addr) const
{
    return uint32_t(*reinterpret_cast<const uint16_t *>(&M.at(addr >> MASK_SIZE)[addr & (MASK ^ 0x1U)]));
}

template <> uint32_t ram::mem_read<4>(uint32_t addr) const
{
    return *reinterpret_cast<const uint32_t *>(&M.at(addr >> MASK_SIZE)[addr & (MASK ^ 0x3U)]);
}

template <> void ram::mem_write<1>(uint32_t addr, uint32_t data)
{
    M[addr >> MASK_SIZE][addr & MASK] = uint8_t(data);
}

template <> void ram::mem_write<2>(uint32_t addr, uint32_t data)
{
    *reinterpret_cast<uint16_t *>(&M[addr >> MASK_SIZE][addr & (MASK ^ 0x1U)]) = uint16_t(data);
}

template <> void ram::mem_write<4>(uint32_t addr, uint32_t data)
{
    *reinterpret_cast<uint32_t *>(&M[addr >> MASK_SIZE][addr & (MASK ^ 0x3U)]) = data;
}
