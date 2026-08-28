/* Experiment 17: DES Key Schedule (encryption order K1..K16, and the
   REVERSED order K16..K1 used for decryption).
   Standard DES key-schedule tables: PC-1, PC-2 and the per-round left
   shift schedule. */
#include <stdio.h>
#include <string.h>

int PC1[56] = {
    57,49,41,33,25,17, 9, 1,58,50,42,34,26,18,
    10, 2,59,51,43,35,27,19,11, 3,60,52,44,36,
    63,55,47,39,31,23,15, 7,62,54,46,38,30,22,
    14, 6,61,53,45,37,29,21,13, 5,28,20,12, 4
};
int PC2[48] = {
    14,17,11,24, 1, 5, 3,28,15, 6,21,10,
    23,19,12, 4,26, 8,16, 7,27,20,13, 2,
    41,52,31,37,47,55,30,40,51,45,33,48,
    44,49,39,56,34,53,46,42,50,36,29,32
};
int shifts[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

void permute(int *in, int *out, int *table, int n) {
    for (int i = 0; i < n; i++) out[i] = in[table[i] - 1];
}

void leftShift(int *half, int n, int s) {
    int tmp[28];
    memcpy(tmp, half, 28 * sizeof(int));
    for (int i = 0; i < 28; i++) half[i] = tmp[(i + s) % 28];
    (void)n;
}

int main() {
    /* Standard test key: 133457799BBCDFF1 (hex, 64 bits incl. parity) */
    char *hexKey = "133457799BBCDFF1";
    int keyBits[64], pc1out[56], C[28], D[28];
    int K[16][48];

    for (int i = 0; i < 16; i++) {
        char h = hexKey[i];
        int val = (h >= '0' && h <= '9') ? h - '0' : h - 'A' + 10;
        for (int b = 0; b < 4; b++) keyBits[i*4 + b] = (val >> (3 - b)) & 1;
    }

    permute(keyBits, pc1out, PC1, 56);
    memcpy(C, pc1out, 28 * sizeof(int));
    memcpy(D, pc1out + 28, 28 * sizeof(int));

    printf("Generating round keys K1..K16 (encryption order):\n");
    for (int r = 0; r < 16; r++) {
        leftShift(C, 28, shifts[r]);
        leftShift(D, 28, shifts[r]);
        int CD[56];
        memcpy(CD, C, 28 * sizeof(int));
        memcpy(CD + 28, D, 28 * sizeof(int));
        permute(CD, K[r], PC2, 48);

        printf("K%-2d = ", r + 1);
        for (int i = 0; i < 48; i++) { printf("%d", K[r][i]); if ((i+1)%4==0) printf(" "); }
        printf("\n");
    }

    printf("\nFor DES DECRYPTION, the SAME 16 keys are applied in REVERSE order:\n");
    for (int r = 15; r >= 0; r--) {
        printf("Round %2d uses K%-2d = ", 16 - r, r + 1);
        for (int i = 0; i < 48; i++) { printf("%d", K[r][i]); if ((i+1)%4==0) printf(" "); }
        printf("\n");
    }

    return 0;
}
