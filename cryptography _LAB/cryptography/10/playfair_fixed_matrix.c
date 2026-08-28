/* Experiment 10: Playfair Encryption using a given (fixed) 5x5 matrix
   M F H I/J K
   U N O P Q
   Z V W X Y
   E L A R G
   D S T B C
   Encrypt: "Must see you over Cadogan West. Coming at once." */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char matrix[5][5] = {
    {'M','F','H','I','K'},
    {'U','N','O','P','Q'},
    {'Z','V','W','X','Y'},
    {'E','L','A','R','G'},
    {'D','S','T','B','C'}
};

void locate(char ch, int *row, int *col) {
    if (ch == 'J') ch = 'I';
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (matrix[i][j] == ch) { *row = i; *col = j; return; }
}

int prepare(char *in, char *out) {
    int n = 0, len = 0;
    char buf[1000];
    for (int i = 0; in[i]; i++)
        if (isalpha((unsigned char)in[i])) buf[len++] = (char)toupper((unsigned char)in[i]);
    buf[len] = '\0';

    for (int i = 0; i < len; i++) {
        char a = buf[i] == 'J' ? 'I' : buf[i];
        out[n++] = a;
        if (i + 1 < len) {
            char b = buf[i + 1] == 'J' ? 'I' : buf[i + 1];
            if (a == b) { out[n++] = 'X'; }
            else { out[n++] = b; i++; }
        } else out[n++] = 'X';
    }
    if (n % 2) out[n++] = 'X';
    out[n] = '\0';
    return n;
}

void playfairEncrypt(char *plain, char *cipher) {
    int len = prepare(plain, cipher);
    for (int i = 0; i < len; i += 2) {
        int r1, c1, r2, c2;
        locate(cipher[i], &r1, &c1);
        locate(cipher[i + 1], &r2, &c2);
        if (r1 == r2) { cipher[i] = matrix[r1][(c1+1)%5]; cipher[i+1] = matrix[r2][(c2+1)%5]; }
        else if (c1 == c2) { cipher[i] = matrix[(r1+1)%5][c1]; cipher[i+1] = matrix[(r2+1)%5][c2]; }
        else { cipher[i] = matrix[r1][c2]; cipher[i+1] = matrix[r2][c1]; }
    }
}

int main() {
    char plaintext[] = "Must see you over Cadogan West. Coming at once.";
    char cipher[1000];

    printf("Fixed Playfair Matrix:\n");
    for (int i = 0; i < 5; i++) { for (int j = 0; j < 5; j++) printf("%c ", matrix[i][j]); printf("\n"); }

    printf("\nPlaintext : %s\n", plaintext);
    playfairEncrypt(plaintext, cipher);
    printf("Ciphertext: %s\n", cipher);
    return 0;
}
