#include "mem.h"

ram::block_type &ram::get_block(uint32_t addr)
{
    return mmap[addr >> mask_size];
}

template <> uint32_t ram::mem_read<1>(uint32_t addr) const
{
    return uint32_t(mmap.at(addr >> mask_size)[addr & mask]);
}

template <> uint32_t ram::mem_read<2>(uint32_t addr) const
{
    return uint32_t(*reinterpret_cast<const uint16_t *>(&mmap.at(addr >> mask_size)[addr & (mask ^ 0x1U)]));
}

template <> uint32_t ram::mem_read<4>(uint32_t addr) const
{
    return *reinterpret_cast<const uint32_t *>(&mmap.at(addr >> mask_size)[addr & (mask ^ 0x3U)]);
}

template <> void ram::mem_write<1>(uint32_t addr, uint32_t data)
{
    mmap[addr >> mask_size][addr & mask] = uint8_t(data);
}

template <> void ram::mem_write<2>(uint32_t addr, uint32_t data)
{
    *reinterpret_cast<uint16_t *>(&mmap[addr >> mask_size][addr & (mask ^ 0x1U)]) = uint16_t(data);
}

template <> void ram::mem_write<4>(uint32_t addr, uint32_t data)
{
    *reinterpret_cast<uint32_t *>(&mmap[addr >> mask_size][addr & (mask ^ 0x3U)]) = data;
}
