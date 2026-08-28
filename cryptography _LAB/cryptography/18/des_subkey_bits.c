/* Experiment 18: Show that in DES, the first 24 bits of each round subkey
   come from the C (first 28-bit) half of the key schedule, and the second
   24 bits come from the D (second 28-bit) half -- by inspecting the PC-2
   permutation table itself. PC-2 selects 48 out of the 56 bits of C||D
   (indices 1-28 belong to C, 29-56 belong to D). */
#include <stdio.h>

int PC2[48] = {
    14,17,11,24, 1, 5, 3,28,15, 6,21,10,
    23,19,12, 4,26, 8,16, 7,27,20,13, 2,
    41,52,31,37,47,55,30,40,51,45,33,48,
    44,49,39,56,34,53,46,42,50,36,29,32
};

int main() {
    printf("PC-2 table entry -> source half (C = bits 1-28, D = bits 29-56)\n\n");
    int fromC_first24 = 0, fromD_first24 = 0;
    int fromC_second24 = 0, fromD_second24 = 0;

    for (int i = 0; i < 48; i++) {
        int bit = PC2[i];
        const char *half = (bit <= 28) ? "C" : "D";
        printf("PC2[%2d] = %2d  -> from %s\n", i + 1, bit, half);
        if (i < 24) { if (bit <= 28) fromC_first24++; else fromD_first24++; }
        else        { if (bit <= 28) fromC_second24++; else fromD_second24++; }
    }

    printf("\nSummary:\n");
    printf("First 24 output bits  : %d from C, %d from D\n", fromC_first24, fromD_first24);
    printf("Second 24 output bits : %d from C, %d from D\n", fromC_second24, fromD_second24);
    printf("\nConclusion: the first 24 bits of every round key come ENTIRELY\n");
    printf("from the 28-bit C register, and the second 24 bits come ENTIRELY\n");
    printf("from the disjoint 28-bit D register -- confirmed by the PC-2 table.\n");
    return 0;
}
