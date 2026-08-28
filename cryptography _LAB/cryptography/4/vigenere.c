/* Experiment 4: Polyalphabetic Substitution Cipher (Vigenere Cipher)
   Uses a separate monoalphabetic (Caesar) shift for each successive letter
   of the plaintext, the shift being determined by the repeating key. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void vigenereEncrypt(char *text, char *key) {
    int klen = (int)strlen(key), j = 0;
    for (int i = 0; text[i]; i++) {
        if (isalpha((unsigned char)text[i])) {
            int base = isupper((unsigned char)text[i]) ? 'A' : 'a';
            int kshift = toupper(key[j % klen]) - 'A';
            text[i] = (char)(((text[i] - base) + kshift) % 26 + base);
            j++;
        }
    }
}

void vigenereDecrypt(char *text, char *key) {
    int klen = (int)strlen(key), j = 0;
    for (int i = 0; text[i]; i++) {
        if (isalpha((unsigned char)text[i])) {
            int base = isupper((unsigned char)text[i]) ? 'A' : 'a';
            int kshift = toupper(key[j % klen]) - 'A';
            text[i] = (char)(((text[i] - base - kshift) % 26 + 26) % 26 + base);
            j++;
        }
    }
}

int main() {
    char plaintext[500], cipher[500], key[100];

    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';

    printf("Enter keyword: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';

    strcpy(cipher, plaintext);
    vigenereEncrypt(cipher, key);
    printf("\nCiphertext : %s\n", cipher);

    vigenereDecrypt(cipher, key);
    printf("Decrypted  : %s\n", cipher);

    return 0;
}
