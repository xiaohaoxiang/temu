#include "mem.h"

template <>
uint32_t ram::mem_read<1>(uint32_t addr)const
{
    return uint32_t(M[addr >> MASK_SIZE][addr & MASK]);
}

template <>
uint32_t ram::mem_read<2>(uint32_t addr)const
{
    return uint32_t(*reinterpret_cast<uint16_t *>(&M[addr >> MASK_SIZE][addr & (MASK ^ 0x1U)]));
}

template <>
uint32_t ram::mem_read<4>(uint32_t addr)const
{
    return *reinterpret_cast<uint32_t *>(&M[addr >> MASK_SIZE][addr & (MASK ^ 0x3U)]);
}

template <>
void ram::mem_write<1>(uint32_t addr, uint32_t data)
{
    M[addr >> MASK_SIZE][addr & MASK] = uint8_t(data);
}

template <>
void ram::mem_write<2>(uint32_t addr, uint32_t data)
{
    *reinterpret_cast<uint16_t *>(&M[addr >> MASK_SIZE][addr & (MASK ^ 0x1U)]) = uint16_t(data);
}

template <>
void ram::mem_write<4>(uint32_t addr, uint32_t data)
{
    *reinterpret_cast<uint32_t *>(&M[addr >> MASK_SIZE][addr & (MASK ^ 0x3U)]) = data;
}

/*
template <>
uint32_t ram::mem_read<1>(uint32_t addr)
{
    return uint32_t(M[addr >> MASK_SIZE][addr & MASK]);
}

template <>
uint32_t ram::mem_read<2>(uint32_t addr)
{
    const auto &va = M[addr >> MASK_SIZE];
    uint32_t r = uint32_t(va[addr & MASK]) << 8;
    if ((++addr) & MASK)
    {
        r |= uint32_t(va[addr & MASK]);
    }
    else
    {
        r |= uint32_t(M[addr >> MASK_SIZE][addr & MASK]);
    }
    return r;
}

template <>
uint32_t ram::mem_read<4>(uint32_t addr)
{
    auto &m = M;
    auto get_it = [&]() {
        map_type::iterator it = m.find(addr >> MASK_SIZE);
        if (it == m.end())
        {
            it = m.emplace(addr >> MASK_SIZE, block_type()).first;
        }
        return it;
    };
    map_type::iterator it = get_it();
    uint32_t r = it->second[addr & MASK] << 24;
    for (int i = 2; i >= 0; i--)
    {
        if (!((++addr) & MASK))
        {
            it = get_it();
        }
        r |= it->second[addr & MASK] << (i << 3);
    }
    return r;
}

template <>
void ram::mem_write<1>(uint32_t addr, uint32_t data)
{
    M[addr >> MASK_SIZE][addr & MASK] = uint8_t(data);
}

template <>
void ram::mem_write<2>(uint32_t addr, uint32_t data)
{
    auto &va = M[addr >> MASK_SIZE];
    va[addr & MASK] = uint8_t(data >> 8);
    if ((++addr) & MASK)
    {
        va[addr & MASK] = uint8_t(data);
    }
    else
    {
        M[addr >> MASK_SIZE][addr & MASK] = uint8_t(data);
    }
}

template <>
void ram::mem_write<4>(uint32_t addr, uint32_t data)
{
    auto &m = M;
    auto get_it = [&]() {
        map_type::iterator it = m.find(addr >> MASK_SIZE);
        if (it == m.end())
        {
            it = m.emplace(addr >> MASK_SIZE, block_type()).first;
        }
        return it;
    };
    map_type::iterator it = get_it();
    it->second[addr & MASK] = uint8_t(data >> 24);
    for (int i = 2; i >= 0; i--)
    {
        if (!((++addr) & MASK))
        {
            it = get_it();
        }
        it->second[addr & MASK] = uint8_t(data >> (i << 3));
    }
}
*/
