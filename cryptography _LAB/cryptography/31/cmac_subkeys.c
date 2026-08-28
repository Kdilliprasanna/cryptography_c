/* Experiment 31: CMAC subkey generation (NIST SP 800-38B).
   L = E(K, 0^b)
   K1 = L << 1              if MSB(L) = 0
      = (L << 1) XOR Rb     if MSB(L) = 1
   K2 = K1 << 1              if MSB(K1) = 0
      = (K1 << 1) XOR Rb    if MSB(K1) = 1

   a. Required constants Rb:
        block size b = 64  bits (e.g. DES)  -> Rb = 0x1B
        block size b = 128 bits (e.g. AES)  -> Rb = 0x87
   b. The left shift and conditional XOR implement multiplication by the
      polynomial "x" in GF(2^b) (with reduction polynomial defined by Rb)
      -- i.e. K1 = L * x and K2 = L * x^2 in that field. Shifting left is
      the natural way to multiply by x; if the top bit (MSB) that falls off
      is 1, the result must be reduced modulo the field's irreducible
      polynomial, which is done by XORing the constant Rb (the low-order
      bits of that polynomial). This program demonstrates the process on
      a generic b-bit block using a toy "E" function. */
#include <stdio.h>
#include <stdint.h>

/* Toy block cipher output L = E(K, 0^b), for demonstration only */
uint64_t toyE(uint64_t key, int bits) {
    uint64_t mask = (bits == 64) ? ~0ULL : ((1ULL << bits) - 1);
    uint64_t v = (key * 2654435761ULL + 0x9E3779B97F4A7C15ULL) & mask;
    return v;
}

void generateSubkeys(uint64_t L, int bits, uint64_t Rb, uint64_t *K1, uint64_t *K2) {
    uint64_t mask = (bits == 64) ? ~0ULL : ((1ULL << bits) - 1);
    uint64_t msbMask = 1ULL << (bits - 1);

    int msbL = (L & msbMask) ? 1 : 0;
    uint64_t shifted = (L << 1) & mask;
    *K1 = msbL ? (shifted ^ Rb) : shifted;

    int msbK1 = (*K1 & msbMask) ? 1 : 0;
    uint64_t shifted2 = (*K1 << 1) & mask;
    *K2 = msbK1 ? (shifted2 ^ Rb) : shifted2;
}

int main() {
    printf("Part (a) Required Rb constants:\n");
    printf("  block size  64 bits (DES-like)  -> Rb = 0x1B\n");
    printf("  block size 128 bits (AES-like)  -> Rb = 0x87\n\n");

    printf("Part (b) Demonstration for a 16-bit toy block (b=16, Rb=0x1B kept for illustration):\n");
    int bits = 16;
    uint64_t Rb = 0x1B;
    uint64_t key = 0x1234;
    uint64_t L = toyE(key, bits) & 0xFFFF;
    uint64_t K1, K2;
    generateSubkeys(L, bits, Rb, &K1, &K2);

    printf("  L  = E(K, 0^b) = 0x%04llX\n", (unsigned long long)L);
    printf("  K1 = 0x%04llX\n", (unsigned long long)K1);
    printf("  K2 = 0x%04llX\n\n", (unsigned long long)K2);

    printf("Demonstration for a 64-bit block (Rb = 0x1B):\n");
    bits = 64; Rb = 0x1B;
    L = toyE(0x1122334455667788ULL, bits);
    generateSubkeys(L, bits, Rb, &K1, &K2);
    printf("  L  = 0x%016llX\n", (unsigned long long)L);
    printf("  K1 = 0x%016llX\n", (unsigned long long)K1);
    printf("  K2 = 0x%016llX\n", (unsigned long long)K2);

    return 0;
}
