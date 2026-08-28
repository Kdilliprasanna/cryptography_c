/* Experiment 5: Affine Cipher (generalization of the Caesar cipher)
   C = E([a,b], p) = (a*p + b) mod 26   ,   p = D([a,b], C) = a^-1 * (C - b) mod 26
   For the cipher to be one-to-one (invertible), gcd(a, 26) must equal 1.
   There is NO restriction on b -- any value 0..25 works, because adding a
   constant is always a bijection modulo 26. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }

int modInverse(int a, int m) {
    a = ((a % m) + m) % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1) return x;
    return -1;
}

void listValidA() {
    printf("Valid values of 'a' (must satisfy gcd(a,26)=1):\n");
    for (int a = 1; a < 26; a++)
        if (gcd(a, 26) == 1) printf("%d ", a);
    printf("\n(b has no restriction -- any value 0-25 is allowed)\n\n");
}

void affineEncrypt(char *text, int a, int b) {
    for (int i = 0; text[i]; i++) {
        if (isupper((unsigned char)text[i]))
            text[i] = (char)((a * (text[i] - 'A') + b) % 26 + 'A');
        else if (islower((unsigned char)text[i]))
            text[i] = (char)((a * (text[i] - 'a') + b) % 26 + 'a');
    }
}

void affineDecrypt(char *text, int a, int b) {
    int ainv = modInverse(a, 26);
    for (int i = 0; text[i]; i++) {
        if (isupper((unsigned char)text[i])) {
            int c = text[i] - 'A';
            int p = ((ainv * (((c - b) % 26 + 26) % 26)) % 26 + 26) % 26;
            text[i] = (char)(p + 'A');
        } else if (islower((unsigned char)text[i])) {
            int c = text[i] - 'a';
            int p = ((ainv * (((c - b) % 26 + 26) % 26)) % 26 + 26) % 26;
            text[i] = (char)(p + 'a');
        }
    }
}

int main() {
    char plaintext[500], cipher[500];
    int a, b;

    listValidA();

    printf("Enter a: "); scanf("%d", &a);
    printf("Enter b: "); scanf("%d", &b);
    getchar();

    if (gcd(a, 26) != 1) {
        printf("Invalid 'a': gcd(a,26) must be 1. Cipher would not be one-to-one.\n");
        return 1;
    }

    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';

    strcpy(cipher, plaintext);
    affineEncrypt(cipher, a, b);
    printf("\nCiphertext : %s\n", cipher);

    affineDecrypt(cipher, a, b);
    printf("Decrypted  : %s\n", cipher);

    return 0;
}
