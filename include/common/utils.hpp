#pragma once

#include "./types.hpp"

namespace nes {

/// Set the nth bit in num (to 1)
/// @param num the number being modified
/// @param n   the index of the bit being set
inline void set_bit(u8 &num, int n) {
    num = num | (1 << n);
}

/// Unset the nth bit in num (to 0)
/// @param num the number being modified
/// @param n   the index of the bit being unset
inline void clear_bit(u8 &num, int n) {
    num = num & ~(1 << n);
}

/// flip the nth bit in num
/// @param num the number being modified
/// @param n   the index of the bit being flipped
inline void flip_bit(u8 &num, int n) {
    num = num ^ (1 << n);
}

/// Set the nth bit in num to a given value
/// @param num   the number being modified
/// @param n     the index of the bit being modified
/// @param value the value to which the bit is being set (0 or 1)
inline void set_bit_value(u8 &num, int n, bool value) {
    if (value) {
        set_bit(num, n);
    } else {
        clear_bit(num, n);
    }
}

/// Get the value of the nth bit in the number num
/// @param num the number
/// @param n the index of the bit
inline bool get_bit_value(u8 num, int n) {
    return (num & (1 << n)) != 0;
}

} // namespace nes