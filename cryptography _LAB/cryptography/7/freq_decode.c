/* Experiment 7: Decrypt a simple-substitution ciphertext using frequency analysis
   Works on arbitrary symbol/character ciphertext (numbers, punctuation marks etc.
   used as cipher "letters", as in classic cryptogram puzzles).
   Strategy (as suggested in the problem):
     1. The most frequent symbol likely stands for 'e'.
     2. Find the most common repeated symbol-group -> likely "the" -> gives t, h.
     3. Print a frequency table + a partially substituted skeleton so the
        rest can be deduced. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSYM 200

int main() {
    char cipher[2000];
    printf("Enter ciphertext (symbols separated by nothing needed): \n");
    fgets(cipher, sizeof(cipher), stdin);
    cipher[strcspn(cipher, "\n")] = '\0';

    char symbols[MAXSYM];
    int freq[MAXSYM];
    int nsym = 0;
    memset(freq, 0, sizeof(freq));

    for (int i = 0; cipher[i]; i++) {
        char ch = cipher[i];
        if (isspace((unsigned char)ch)) continue;
        int found = -1;
        for (int j = 0; j < nsym; j++) if (symbols[j] == ch) { found = j; break; }
        if (found == -1) { symbols[nsym] = ch; freq[nsym] = 1; nsym++; }
        else freq[found]++;
    }

    /* sort symbols by frequency, descending (simple bubble sort) */
    for (int i = 0; i < nsym - 1; i++)
        for (int j = 0; j < nsym - 1 - i; j++)
            if (freq[j] < freq[j + 1]) {
                int tf = freq[j]; freq[j] = freq[j + 1]; freq[j + 1] = tf;
                char ts = symbols[j]; symbols[j] = symbols[j + 1]; symbols[j + 1] = ts;
            }

    printf("\nSymbol Frequency Table (most frequent first):\n");
    printf("Symbol | Count | Likely Letter (English freq order)\n");
    const char *englishOrder = "etaoinshrdlucmfwypvbgkjqxz";
    for (int i = 0; i < nsym; i++) {
        char guess = (i < (int)strlen(englishOrder)) ? englishOrder[i] : '?';
        printf("  %-4c |  %3d  |  %c\n", symbols[i], freq[i], guess);
    }

    /* Build a quick heuristic decode: highest-frequency symbol -> 'e', etc. */
    char map[MAXSYM];
    for (int i = 0; i < nsym; i++)
        map[i] = (i < (int)strlen(englishOrder)) ? englishOrder[i] : '?';

    printf("\nHeuristic first-pass decode (refine manually using repeated\n");
    printf("3-symbol groups for \"the\", double letters, etc.):\n");
    for (int i = 0; cipher[i]; i++) {
        char ch = cipher[i];
        if (isspace((unsigned char)ch)) { putchar(' '); continue; }
        int idx = -1;
        for (int j = 0; j < nsym; j++) if (symbols[j] == ch) { idx = j; break; }
        putchar(idx >= 0 ? map[idx] : ch);
    }
    printf("\n");
    return 0;
}
