/* Experiment 2: Monoalphabetic Substitution Cipher
   Each plaintext letter maps to a single unique ciphertext letter (a random permutation) */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

char cipherAlpha[26];

void generateKey() {
    char alpha[26];
    for (int i = 0; i < 26; i++) alpha[i] = 'A' + i;
    srand((unsigned)time(NULL));
    for (int i = 25; i > 0; i--) {          /* Fisher-Yates shuffle */
        int j = rand() % (i + 1);
        char t = alpha[i]; alpha[i] = alpha[j]; alpha[j] = t;
    }
    memcpy(cipherAlpha, alpha, 26);
}

void encrypt(char *text) {
    for (int i = 0; text[i]; i++) {
        if (isupper(text[i])) text[i] = cipherAlpha[text[i] - 'A'];
        else if (islower(text[i])) text[i] = tolower(cipherAlpha[text[i] - 'a']);
    }
}

void decrypt(char *text) {
    char inv[26];
    for (int i = 0; i < 26; i++) inv[cipherAlpha[i] - 'A'] = 'A' + i;
    for (int i = 0; text[i]; i++) {
        if (isupper(text[i])) text[i] = inv[text[i] - 'A'];
        else if (islower(text[i])) text[i] = tolower(inv[text[i] - 'a']);
    }
}

int main() {
    char plaintext[500], cipher[500];

    generateKey();
    printf("Generated cipher alphabet key:\n");
    printf("Plain : ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
    printf("Cipher: %s\n\n", cipherAlpha);

    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';

    strcpy(cipher, plaintext);
    encrypt(cipher);
    printf("\nCiphertext : %s\n", cipher);

    decrypt(cipher);
    printf("Decrypted  : %s\n", cipher);

    return 0;
}
