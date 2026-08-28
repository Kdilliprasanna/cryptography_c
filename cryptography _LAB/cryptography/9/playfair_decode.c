/* Experiment 9: Decrypt a Playfair-enciphered message
   (PT-109 style ciphertext). Reuses the standard Playfair 5x5 matrix built
   from a keyword; set KEYWORD below to whatever key was used to encipher
   the message, then paste the ciphertext groups when prompted. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char matrix[5][5];

void buildMatrix(char *key) {
    int used[26] = {0};
    used['J' - 'A'] = 1;
    int r = 0, c = 0;
    for (int i = 0; key[i]; i++) {
        char ch = (char)toupper((unsigned char)key[i]);
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

void playfairDecrypt(char *cipher, char *plain) {
    int len = (int)strlen(cipher);
    strcpy(plain, cipher);
    for (int i = 0; i + 1 < len; i += 2) {
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
    char keyword[100], raw[2000], cipherOnly[2000], plain[2000];
    int n = 0;

    printf("Enter keyword used to build the Playfair matrix: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0';
    buildMatrix(keyword);

    printf("\nPlayfair 5x5 Matrix:\n");
    for (int i = 0; i < 5; i++) { for (int j = 0; j < 5; j++) printf("%c ", matrix[i][j]); printf("\n"); }

    printf("\nEnter ciphertext groups (spaces allowed): ");
    fgets(raw, sizeof(raw), stdin);
    raw[strcspn(raw, "\n")] = '\0';

    for (int i = 0; raw[i]; i++)
        if (isalpha((unsigned char)raw[i])) cipherOnly[n++] = (char)toupper((unsigned char)raw[i]);
    cipherOnly[n] = '\0';

    playfairDecrypt(cipherOnly, plain);
    printf("\nDecrypted plaintext (in digraphs, remove filler X's manually): %s\n", plain);
    return 0;
}
