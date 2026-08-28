/* Experiment 39: Automated letter-frequency attack on an Additive (Caesar)
   cipher (same technique as Experiment 15, repeated as its own exercise).
   Tries all 25 shifts, scores each candidate with a chi-squared comparison
   against standard English letter frequencies, and prints the top-N most
   likely plaintexts (best score first). */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

double englishFreq[26] = {
    8.167,1.492,2.782,4.253,12.702,2.228,2.015,6.094,6.966,0.153,
    0.772,4.025,2.406,6.749,7.507,1.929,0.095,5.987,6.327,9.056,
    2.758,0.978,2.360,0.150,1.974,0.074
};

int main() {
    char cipher[1000];
    int topn;

    printf("Enter ciphertext: ");
    fgets(cipher, sizeof(cipher), stdin);
    cipher[strcspn(cipher, "\n")] = '\0';

    printf("How many top candidates to display? ");
    scanf("%d", &topn);

    int letters = 0;
    for (int i = 0; cipher[i]; i++) if (isalpha((unsigned char)cipher[i])) letters++;

    double score[26];
    for (int k = 0; k < 26; k++) {
        int count[26] = {0};
        for (int i = 0; cipher[i]; i++) {
            if (isalpha((unsigned char)cipher[i])) {
                int c = toupper((unsigned char)cipher[i]) - 'A';
                int p = ((c - k) % 26 + 26) % 26;
                count[p]++;
            }
        }
        double chi = 0.0;
        for (int j = 0; j < 26; j++) {
            double expected = englishFreq[j] / 100.0 * letters;
            double diff = count[j] - expected;
            if (expected > 0) chi += (diff * diff) / expected;
        }
        score[k] = chi;
    }

    int order[26];
    for (int i = 0; i < 26; i++) order[i] = i;
    for (int i = 0; i < 25; i++)
        for (int j = 0; j < 25 - i; j++)
            if (score[order[j]] > score[order[j + 1]]) {
                int t = order[j]; order[j] = order[j+1]; order[j+1] = t;
            }

    printf("\nTop %d candidate shifts (lower chi-squared score = more likely):\n\n", topn);
    for (int r = 0; r < topn && r < 26; r++) {
        int k = order[r];
        printf("#%2d  shift=%2d  score=%8.2f   Plaintext: ", r + 1, k, score[k]);
        for (int i = 0; cipher[i]; i++) {
            if (isupper((unsigned char)cipher[i]))
                putchar(((cipher[i]-'A'-k)%26+26)%26 + 'A');
            else if (islower((unsigned char)cipher[i]))
                putchar(((cipher[i]-'a'-k)%26+26)%26 + 'a');
            else putchar(cipher[i]);
        }
        printf("\n");
    }
    return 0;
}
