/* Experiment 19: Encryption using Cipher Block Chaining (CBC) mode with a
   block cipher stronger than single DES (e.g. 3DES). This program
   demonstrates the CBC chaining MECHANISM itself (XOR with previous
   ciphertext block, then apply the block cipher) using a pluggable
   blockCipher() function -- swap in any strong cipher (3DES/AES) there.
   Discussion:
   a. For SECURITY: prefer 3DES (or any strong block cipher) in CBC mode --
      CBC hides repeated plaintext-block patterns and is well understood.
   b. For PERFORMANCE: CBC encryption cannot be parallelised (each block
      depends on the previous ciphertext), so if throughput matters more
      than strict chaining, a mode like CTR (parallelisable) with a fast
      cipher is usually preferred; among chained modes, CFB/OFB give
      stream-like behaviour but are still serial for encryption. */
#include <stdio.h>
#include <string.h>

#define BLOCK 8

/* Placeholder "strong block cipher" -- replace with real 3DES/AES call */
void blockCipher(unsigned char *block, unsigned char *key) {
    for (int i = 0; i < BLOCK; i++)
        block[i] = (unsigned char)((block[i] ^ key[i % strlen((char*)key)]) + 1);
}
void blockCipherInverse(unsigned char *block, unsigned char *key) {
    for (int i = 0; i < BLOCK; i++)
        block[i] = (unsigned char)((block[i] - 1) ^ key[i % strlen((char*)key)]);
}

void xorBlock(unsigned char *a, unsigned char *b) {
    for (int i = 0; i < BLOCK; i++) a[i] ^= b[i];
}

int main() {
    unsigned char iv[BLOCK] = "INITVEC!";
    unsigned char key[] = "STRONGKEY123456";
    unsigned char plaintext[] = "THISISASECRETMESSAGE123";
    int len = (int)strlen((char*)plaintext);
    int blocks = (len + BLOCK - 1) / BLOCK;

    unsigned char buf[1000];
    memset(buf, 0, sizeof(buf));
    memcpy(buf, plaintext, len);

    unsigned char cipher[1000];
    unsigned char prev[BLOCK];
    memcpy(prev, iv, BLOCK);

    printf("Plaintext blocks -> CBC ciphertext blocks:\n");
    for (int b = 0; b < blocks; b++) {
        unsigned char block[BLOCK];
        memcpy(block, buf + b * BLOCK, BLOCK);
        xorBlock(block, prev);
        blockCipher(block, key);
        memcpy(cipher + b * BLOCK, block, BLOCK);
        memcpy(prev, block, BLOCK);
        printf("C%d: ", b + 1);
        for (int i = 0; i < BLOCK; i++) printf("%02X ", block[i]);
        printf("\n");
    }

    /* Decrypt */
    memcpy(prev, iv, BLOCK);
    unsigned char recovered[1000];
    for (int b = 0; b < blocks; b++) {
        unsigned char block[BLOCK], saved[BLOCK];
        memcpy(block, cipher + b * BLOCK, BLOCK);
        memcpy(saved, block, BLOCK);
        blockCipherInverse(block, key);
        xorBlock(block, prev);
        memcpy(recovered + b * BLOCK, block, BLOCK);
        memcpy(prev, saved, BLOCK);
    }
    recovered[len] = '\0';
    printf("\nRecovered plaintext: %s\n", recovered);

    printf("\nDiscussion:\n");
    printf("a. Security  -> use 3DES (or AES) in CBC mode.\n");
    printf("b. Performance -> CTR mode (parallelisable) is generally faster;\n");
    printf("   CBC's serial chaining limits throughput on multi-core hardware.\n");
    return 0;
}
