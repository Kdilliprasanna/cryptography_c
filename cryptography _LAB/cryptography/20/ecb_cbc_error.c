/* Experiment 20: Error propagation in ECB vs CBC mode.
   a. In ECB, a transmission error in one ciphertext block corrupts ONLY the
      corresponding plaintext block on decryption -- no other blocks are
      affected.
   b. In CBC, an error in ciphertext block Ci corrupts plaintext block Pi
      completely (garbled), AND causes a corresponding single-bit error in
      P(i+1) at the same bit position (because Ci is XORed into P(i+1)'s
      computation) -- but blocks beyond P(i+1) are NOT affected. */
#include <stdio.h>
#include <string.h>

#define BLOCK 8

void blockCipher(unsigned char *block, unsigned char *key) {
    for (int i = 0; i < BLOCK; i++)
        block[i] = (unsigned char)((block[i] ^ key[i % 8]) + 3);
}
void blockCipherInverse(unsigned char *block, unsigned char *key) {
    for (int i = 0; i < BLOCK; i++)
        block[i] = (unsigned char)((block[i] - 3) ^ key[i % 8]);
}
void xorBlock(unsigned char *a, unsigned char *b) {
    for (int i = 0; i < BLOCK; i++) a[i] ^= b[i];
}

int main() {
    unsigned char key[] = "MYKEY123";
    unsigned char iv[BLOCK] = "12345678";
    unsigned char plaintext[] = "BLOCK111BLOCK222BLOCK333";  /* 3 blocks of 8 */
    int blocks = (int)strlen((char*)plaintext) / BLOCK;

    unsigned char ecbCipher[100], cbcCipher[100];

    /* --- ECB encrypt --- */
    for (int b = 0; b < blocks; b++) {
        unsigned char block[BLOCK];
        memcpy(block, plaintext + b*BLOCK, BLOCK);
        blockCipher(block, key);
        memcpy(ecbCipher + b*BLOCK, block, BLOCK);
    }
    /* --- CBC encrypt --- */
    unsigned char prev[BLOCK]; memcpy(prev, iv, BLOCK);
    for (int b = 0; b < blocks; b++) {
        unsigned char block[BLOCK];
        memcpy(block, plaintext + b*BLOCK, BLOCK);
        xorBlock(block, prev);
        blockCipher(block, key);
        memcpy(cbcCipher + b*BLOCK, block, BLOCK);
        memcpy(prev, block, BLOCK);
    }

    /* --- Introduce a single-bit error in ciphertext block 1 (index 0) --- */
    ecbCipher[0] ^= 0x01;
    cbcCipher[0] ^= 0x01;

    printf("=== ECB: decrypting with a corrupted C1 ===\n");
    for (int b = 0; b < blocks; b++) {
        unsigned char block[BLOCK];
        memcpy(block, ecbCipher + b*BLOCK, BLOCK);
        blockCipherInverse(block, key);
        
        printf("P%d recovered: %.8s\n", b+1, block);
    }

    printf("\n=== CBC: decrypting with a corrupted C1 ===\n");
    memcpy(prev, iv, BLOCK);
    for (int b = 0; b < blocks; b++) {
        unsigned char block[BLOCK], saved[BLOCK];
        memcpy(block, cbcCipher + b*BLOCK, BLOCK);
        memcpy(saved, block, BLOCK);
        blockCipherInverse(block, key);
        xorBlock(block, prev);
        
        printf("P%d recovered: %.8s\n", b+1, block);
        memcpy(prev, saved, BLOCK);
    }

    printf("\nObservation:\n");
    printf("ECB : only P1 is affected -> P2, P3 decrypt correctly.\n");
    printf("CBC : P1 is completely garbled AND P2 has a matching single-bit\n");
    printf("      flip at the same position (since C1 feeds into P2's XOR).\n");
    printf("      P3 onward are NOT affected.\n");
    return 0;
}
