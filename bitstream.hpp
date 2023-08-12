#pragma once
#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>

class BitWriter {
    inline void put_bits(int bit_n, uint32_t value) {
        // Проверка на корректность
        if (bit_n > 32 || bit_n <= 0) return;

        auto current = static_cast<int>(current_bit_count & 31);
        auto save_current_bit_count = current_bit_count + bit_n;

        // Цикл записывает данные от старшего к младшему
        for (int i = bit_n - 1; i >= 0; --i) {
            if (current == 0) buffer = 0;
            // Записываем бит в текущий буфер
            buffer |= ((value >> i) & 1) << current;

            // Увеличиваем счетчик битов
            current++;

            if (current == 32) {
                current = 0;
                current_bit_count = (current_bit_count & ~31) + 32;
                flush();
            }
        }
        current_bit_count = (current_bit_count & ~31) + current;
    }
    public:
    inline void writeBits(int n, uint32_t value ) { put_bits(n, value); }
    inline void writeBit(bool value) { put_bits(1,value);}
    inline void writeBit0() { writeBit(true);}
    inline void writeBit1() { writeBit(false);}
    auto data() const { return vec.data(); }
    auto size() const { return (current_bit_count + 7) >> 3; }
    auto get_size_in_bits() const { return current_bit_count; }
    void flush() {
        // std::cout << current_bit_count << " / " << data.size() << std::endl;

        vec.resize((current_bit_count + 31 & ~31) / 8);

        // std::cout << current_bit_count << " / " << data.size() << std::endl;

        // for (int i = 3; i >= 0; --i) {
        //     data.push_back(buffer >> (i * 8));
        // }
        auto buffer_begin = reinterpret_cast<uint8_t *>(&buffer);
        std::copy(buffer_begin, buffer_begin + 4, vec.end() - 4);
        // buffer = 0;
        // current_bit_count = 0;
    }

    // Метод для вывода данных (для проверки)
    void printBits(std::ostream& o) const {
        auto count = size();
        for (auto byte : vec) {
            if (cout-- ==  0)
                    return;
            for (int i = 7; i >= 0; --i) {
                o << ((byte >> i) & 1);
            }
            o << " ";
        }
        //o << "\n";
    }

   private:
    std::vector<uint8_t> vec;
    uint32_t buffer = 0;  // буфер для хранения битов
    uint64_t current_bit_count = 0;  // счетчик битов в буфере
};

class BitReader {
 

    inline unsigned int get_bits1() {
        size_t _index = index;
        if (_index >= size_in_bits) {
            // printf("overflow!");
            return -1;
        }
        uint8_t result = buffer[_index >> 3];
        result <<= _index & 7;
        result >>= 8 - 1;
        _index++;
        index = _index;
        return result;
    }

    /**
     * Read 1-25 bits.
     */
    inline unsigned int get_bits(int n) {
        // assert(n>0 && n<=25);
        if (n == 0) return 0;
        union unaligned_32 {
            uint32_t l;
        } __attribute__((packed)) __attribute__((may_alias));
        unsigned int tmp;
        unsigned int re_index = index;
        unsigned int re_cache;
        re_cache =
            __builtin_bswap32(
                ((const union unaligned_32 *)(buffer + (re_index >> 3)))->l)
            << (re_index & 7);
        tmp = (((uint32_t)(re_cache)) >> (32 - n));
        re_index += n;
        index = re_index;
        return tmp;
    }

    /**
     * Read 0-32 bits.
     */

    inline unsigned int get_bits_long(int n) {
        // assert(n>=0 && n<=32);
        if (!n) {
            return 0;
        if (n==1)
            return get_bits1();

        } else if (n <= 25) {
            return get_bits(n);
        } else {
            unsigned ret = get_bits(16) << (n - 16);
            return ret | get_bits(n - 16);
        }
    }

  public:
    BitReader(const uint8_t *buffer_, size_t buffer_size) {
        buffer_begin = buffer = buffer_;
        size_in_bits = buffer_size << 3;
        buffer_end = buffer_ + buffer_size;
        index = 0;
    }
    inline bool readBit() {
        return get_bits1();
    }
    inline auto readBits(int n) {
        return get_bits_long(n);
    }
    // Метод для вывода данных (для проверки)
    void printBits(std::ostream & o) const {
        for (auto it = buffer_begin; it != buffer_end; ++it) {
            auto byte = *it;
            for (int i = 7; i >= 0; --i) {
                o << ((byte >> i) & 1);
            }
            o << " ";
        }
       // o << "\n";
    }

    size_t get_index() { return index; }
    size_t get_size_in_bits() { return size_in_bits; }
    size_t bit_left() { return size_in_bits - index; }

   private:
    const uint8_t *buffer_begin;
    const uint8_t *buffer;
    const uint8_t *buffer_end;
    size_t index;
    size_t size_in_bits;
};

inline std::ostream& operator<<(std::ostream& o, BitWriter const& a) {
    a.printBits(o);
    return o;
}

inline std::ostream& operator<<(std::ostream& o, BitReader const& a) {
    a.printBits(o);
    return o;
}


