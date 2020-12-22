#ifndef MEM_H
#define MEM_H

#include <array>
#include <cstdint>
#include <unordered_map>

class ram
{
  public:
    static constexpr uint32_t mask_size = 12;
    static constexpr uint32_t mask = (1U << mask_size) - 1;
    static constexpr uint32_t block_size = 1UL << mask_size;

    using block_type = std::array<uint8_t, block_size>;
    using map_type = std::unordered_map<uint32_t, block_type>;

    template <uint32_t len> uint32_t mem_read(uint32_t addr) const
    {
        static_assert(len == 1 || len == 2 || len == 4, "len == 1 || len == 2 || len == 4");
        return 0;
    }

    template <uint32_t len> void mem_write(uint32_t addr, uint32_t data)
    {
        static_assert(len == 1 || len == 2 || len == 4, "len == 1 || len == 2 || len == 4");
    }

    block_type &get_block(uint32_t addr);

  private:
    map_type mmap;
};

template <> uint32_t ram::mem_read<1>(uint32_t addr) const;
template <> uint32_t ram::mem_read<2>(uint32_t addr) const;
template <> uint32_t ram::mem_read<4>(uint32_t addr) const;

template <> void ram::mem_write<1>(uint32_t addr, uint32_t data);
template <> void ram::mem_write<2>(uint32_t addr, uint32_t data);
template <> void ram::mem_write<4>(uint32_t addr, uint32_t data);

#endif // !MEM_H
