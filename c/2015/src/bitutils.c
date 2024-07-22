#include "bitutils.h"

uint32_t rotate_left(uint32_t input, uint32_t n) {
    return (input << n) | (input >> (32 - n));
}

uint32_t swap_bytes(uint32_t value) {
    return  ((value >> 24) & 0x000000FF) |
            ((value >> 8)  & 0x0000FF00) |
            ((value << 8)  & 0x00FF0000) |
            ((value << 24) & 0xFF000000);
}