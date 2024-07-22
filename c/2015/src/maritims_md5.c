#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "maritims_md5.h"

static uint32_t rotate_left(uint32_t input, uint32_t n) {
    return (input << n) | (input >> (32 - n));
}

static uint32_t get_table_value(uint32_t i)
{
    return (uint32_t)(pow(2, 32) * fabs(sin(i + 1)));
}

static uint32_t F(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) | (~x & z);
}

static uint32_t G(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & z) | (y & ~z);
}

static uint32_t H(uint32_t x, uint32_t y, uint32_t z)
{
    return x ^ y ^ z;
}

static uint32_t I(uint32_t x, uint32_t y, uint32_t z)
{
    return y ^ (x | ~z);
}

static const uint8_t message_indices[64] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12,
    5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2,
    0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9};

static const uint8_t S[4][4] = {
    {7, 12, 17, 22},
    {5, 9, 14, 20},
    {4, 11, 16, 23},
    {6, 10, 15, 21}};

static uint32_t (*FGHI[4])(uint32_t, uint32_t, uint32_t) = {F, G, H, I};

/*
Append Padding Bits

The message is "padded" (extended) so that its length (in bits) is
congruent to 448, modulo 512. That is, the message is extended so
that it is just 64 bits shy of being a multiple of 512 bits long.
Padding is always performed, even if the length of the message is
already congruent to 448, modulo 512.

Padding is performed as follows: a single "1" bit is appended to the
message, and then "0" bits are appended so that the length in bits of
the padded message becomes congruent to 448, modulo 512. In all, at
least one bit and at most 512 bits are appended.
*/
static uint8_t *pad_input(const char *input, size_t *out_length)
{
    size_t original_length = strlen(input);
    size_t bit_length = original_length * 8;
    size_t padding_length = ((448 - (bit_length + 1) % 512) + 512) % 512;
    size_t total_length = bit_length + 1 + padding_length + 64;
    size_t padded_length = total_length / 8;

    uint8_t *padded_input = (uint8_t *)calloc(padded_length, sizeof(uint8_t));
    memcpy(padded_input, input, original_length);
    padded_input[original_length] = 0x80;

    for (size_t i = 0; i < 8; i++)
    {
        padded_input[padded_length - 8 + i] = (uint8_t)((bit_length >> (i * 8)) & 0xFF);
    }

    *out_length = padded_length;
    return padded_input;
}

// Encodes input by splitting a 32-bit Word into bytes.
static void encode(uint8_t *output, const uint32_t *input, uint32_t length)
{
    for (uint32_t i = 0, j = 0; j < length; i++, j += 4)
    {
        output[j] = (uint8_t)(input[i] & 0xFF);
        output[j + 1] = (uint8_t)((input[i] >> 8) & 0xFF);
        output[j + 2] = (uint8_t)((input[i] >> 16) & 0xFF);
        output[j + 3] = (uint8_t)((input[i] >> 24) & 0xFF);
    }
}

// Decodes input by combining four bytes into a 32-bit Word. The length parameter must be divisible by 4.
static void decode(uint32_t *output, const uint8_t *input, uint32_t length)
{
    for (uint32_t i = 0, j = 0; j < length; i++, j += 4)
    {
        output[i] = ((uint32_t)input[j]) |
                    (((uint32_t)input[j + 1]) << 8) |
                    (((uint32_t)input[j + 2]) << 16) |
                    (((uint32_t)input[j + 3]) << 24);
    }
}

static void transform(uint32_t *state, const uint8_t *block)
{
    // Store the values of the registers from before the transformation to avoid accessing the state array directly.
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3], messages[16];
    decode(messages, block, 64);

    /*
    In each round sixteen operations are performed where the formula "a = b + ((a + F(b, c, d) + X[k] + T[i]) <<< s)" is applied.

    These operations can be split in batches of four which use the reigsters in the following orders: ABCD, DABC, CDAB, BCDA.
    To achieve this we have to rotate the registers after every operation in the following order:
    The value of the D register is stored in a temporary variable.
    The value of the C register is assigned to the D register.
    The value of the B register is assigned to the C register.
    The value of the A register is assigned to the B register.
    The value of the D register is assigned to the A register by retrieving the original value of the D register from the temporary varaible.
    This can be expressed as temp = D, D = C, C = B, B = A, A = temp.

    In the RFC every ABCD/DABC/CDAB/BCDA operation takes three arguments: k, s and i.
    The variable k represents the message index, the variable s represents how many left rotations to perform and the variable i represents the index to look up in the precomputed table.
    */
    for (uint8_t i = 0; i < 64; i++)
    {
        // Helper variable for doing lookups in the S array to find number of left rotations.
        uint8_t round = i / 16;
        
        // Our representation of k from the formula.
        uint8_t message_index = message_indices[i];

        // Our representation of X[k] from the formula.
        uint32_t message = messages[message_index];

        // Our representation of T[i] from the formula.
        uint32_t table_value = get_table_value(i);

        // Our representation of s from the formula.
        uint8_t number_of_rotations = S[round][i % 4];

        // Applying the formula.
        a += FGHI[round](b, c, d) + message + table_value;
        a = rotate_left(a, number_of_rotations);
        a += b;

        // Rotating the registers.
        uint32_t temp = d;
        d = c;
        c = b;
        b = a;
        a = temp;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

uint8_t *md5_digest(const char *input)
{
    // Append padding bits.
    size_t padded_length;
    uint8_t *padded_input = pad_input(input, &padded_length);

    // Initialize buffer.
    uint32_t state[4] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476};

    // Process message in 16-Word blocks. A Word is 32 bits.
    for(size_t i = 0; i < padded_length; i += 64) {
        transform(state, padded_input + i);
    }
    free(padded_input);

    // Output digest.
    uint8_t *output = calloc(16, sizeof(uint8_t));
    encode(output, state, 16);

    return output;
}