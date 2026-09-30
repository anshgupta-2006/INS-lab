#include <stdio.h>
#include <stdint.h>

// Permutation tables for key generation
static const int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
static const int P8[]  = {6, 3, 7, 4, 8, 5, 10, 9};

/**
 * Generic bit permutation function.
 * Maps n_in bits from 'input' to n_out bits based on 1-indexed 'table'.
 */
uint16_t permute(uint16_t input, const int *table, int n_out, int n_in) {
    uint16_t output = 0;
    for (int i = 0; i < n_out; i++) {
        // Extract bit at 1-indexed position from left
        int bit = (input >> (n_in - table[i])) & 1;
        output |= (bit << (n_out - 1 - i));
    }
    return output;
}

/**
 * Performs a circular left shift on a 5-bit unsigned integer.
 */
uint8_t left_shift_5bit(uint8_t val, int shift) {
    val &= 0x1F; // Restrict to 5 bits
    return ((val << shift) | (val >> (5 - shift))) & 0x1F;
}

/**
 * Generates two 8-bit subkeys (K1, K2) from a 10-bit master key.
 */
void generate_sdes_subkeys(uint16_t master_key_10bit, uint8_t *k1, uint8_t *k2) {
    // Step 1: Apply P10 permutation to 10-bit key
    uint16_t p10_key = permute(master_key_10bit, P10, 10, 10);

    // Step 2: Split into Left and Right 5-bit halves
    uint8_t left  = (p10_key >> 5) & 0x1F;
    uint8_t right = p10_key & 0x1F;

    // Step 3: Perform LS-1 (Left Shift by 1 bit on both halves)
    left  = left_shift_5bit(left, 1);
    right = left_shift_5bit(right, 1);

    // Step 4: Combine halves and apply P8 to get K1
    uint16_t combined_ls1 = ((uint16_t)left << 5) | right;
    *k1 = (uint8_t)permute(combined_ls1, P8, 8, 10);

    // Step 5: Perform LS-2 (Left Shift by 2 bits on previously shifted halves)
    left  = left_shift_5bit(left, 2);
    right = left_shift_5bit(right, 2);

    // Step 6: Combine halves and apply P8 to get K2
    uint16_t combined_ls2 = ((uint16_t)left << 5) | right;
    *k2 = (uint8_t)permute(combined_ls2, P8, 8, 10);
}

// Helper function to print values in binary
void print_binary(uint16_t val, int num_bits) {
    for (int i = num_bits - 1; i >= 0; i--) {
        printf("%d", (val >> i) & 1);
    }
}

int main(void) {
    // Example 10-bit key: 1010000010 (0x282)
    uint16_t key = 0b1010000010;
    uint8_t k1 = 0, k2 = 0;

    generate_sdes_subkeys(key, &k1, &k2);

    printf("Input Key (10 bits) : ");
    print_binary(key, 10);
    printf(" (0x%03X)\n\n", key);

    printf("Subkey K1 (8 bits)   : ");
    print_binary(k1, 8);
    printf(" (0x%02X)\n", k1);

    printf("Subkey K2 (8 bits)   : ");
    print_binary(k2, 8);
    printf(" (0x%02X)\n", k2);

    return 0;
}