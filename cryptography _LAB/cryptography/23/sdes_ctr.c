/* Experiment 23: Encrypt and decrypt in Counter (CTR) mode using S-DES.
   Test data: Counter starts at 0000 0000, Key = 0111111101,
              Plaintext = 0000 0001 0000 0010 0000 0100  (three 8-bit blocks)
   In CTR mode: keystream_i = E(K, counter_i); ciphertext_i = plaintext_i XOR keystream_i
   The counter is simply incremented (as an 8-bit binary number) for each block. */
#include <stdio.h>
#include <string.h>
#include "sdes_common.h"

void xor8(int *a, int *b, int *out) {
    for (int i = 0; i < 8; i++) out[i] = a[i] ^ b[i];
}

void incrementCounter(int *ctr) {
    for (int i = 7; i >= 0; i--) {
        if (ctr[i] == 0) { ctr[i] = 1; break; }
        else ctr[i] = 0;
    }
}

int main() {
    int key10[10]; strToBits("0111111101", key10, 10);
    int K1[8], K2[8];
    sdesKeygen(key10, K1, K2);

    int counter[8]; strToBits("00000000", counter, 8);
    int plains[3][8];
    strToBits("00000001", plains[0], 8);
    strToBits("00000010", plains[1], 8);
    strToBits("00000100", plains[2], 8);

    int ciphers[3][8];
    printf("=== CTR Mode Encryption ===\n");
    printf("Key = 0111111101, Counter starts at 00000000\n\n");
    for (int b = 0; b < 3; b++) {
        int keystream[8];
        sdesEncryptBlock(counter, K1, K2, keystream);
        xor8(plains[b], keystream, ciphers[b]);

        char cs[9], ks[9], ps[9], cts[9];
        bitsToStr(counter, 8, cts); bitsToStr(keystream, 8, ks);
        bitsToStr(plains[b], 8, ps); bitsToStr(ciphers[b], 8, cs);
        printf("Block %d: counter=%s  keystream=%s  plain=%s  cipher=%s\n",
               b + 1, cts, ks, ps, cs);
        incrementCounter(counter);
    }

    /* Decryption: same process, XOR ciphertext with the same keystream */
    printf("\n=== CTR Mode Decryption (verification) ===\n");
    strToBits("00000000", counter, 8);
    for (int b = 0; b < 3; b++) {
        int keystream[8], recovered[8];
        sdesEncryptBlock(counter, K1, K2, keystream);
        xor8(ciphers[b], keystream, recovered);
        char rs[9];
        bitsToStr(recovered, 8, rs);
        printf("Block %d recovered plaintext: %s\n", b + 1, rs);
        incrementCounter(counter);
    }
    return 0;
}
