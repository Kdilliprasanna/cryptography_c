/* Experiment 38: Known-Plaintext Attack on the Hill Cipher.
   (Same attack as Experiment 13 -- the Hill cipher succumbs to a
   known-plaintext attack whenever enough plaintext-ciphertext pairs are
   available, since C = K*P (mod 26) implies K = C * P^-1 (mod 26).) */
#include <stdio.h>
#include <ctype.h>

int modInverse(int a, int m) {
    a = ((a % m) + m) % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1) return x;
    return -1;
}

int main() {
    char p1[3], p2[3], c1[3], c2[3];

    printf("Enter first known plaintext digraph (2 letters): ");
    scanf("%2s", p1);
    printf("Enter its ciphertext digraph (2 letters): ");
    scanf("%2s", c1);
    printf("Enter second known plaintext digraph (2 letters): ");
    scanf("%2s", p2);
    printf("Enter its ciphertext digraph (2 letters): ");
    scanf("%2s", c2);

    int P[2][2] = {
        { toupper(p1[0]) - 'A', toupper(p2[0]) - 'A' },
        { toupper(p1[1]) - 'A', toupper(p2[1]) - 'A' }
    };
    int C[2][2] = {
        { toupper(c1[0]) - 'A', toupper(c2[0]) - 'A' },
        { toupper(c1[1]) - 'A', toupper(c2[1]) - 'A' }
    };

    int det = (P[0][0]*P[1][1] - P[0][1]*P[1][0]) % 26;
    det = ((det % 26) + 26) % 26;
    int detInv = modInverse(det, 26);
    if (detInv == -1) {
        printf("\nThe plaintext matrix is not invertible mod 26 with this pair.\n");
        printf("Choose a different / additional pair of known digraphs.\n");
        return 1;
    }

    int Pinv[2][2];
    Pinv[0][0] = ( P[1][1] * detInv) % 26;
    Pinv[0][1] = (-P[0][1] * detInv) % 26;
    Pinv[1][0] = (-P[1][0] * detInv) % 26;
    Pinv[1][1] = ( P[0][0] * detInv) % 26;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            Pinv[i][j] = ((Pinv[i][j] % 26) + 26) % 26;

    int K[2][2];
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            K[i][j] = ((C[i][0]*Pinv[0][j] + C[i][1]*Pinv[1][j]) % 26 + 26) % 26;

    printf("\nRecovered key matrix K =\n");
    printf("| %2d %2d |\n", K[0][0], K[0][1]);
    printf("| %2d %2d |\n", K[1][0], K[1][1]);

    /* Verify: K * P should equal C */
    printf("\nVerification (K * P mod 26 should equal C):\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++)
            printf("%2d ", ((K[i][0]*P[0][j] + K[i][1]*P[1][j]) % 26 + 26) % 26);
        printf("\n");
    }
    return 0;
}
