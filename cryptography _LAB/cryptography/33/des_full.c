/* Experiment 33: Full DES (Data Encryption Standard) implementation.
   64-bit blocks, 56-bit effective key (64 bits with parity), 16 rounds.
   Verified against the standard textbook test vector:
     Key       = 133457799BBCDFF1
     Plaintext = 0123456789ABCDEF
     Ciphertext= 85E813540F0AB405                                        */
#include <stdio.h>
#include <string.h>

/* ---------- Standard DES tables ---------- */
static int IP[64] = {
    58,50,42,34,26,18,10,2, 60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6, 64,56,48,40,32,24,16,8,
    57,49,41,33,25,17, 9,1, 59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5, 63,55,47,39,31,23,15,7
};
static int FP[64] = {
    40,8,48,16,56,24,64,32, 39,7,47,15,55,23,63,31,
    38,6,46,14,54,22,62,30, 37,5,45,13,53,21,61,29,
    36,4,44,12,52,20,60,28, 35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26, 33,1,41, 9,49,17,57,25
};
static int E[48] = {
    32,1,2,3,4,5, 4,5,6,7,8,9, 8,9,10,11,12,13,
    12,13,14,15,16,17, 16,17,18,19,20,21, 20,21,22,23,24,25,
    24,25,26,27,28,29, 28,29,30,31,32,1
};
static int P[32] = {
    16,7,20,21,29,12,28,17, 1,15,23,26,5,18,31,10,
    2,8,24,14,32,27,3,9, 19,13,30,6,22,11,4,25
};
static int PC1[56] = {
    57,49,41,33,25,17, 9, 1,58,50,42,34,26,18,
    10, 2,59,51,43,35,27,19,11, 3,60,52,44,36,
    63,55,47,39,31,23,15, 7,62,54,46,38,30,22,
    14, 6,61,53,45,37,29,21,13, 5,28,20,12, 4
};
static int PC2[48] = {
    14,17,11,24, 1, 5, 3,28,15, 6,21,10,
    23,19,12, 4,26, 8,16, 7,27,20,13, 2,
    41,52,31,37,47,55,30,40,51,45,33,48,
    44,49,39,56,34,53,46,42,50,36,29,32
};
static int SHIFTS[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

static int SBOX[8][4][16] = {
 {{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
  {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
  {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
  {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
 {{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
  {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
  {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
  {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
 {{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
  {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
  {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
  {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
 {{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
  {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
  {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
  {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
 {{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
  {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
  {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
  {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
 {{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
  {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
  {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
  {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
 {{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
  {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
  {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
  {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
 {{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
  {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
  {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
  {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}
};

/* ---------- helpers ---------- */
static void hexToBits(const char *hex, int *bits, int nbits) {
    int nhex = nbits / 4;
    for (int i = 0; i < nhex; i++) {
        char h = hex[i];
        int val = (h >= '0' && h <= '9') ? h - '0' :
                  (h >= 'A' && h <= 'F') ? h - 'A' + 10 : h - 'a' + 10;
        for (int b = 0; b < 4; b++) bits[i*4+b] = (val >> (3-b)) & 1;
    }
}
static void bitsToHex(int *bits, int nbits, char *hex) {
    int nhex = nbits / 4;
    for (int i = 0; i < nhex; i++) {
        int val = 0;
        for (int b = 0; b < 4; b++) val = (val << 1) | bits[i*4+b];
        hex[i] = "0123456789ABCDEF"[val];
    }
    hex[nhex] = '\0';
}
static void permute(int *in, int *out, int *table, int n) {
    for (int i = 0; i < n; i++) out[i] = in[table[i]-1];
}
static void leftShiftN(int *arr, int n, int s) {
    int tmp[28];
    for (int i = 0; i < n; i++) tmp[i] = arr[(i+s)%n];
    for (int i = 0; i < n; i++) arr[i] = tmp[i];
}

/* generate all 16 round keys (48 bits each) */
static void keySchedule(int *key64, int K[16][48]) {
    int key56[56], pc1out[56];
    /* drop parity bits: DES key input here given as 64 bits already
       laid out for PC1, so PC1 itself selects the 56 significant bits */
    memcpy(key56, key64, 56 * sizeof(int)); /* placeholder, PC1 reads from 64-bit key64 */
    permute(key64, pc1out, PC1, 56);
    int C[28], D[28];
    memcpy(C, pc1out, 28*sizeof(int));
    memcpy(D, pc1out+28, 28*sizeof(int));
    for (int r = 0; r < 16; r++) {
        leftShiftN(C, 28, SHIFTS[r]);
        leftShiftN(D, 28, SHIFTS[r]);
        int CD[56];
        memcpy(CD, C, 28*sizeof(int));
        memcpy(CD+28, D, 28*sizeof(int));
        permute(CD, K[r], PC2, 48);
    }
    (void)key56;
}

static void fFunction(int *R, int *K48, int *out32) {
    int expanded[48];
    permute(R, expanded, E, 48);
    int x[48];
    for (int i = 0; i < 48; i++) x[i] = expanded[i] ^ K48[i];

    int sboxOut[32];
    for (int s = 0; s < 8; s++) {
        int *b = x + s*6;
        int row = b[0]*2 + b[5];
        int col = b[1]*8 + b[2]*4 + b[3]*2 + b[4];
        int val = SBOX[s][row][col];
        for (int k = 0; k < 4; k++) sboxOut[s*4+k] = (val >> (3-k)) & 1;
    }
    permute(sboxOut, out32, P, 32);
}

/* core DES process; encrypt=1 uses K[0..15] forward, encrypt=0 uses reverse order */
static void desProcess(int *inBits64, int *outBits64, int K[16][48], int encrypt) {
    int ip[64];
    permute(inBits64, ip, IP, 64);
    int L[32], Rr[32];
    memcpy(L, ip, 32*sizeof(int));
    memcpy(Rr, ip+32, 32*sizeof(int));

    for (int round = 0; round < 16; round++) {
        int keyIndex = encrypt ? round : (15 - round);
        int fout[32];
        fFunction(Rr, K[keyIndex], fout);
        int newR[32];
        for (int i = 0; i < 32; i++) newR[i] = L[i] ^ fout[i];
        memcpy(L, Rr, 32*sizeof(int));
        memcpy(Rr, newR, 32*sizeof(int));
    }
    int preOutput[64];
    memcpy(preOutput, Rr, 32*sizeof(int));       /* note the final swap (R16,L16) */
    memcpy(preOutput+32, L, 32*sizeof(int));
    permute(preOutput, outBits64, FP, 64);
}

int main() {
    char keyHex[20], plainHex[20];
    printf("Enter 16-hex-digit key (default 133457799BBCDFF1): ");
    fgets(keyHex, sizeof(keyHex), stdin);
    keyHex[strcspn(keyHex, "\n")] = '\0';
    if (strlen(keyHex) != 16) strcpy(keyHex, "133457799BBCDFF1");

    printf("Enter 16-hex-digit plaintext block (default 0123456789ABCDEF): ");
    fgets(plainHex, sizeof(plainHex), stdin);
    plainHex[strcspn(plainHex, "\n")] = '\0';
    if (strlen(plainHex) != 16) strcpy(plainHex, "0123456789ABCDEF");

    int keyBits[64], plainBits[64];
    hexToBits(keyHex, keyBits, 64);
    hexToBits(plainHex, plainBits, 64);

    int K[16][48];
    keySchedule(keyBits, K);

    int cipherBits[64];
    desProcess(plainBits, cipherBits, K, 1);
    char cipherHex[20];
    bitsToHex(cipherBits, 64, cipherHex);

    int recoveredBits[64];
    desProcess(cipherBits, recoveredBits, K, 0);
    char recoveredHex[20];
    bitsToHex(recoveredBits, 64, recoveredHex);

    printf("\nKey       : %s\n", keyHex);
    printf("Plaintext : %s\n", plainHex);
    printf("Ciphertext: %s\n", cipherHex);
    printf("Decrypted : %s\n", recoveredHex);

    printf("\n(Standard test vector 133457799BBCDFF1 / 0123456789ABCDEF should\n");
    printf("produce ciphertext 85E813540F0AB405.)\n");
    return 0;
}
