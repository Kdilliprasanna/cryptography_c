/* Experiment 1: Caesar Cipher
   Replace each letter with the letter k places further down the alphabet (k = 1..25) */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void caesarEncrypt(char *text, int k) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (isupper(ch))
            text[i] = (char)(((ch - 'A') + k) % 26 + 'A');
        else if (islower(ch))
            text[i] = (char)(((ch - 'a') + k) % 26 + 'a');
    }
}

void caesarDecrypt(char *text, int k) {
    caesarEncrypt(text, (26 - (k % 26)) % 26);
}

int main() {
    char plaintext[500], cipher[500];
    int k;

    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';

    printf("Enter key k (1-25): ");
    scanf("%d", &k);
    k = ((k % 26) + 26) % 26;

    strcpy(cipher, plaintext);
    caesarEncrypt(cipher, k);
    printf("\nCiphertext : %s\n", cipher);

    caesarDecrypt(cipher, k);
    printf("Decrypted  : %s\n", cipher);

    return 0;
}
