/* Experiment 22: Encrypt and decrypt in Cipher Block Chaining (CBC) mode
   using S-DES as the block cipher.
   Test data: IV = 10101010, Key = 0111111101,
              Plaintext = 0000 0001 0010 0011  (two 8-bit blocks) */
#include <stdio.h>
#include <string.h>
#include "sdes_common.h"

void xor8(int *a, int *b, int *out) {
    for (int i = 0; i < 8; i++) out[i] = a[i] ^ b[i];
}

int main() {
    int key10[10]; strToBits("0111111101", key10, 10);
    int K1[8], K2[8];
    sdesKeygen(key10, K1, K2);

    int iv[8]; strToBits("10101010", iv, 8);
    int p1[8], p2[8]; strToBits("00000001", p1, 8); strToBits("00100011", p2, 8);

    /* --- CBC Encryption --- */
    int x1[8], c1[8], x2[8], c2[8];
    xor8(p1, iv, x1);
    sdesEncryptBlock(x1, K1, K2, c1);
    xor8(p2, c1, x2);
    sdesEncryptBlock(x2, K1, K2, c2);

    char s1[9], s2[9];
    bitsToStr(c1, 8, s1); bitsToStr(c2, 8, s2);
    printf("=== CBC Encryption ===\n");
    printf("IV        : 10101010\n");
    printf("Plaintext : 00000001 00100011\n");
    printf("Ciphertext: %s %s\n\n", s1, s2);

    /* --- CBC Decryption --- */
    int d1[8], r1[8], d2[8], r2[8];
    sdesDecryptBlock(c1, K1, K2, d1);
    xor8(d1, iv, r1);
    sdesDecryptBlock(c2, K1, K2, d2);
    xor8(d2, c1, r2);

    char rs1[9], rs2[9];
    bitsToStr(r1, 8, rs1); bitsToStr(r2, 8, rs2);
    printf("=== CBC Decryption (verification) ===\n");
    printf("Recovered plaintext: %s %s\n", rs1, rs2);

    return 0;
}
