/* Experiment 14: One-Time-Pad version of the Vigenere cipher
   The key is a stream of random numbers (0-26), a different shift for
   every letter of plaintext.
   a. Encrypt "send more money" with key stream 9 0 1 7 23 15 21 14 11 11 2 8 9
   b. Given the resulting ciphertext, find a NEW key stream that decrypts
      the SAME ciphertext to "cash not needed". */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char plaintext[] = "sendmoremoney";
    int key1[] = {9, 0, 1, 7, 23, 15, 21, 14, 11, 11, 2, 8, 9};
    int n = (int)strlen(plaintext);

    char cipher[50];
    printf("Part (a)\n");
    printf("Plaintext : %s\n", plaintext);
    printf("Key stream: ");
    for (int i = 0; i < n; i++) printf("%d ", key1[i]);
    printf("\n");

    for (int i = 0; i < n; i++)
        cipher[i] = (char)(((plaintext[i] - 'a') + key1[i]) % 26 + 'a');
    cipher[n] = '\0';
    printf("Ciphertext: %s\n\n", cipher);

    printf("Part (b)\n");
    char plaintext2[] = "cashnotneeded";
    printf("Target plaintext: %s\n", plaintext2);
    printf("Required key stream (key2[i] = cipher[i] - plaintext2[i] mod 26): ");
    int key2[50];
    for (int i = 0; i < n; i++) {
        key2[i] = ((cipher[i] - 'a') - (plaintext2[i] - 'a') % 26 + 26) % 26;
        printf("%d ", key2[i]);
    }
    printf("\n");

    /* Verify */
    char check[50];
    for (int i = 0; i < n; i++)
        check[i] = (char)(((cipher[i] - 'a') - key2[i] + 26) % 26 + 'a');
    check[n] = '\0';
    printf("Verification (decrypt cipher with key2): %s\n", check);

    return 0;
}
