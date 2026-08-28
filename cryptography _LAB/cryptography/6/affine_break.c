/* Experiment 6: Break an Affine Cipher using frequency analysis
   Given: the ciphertext was generated with an affine cipher C=(a*p+b) mod 26.
   The most frequent ciphertext letter is 'B' and the second most frequent is 'U'.
   Since the most/second-most frequent letters in English are 'e' and 't',
   we assume  B <-> e (p=4)  and  U <-> t (p=19), and solve two equations
   for the unknown key (a, b), then use it to decrypt the ciphertext. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int modInverse(int a, int m) {
    a = ((a % m) + m) % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1) return x;
    return -1;
}

int main() {
    /* c1 = a*p1 + b (mod 26), c2 = a*p2 + b (mod 26) */
    int p1 = 4,  c1 = 'B' - 'A';   /* e -> B */
    int p2 = 19, c2 = 'U' - 'A';   /* t -> U */

    int diffP = ((p1 - p2) % 26 + 26) % 26;
    int diffC = ((c1 - c2) % 26 + 26) % 26;
    int invDiffP = modInverse(diffP, 26);
    int a = (diffC * invDiffP) % 26;
    int b = ((c1 - a * p1) % 26 + 26) % 26;

    printf("Recovered key: a = %d, b = %d\n", a, b);

    int ainv = modInverse(a, 26);
    printf("Modular inverse of a: %d\n\n", ainv);

    char cipher[500];
    printf("Enter ciphertext to decrypt: ");
    fgets(cipher, sizeof(cipher), stdin);
    cipher[strcspn(cipher, "\n")] = '\0';

    printf("\nDecrypted plaintext: ");
    for (int i = 0; cipher[i]; i++) {
        if (isupper((unsigned char)cipher[i])) {
            int c = cipher[i] - 'A';
            int p = ((ainv * (((c - b) % 26 + 26) % 26)) % 26 + 26) % 26;
            putchar(p + 'A');
        } else if (islower((unsigned char)cipher[i])) {
            int c = cipher[i] - 'a';
            int p = ((ainv * (((c - b) % 26 + 26) % 26)) % 26 + 26) % 26;
            putchar(p + 'a');
        } else {
            putchar(cipher[i]);
        }
    }
    printf("\n");
    return 0;
}
