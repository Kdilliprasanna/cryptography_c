/* Experiment 8: Monoalphabetic Cipher generated from a Keyword
   (avoids the need to memorize a fully random permuted sequence)
   Example: keyword CIPHER
   plain : a b c d e f g h i j k l m n o p q r s t u v w x y z
   cipher: C I P H E R A B D F G J K L M N O Q S T U V W X Y Z */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char cipherAlpha[26];

void buildFromKeyword(char *key) {
    int used[26] = {0};
    int pos = 0;
    for (int i = 0; key[i]; i++) {
        char ch = (char)toupper((unsigned char)key[i]);
        if (ch < 'A' || ch > 'Z') continue;
        if (!used[ch - 'A']) { used[ch - 'A'] = 1; cipherAlpha[pos++] = ch; }
    }
    for (char ch = 'A'; ch <= 'Z'; ch++)
        if (!used[ch - 'A']) cipherAlpha[pos++] = ch;
}

void encrypt(char *text) {
    for (int i = 0; text[i]; i++) {
        if (isupper((unsigned char)text[i])) text[i] = cipherAlpha[text[i] - 'A'];
        else if (islower((unsigned char)text[i])) text[i] = (char)tolower(cipherAlpha[text[i] - 'a']);
    }
}

void decrypt(char *text) {
    char inv[26];
    for (int i = 0; i < 26; i++) inv[cipherAlpha[i] - 'A'] = (char)('A' + i);
    for (int i = 0; text[i]; i++) {
        if (isupper((unsigned char)text[i])) text[i] = inv[text[i] - 'A'];
        else if (islower((unsigned char)text[i])) text[i] = (char)tolower(inv[text[i] - 'a']);
    }
}

int main() {
    char keyword[100], plaintext[500], cipher[500];

    printf("Enter keyword (e.g. CIPHER): ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0';

    buildFromKeyword(keyword);
    printf("\nplain : ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
    printf("cipher: %s\n\n", cipherAlpha);

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
