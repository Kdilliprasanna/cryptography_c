/* Experiment 12: Hill Cipher encryption/decryption
   Key matrix K = | 9 4 |     Encrypt "meet me at the usual place at ten
                   | 5 7 |     rather than eight oclock" (2 letters at a time) */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int K[2][2] = {{9, 4}, {5, 7}};
int Kinv[2][2];

int modInverse(int a, int m) {
    a = ((a % m) + m) % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1) return x;
    return -1;
}

void computeInverse() {
    int det = (K[0][0]*K[1][1] - K[0][1]*K[1][0]) % 26;
    det = ((det % 26) + 26) % 26;
    int detInv = modInverse(det, 26);
    printf("Determinant = %d, Determinant inverse mod 26 = %d\n", det, detInv);

    Kinv[0][0] = ( K[1][1] * detInv) % 26;
    Kinv[0][1] = (-K[0][1] * detInv) % 26;
    Kinv[1][0] = (-K[1][0] * detInv) % 26;
    Kinv[1][1] = ( K[0][0] * detInv) % 26;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            Kinv[i][j] = ((Kinv[i][j] % 26) + 26) % 26;
}

void encryptBlock(int p0, int p1, int *c0, int *c1) {
    *c0 = (K[0][0]*p0 + K[0][1]*p1) % 26;
    *c1 = (K[1][0]*p0 + K[1][1]*p1) % 26;
}

void decryptBlock(int c0, int c1, int *p0, int *p1) {
    *p0 = (Kinv[0][0]*c0 + Kinv[0][1]*c1) % 26;
    *p1 = (Kinv[1][0]*c0 + Kinv[1][1]*c1) % 26;
}

int main() {
    char raw[500] = "meetmeattheusualplaceattenratherthaneightoclock";
    char clean[500];
    int len = 0;
    for (int i = 0; raw[i]; i++)
        if (isalpha((unsigned char)raw[i])) clean[len++] = (char)toupper((unsigned char)raw[i]);
    if (len % 2) clean[len++] = 'X';   /* pad if odd */
    clean[len] = '\0';

    printf("Key matrix K = [ %d %d ; %d %d ]\n\n", K[0][0], K[0][1], K[1][0], K[1][1]);
    computeInverse();
    printf("Inverse key matrix Kinv = [ %d %d ; %d %d ]\n\n", Kinv[0][0], Kinv[0][1], Kinv[1][0], Kinv[1][1]);

    printf("Plaintext : %s\n", clean);

    char cipher[500];
    for (int i = 0; i < len; i += 2) {
        int p0 = clean[i] - 'A', p1 = clean[i+1] - 'A', c0, c1;
        encryptBlock(p0, p1, &c0, &c1);
        cipher[i]   = (char)(c0 + 'A');
        cipher[i+1] = (char)(c1 + 'A');
    }
    cipher[len] = '\0';
    printf("Ciphertext: %s\n", cipher);

    char decrypted[500];
    for (int i = 0; i < len; i += 2) {
        int c0 = cipher[i] - 'A', c1 = cipher[i+1] - 'A', p0, p1;
        decryptBlock(c0, c1, &p0, &p1);
        decrypted[i]   = (char)(p0 + 'A');
        decrypted[i+1] = (char)(p1 + 'A');
    }
    decrypted[len] = '\0';
    printf("Decrypted : %s\n", decrypted);

    return 0;
}
