/* Experiment 16: Automated letter-frequency attack on a Monoalphabetic
   Substitution cipher. Ranks ciphertext symbols by frequency and maps them,
   in order, onto the standard English letter-frequency order
   (E T A O I N S H R D L U ...) to produce a first-pass candidate plaintext.
   The result is only a starting point -- true monoalphabetic cryptanalysis
   normally also uses digraph/trigraph statistics, which this program lists
   as extra clues to refine the guess by hand. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char cipher[2000];
    printf("Enter ciphertext: ");
    fgets(cipher, sizeof(cipher), stdin);
    cipher[strcspn(cipher, "\n")] = '\0';

    int freq[26] = {0};
    for (int i = 0; cipher[i]; i++)
        if (isalpha((unsigned char)cipher[i]))
            freq[toupper((unsigned char)cipher[i]) - 'A']++;

    int order[26];
    for (int i = 0; i < 26; i++) order[i] = i;
    for (int i = 0; i < 25; i++)
        for (int j = 0; j < 25 - i; j++)
            if (freq[order[j]] < freq[order[j + 1]]) {
                int t = order[j]; order[j] = order[j + 1]; order[j + 1] = t;
            }

    printf("\nCiphertext letter frequency ranking (most -> least frequent):\n");
    for (int i = 0; i < 26; i++)
        if (freq[order[i]] > 0)
            printf("%c(%d) ", 'A' + order[i], freq[order[i]]);
    printf("\n\n");

    const char *englishOrder = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
    char map[26];
    for (int i = 0; i < 26; i++) map[order[i]] = englishOrder[i];

    printf("Heuristic candidate plaintext (top-10 highest frequency mappings\n");
    printf("shown; refine remaining rare letters manually using bigrams/\n");
    printf("trigrams such as \"the\", \"and\", double letters, etc.):\n\n");

    for (int i = 0; cipher[i]; i++) {
        char ch = cipher[i];
        if (isalpha((unsigned char)ch)) {
            int idx = toupper((unsigned char)ch) - 'A';
            char out = map[idx];
            putchar(islower((unsigned char)ch) ? tolower(out) : out);
        } else putchar(ch);
    }
    printf("\n\n");

    printf("Top-10 candidate substitutions (cipher -> plain):\n");
    for (int i = 0; i < 10 && i < 26; i++)
        if (freq[order[i]] > 0)
            printf("  %c -> %c   (count %d)\n", 'A' + order[i], map[order[i]], freq[order[i]]);

    return 0;
}
