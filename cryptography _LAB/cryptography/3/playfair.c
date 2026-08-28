/* Experiment 3: Playfair Cipher
   5x5 matrix built from a keyword. Plaintext encrypted two letters at a time. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char matrix[5][5];

void buildMatrix(char *key) {
    int used[26] = {0};
    used['J' - 'A'] = 1;           /* I/J share a cell */
    int r = 0, c = 0;
    for (int i = 0; key[i]; i++) {
        char ch = toupper(key[i]);
        if (ch < 'A' || ch > 'Z') continue;
        if (ch == 'J') ch = 'I';
        if (!used[ch - 'A']) {
            used[ch - 'A'] = 1;
            matrix[r][c] = ch;
            c++; if (c == 5) { c = 0; r++; }
        }
    }
    for (int ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue;
        if (!used[ch - 'A']) {
            used[ch - 'A'] = 1;
            matrix[r][c] = (char)ch;
            c++; if (c == 5) { c = 0; r++; }
        }
    }
}

void locate(char ch, int *row, int *col) {
    if (ch == 'J') ch = 'I';
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (matrix[i][j] == ch) { *row = i; *col = j; return; }
}

/* Prepare plaintext: uppercase, split into digraphs, insert X between double
   letters in a pair, pad with X if odd length */
int prepare(char *in, char *out) {
    int n = 0;
    char buf[1000];
    int len = 0;
    for (int i = 0; in[i]; i++)
        if (isalpha((unsigned char)in[i])) buf[len++] = (char)toupper(in[i]);
    buf[len] = '\0';

    for (int i = 0; i < len; i++) {
        char a = buf[i] == 'J' ? 'I' : buf[i];
        out[n++] = a;
        if (i + 1 < len) {
            char b = buf[i + 1] == 'J' ? 'I' : buf[i + 1];
            if (a == b) {
                out[n++] = 'X';
            } else {
                out[n++] = b;
                i++;
            }
        } else {
            out[n++] = 'X';
        }
    }
    if (n % 2 != 0) out[n++] = 'X';
    out[n] = '\0';
    return n;
}

void playfairEncrypt(char *plain, char *cipher) {
    int len = prepare(plain, cipher);
    for (int i = 0; i < len; i += 2) {
        int r1, c1, r2, c2;
        locate(cipher[i], &r1, &c1);
        locate(cipher[i + 1], &r2, &c2);
        if (r1 == r2) {
            cipher[i]     = matrix[r1][(c1 + 1) % 5];
            cipher[i + 1] = matrix[r2][(c2 + 1) % 5];
        } else if (c1 == c2) {
            cipher[i]     = matrix[(r1 + 1) % 5][c1];
            cipher[i + 1] = matrix[(r2 + 1) % 5][c2];
        } else {
            cipher[i]     = matrix[r1][c2];
            cipher[i + 1] = matrix[r2][c1];
        }
    }
}

void playfairDecrypt(char *cipher, char *plain) {
    int len = (int)strlen(cipher);
    strcpy(plain, cipher);
    for (int i = 0; i < len; i += 2) {
        int r1, c1, r2, c2;
        locate(cipher[i], &r1, &c1);
        locate(cipher[i + 1], &r2, &c2);
        if (r1 == r2) {
            plain[i]     = matrix[r1][(c1 + 4) % 5];
            plain[i + 1] = matrix[r2][(c2 + 4) % 5];
        } else if (c1 == c2) {
            plain[i]     = matrix[(r1 + 4) % 5][c1];
            plain[i + 1] = matrix[(r2 + 4) % 5][c2];
        } else {
            plain[i]     = matrix[r1][c2];
            plain[i + 1] = matrix[r2][c1];
        }
    }
}

int main() {
    char keyword[100], plaintext[500], cipher[1000], recovered[1000];

    printf("Enter keyword: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0';

    buildMatrix(keyword);
    printf("\nPlayfair 5x5 Matrix:\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) printf("%c ", matrix[i][j]);
        printf("\n");
    }

    printf("\nEnter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';

    playfairEncrypt(plaintext, cipher);
    printf("\nCiphertext : %s\n", cipher);

    playfairDecrypt(cipher, recovered);
    printf("Decrypted  : %s\n", recovered);

    return 0;
}
