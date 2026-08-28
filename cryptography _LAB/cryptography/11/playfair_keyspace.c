/* Experiment 11: How many possible keys does the Playfair cipher have?
   The key is an arrangement of 25 letters (I/J combined) in a 5x5 grid,
   so the raw number of keys is 25! . We compute log2(25!) to express the
   answer as an approximate power of 2.

   Effectively-unique keys: many different 5x5 layouts produce identical
   encryption behaviour -- e.g. cyclically shifting every row left/right,
   cyclically shifting every column up/down, and swapping rows with columns
   (transposing) all leave the encryption function unchanged. That is a
   redundancy factor of (5*5*4*2) = 200, i.e. divide 25! by 200 for a fairer
   count of effectively distinct keys. */
#include <stdio.h>
#include <math.h>

int main() {
    double log2_25fact = 0.0;
    for (int i = 1; i <= 25; i++)
        log2_25fact += log2((double)i);

    printf("25! (raw number of Playfair keys)\n");
    printf("log2(25!) = %.4f\n", log2_25fact);
    printf("=> approximately 2^%.0f possible keys\n\n", log2_25fact);

    double redundancy = 5 * 5 * 4 * 2;  /* row shifts * col shifts * transpose */
    double log2_effective = log2_25fact - log2(redundancy);

    printf("Redundancy factor (row/col cyclic shifts + transpose) = %.0f\n", redundancy);
    printf("log2(effectively unique keys) = %.4f\n", log2_effective);
    printf("=> approximately 2^%.0f effectively unique keys\n", log2_effective);

    return 0;
}
