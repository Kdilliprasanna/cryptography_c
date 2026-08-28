/* Common S-DES (Simplified DES) implementation, used by both the CBC
   (Experiment 22) and Counter-mode (Experiment 23) programs.
   Reference: W. Stallings, "Cryptography and Network Security", Appendix on S-DES. */
#ifndef SDES_COMMON_H
#define SDES_COMMON_H

static int P10[10] = {3,5,2,7,4,10,1,9,8,6};
static int P8[8]   = {6,3,7,4,8,5,10,9};
static int IP[8]   = {2,6,3,1,4,8,5,7};
static int IPinv[8]= {4,1,3,5,7,2,8,6};
static int EP[8]   = {4,1,2,3,2,3,4,1};
static int P4[4]   = {2,4,3,1};

static int S0[4][4] = {
    {1,0,3,2},
    {3,2,1,0},
    {0,2,1,3},
    {3,1,3,2}
};
static int S1[4][4] = {
    {0,1,2,3},
    {2,0,1,3},
    {3,0,1,0},
    {2,1,0,3}
};

static void permute(int *in, int *out, int *table, int n) {
    for (int i = 0; i < n; i++) out[i] = in[table[i] - 1];
}
static void leftShiftN(int *arr, int n, int s) {
    int tmp[10];
    for (int i = 0; i < n; i++) tmp[i] = arr[(i + s) % n];
    for (int i = 0; i < n; i++) arr[i] = tmp[i];
}

/* Generate the two 8-bit subkeys K1, K2 from a 10-bit key */
static void sdesKeygen(int *key10, int *K1, int *K2) {
    int p10out[10];
    permute(key10, p10out, P10, 10);
    int left[5], right[5];
    for (int i = 0; i < 5; i++) { left[i] = p10out[i]; right[i] = p10out[i + 5]; }

    leftShiftN(left, 5, 1); leftShiftN(right, 5, 1);
    int combined[10];
    for (int i = 0; i < 5; i++) { combined[i] = left[i]; combined[i+5] = right[i]; }
    permute(combined, K1, P8, 8);

    leftShiftN(left, 5, 2); leftShiftN(right, 5, 2);
    for (int i = 0; i < 5; i++) { combined[i] = left[i]; combined[i+5] = right[i]; }
    permute(combined, K2, P8, 8);
}

static int sBoxLookup(int s[4][4], int *bits4) {
    int row = bits4[0] * 2 + bits4[3];
    int col = bits4[1] * 2 + bits4[2];
    return s[row][col];
}

static void fFunction(int *right4, int *key8, int *out4) {
    int ep[8];
    permute(right4, ep, EP, 8);
    int x[8];
    for (int i = 0; i < 8; i++) x[i] = ep[i] ^ key8[i];

    int s0in[4] = {x[0], x[1], x[2], x[3]};
    int s1in[4] = {x[4], x[5], x[6], x[7]};
    int s0val = sBoxLookup(S0, s0in);
    int s1val = sBoxLookup(S1, s1in);

    int sOut[4] = { (s0val >> 1) & 1, s0val & 1, (s1val >> 1) & 1, s1val & 1 };
    permute(sOut, out4, P4, 4);
}

/* One S-DES round: fk operation */
static void fk(int *bits8, int *key8) {
    int left[4], right[4];
    for (int i = 0; i < 4; i++) { left[i] = bits8[i]; right[i] = bits8[i+4]; }
    int fout[4];
    fFunction(right, key8, fout);
    for (int i = 0; i < 4; i++) bits8[i] = left[i] ^ fout[i];
    for (int i = 0; i < 4; i++) bits8[i+4] = right[i];
}
static void swapHalves(int *bits8) {
    int tmp[4];
    for (int i = 0; i < 4; i++) tmp[i] = bits8[i];
    for (int i = 0; i < 4; i++) bits8[i] = bits8[i+4];
    for (int i = 0; i < 4; i++) bits8[i+4] = tmp[i];
}

static void sdesEncryptBlock(int *plain8, int *K1, int *K2, int *cipher8) {
    int ip[8];
    permute(plain8, ip, IP, 8);
    fk(ip, K1);
    swapHalves(ip);
    fk(ip, K2);
    permute(ip, cipher8, IPinv, 8);
}
static void sdesDecryptBlock(int *cipher8, int *K1, int *K2, int *plain8) {
    int ip[8];
    permute(cipher8, ip, IP, 8);
    fk(ip, K2);
    swapHalves(ip);
    fk(ip, K1);
    permute(ip, plain8, IPinv, 8);
}

static void strToBits(const char *s, int *bits, int n) {
    for (int i = 0; i < n; i++) bits[i] = s[i] - '0';
}
static void bitsToStr(int *bits, int n, char *s) {
    for (int i = 0; i < n; i++) s[i] = (char)(bits[i] + '0');
    s[n] = '\0';
}

#endif
