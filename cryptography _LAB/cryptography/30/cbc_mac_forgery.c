/* Experiment 30: CBC-MAC forgery property.
   T = MAC(K, X) for a ONE-BLOCK message X.
   Claim: the adversary immediately knows MAC(K, X || (X XOR T)) -- it is
   again T -- WITHOUT knowing K.
   Why: MAC(K, X||Y) = E(K, E(K,X) XOR Y) for a two-block message X||Y.
        Here Y = X XOR T, and E(K,X) = T (by definition of the one-block MAC).
        So MAC(K, X||Y) = E(K, T XOR (X XOR T)) = E(K, X) = T.  QED.
   This program verifies the algebraic identity with any toy block cipher
   E() (the identity holds for ANY block cipher, not just this toy one). */
#include <stdio.h>
#include <string.h>

#define BLOCK 8

void E(unsigned char *block, unsigned char *key) {
    for (int i = 0; i < BLOCK; i++)
        block[i] = (unsigned char)((block[i] ^ key[i % 8]) + 7);
}
void xorBlock(unsigned char *a, unsigned char *b, unsigned char *out) {
    for (int i = 0; i < BLOCK; i++) out[i] = a[i] ^ b[i];
}

int main() {
    unsigned char key[] = "SECRET12";
    unsigned char X[BLOCK] = "MESSAGE1";

    unsigned char T[BLOCK];
    memcpy(T, X, BLOCK);
    E(T, key);   /* T = MAC(K, X) = E(K, X) for a one-block message */

    printf("X (one-block message) = %.*s\n", BLOCK, X);
    printf("T = MAC(K, X) = "); for (int i=0;i<BLOCK;i++) printf("%02X ", T[i]); printf("\n\n");

    unsigned char Y[BLOCK];
    xorBlock(X, T, Y);   /* Y = X XOR T (attacker can compute this without K!) */
    printf("Attacker computes Y = X XOR T (no key needed): ");
    for (int i=0;i<BLOCK;i++) printf("%02X ", Y[i]); printf("\n\n");

    /* Now compute the REAL CBC-MAC of the two-block forged message X||Y,
       using the actual algorithm (which needs K) just to VERIFY the claim */
    unsigned char state[BLOCK];
    memcpy(state, X, BLOCK);
    E(state, key);                 /* first block: E(K, X) = T */
    unsigned char in2[BLOCK];
    xorBlock(state, Y, in2);       /* XOR with second message block Y */
    E(in2, key);                   /* second block: E(K, T XOR Y) */

    printf("Real CBC-MAC of forged message X||Y = ");
    for (int i=0;i<BLOCK;i++) printf("%02X ", in2[i]); printf("\n");
    printf("Original T                          = ");
    for (int i=0;i<BLOCK;i++) printf("%02X ", T[i]); printf("\n\n");

    printf("%s\n", memcmp(in2, T, BLOCK) == 0
        ? "MATCH confirmed -- the adversary forged a valid MAC without the key!"
        : "mismatch (unexpected)");
    return 0;
}
