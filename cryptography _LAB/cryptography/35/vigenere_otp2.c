/* Experiment 35: One-Time-Pad version of the Vigenere cipher.
   (Same construction as Experiment 14, demonstrated here with a different
   message/key pair as its own standalone exercise.)
   The key is a stream of random numbers between 0 and 26; the i-th letter
   of plaintext is shifted by the i-th number in the key stream. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char plaintext[100], cipher[100];
    int key[100], n;

    printf("Enter plaintext (letters only, no spaces): ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';
    n = (int)strlen(plaintext);

    printf("Enter %d key-stream numbers (0-26), separated by spaces:\n", n);
    for (int i = 0; i < n; i++) scanf("%d", &key[i]);

    for (int i = 0; i < n; i++) {
        char base = isupper((unsigned char)plaintext[i]) ? 'A' : 'a';
        cipher[i] = (char)(((tolower(plaintext[i]) - 'a') + key[i]) % 26 + base);
    }
    cipher[n] = '\0';

    printf("\nPlaintext : %s\n", plaintext);
    printf("Key stream: "); for (int i = 0; i < n; i++) printf("%d ", key[i]); printf("\n");
    printf("Ciphertext: %s\n", cipher);

    /* decrypt back to verify */
    char decrypted[100];
    for (int i = 0; i < n; i++) {
        char base = isupper((unsigned char)cipher[i]) ? 'A' : 'a';
        decrypted[i] = (char)(((tolower(cipher[i]) - 'a') - key[i] % 26 + 26) % 26 + base);
    }
    decrypted[n] = '\0';
    printf("Decrypted : %s\n", decrypted);

    return 0;
}
