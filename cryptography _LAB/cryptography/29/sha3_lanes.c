/* Experiment 29: SHA-3 with a 1024-bit block (rate r=1024, capacity c=576,
   total state 1600 bits = 25 lanes of 64 bits each). Assume every lane of
   the first message block P0 has at least one nonzero bit. Initially, all
   lanes belonging to the CAPACITY portion of the state are all-zero.
   Question: ignoring the Keccak permutation, how long until all those
   capacity lanes have at least one nonzero bit?

   Reasoning modeled by this program: the sponge construction only XORs
   each new message block into the RATE portion of the state; the
   permutation (theta/rho/pi/chi/iota) is the ONLY step that mixes bits
   between the rate and the capacity. If we are told to IGNORE the
   permutation, then the capacity lanes are never touched by anything --
   they simply stay all-zero forever, no matter how many message blocks
   are absorbed. This program simulates exactly that (absorb block after
   block, XOR only into the rate words, and check the capacity words). */
#include <stdio.h>

#define LANES 25
#define RATE_LANES 16     /* 1024 bits / 64 bits per lane = 16 rate lanes  */
#define CAP_LANES  9      /* 25 - 16 = 9 capacity lanes (576 bits)        */

int main() {
    unsigned long long state[LANES] = {0};   /* all lanes start at zero */
    unsigned long long messageBlock[RATE_LANES];
    for (int i = 0; i < RATE_LANES; i++) messageBlock[i] = 0x1ULL << (i % 64); /* nonzero */

    printf("Capacity lanes (indices %d..%d) start all-zero.\n\n", RATE_LANES, LANES - 1);

    int blocksAbsorbed = 0;
    int maxBlocksToTry = 1000000;
    int allCapacityNonzero = 0;

    for (blocksAbsorbed = 1; blocksAbsorbed <= maxBlocksToTry; blocksAbsorbed++) {
        /* Absorb: XOR the message block into the RATE lanes ONLY.
           (No permutation applied -- as instructed by the problem.) */
        for (int i = 0; i < RATE_LANES; i++)
            state[i] ^= messageBlock[i];

        /* Check capacity lanes */
        int allNonzero = 1;
        for (int i = RATE_LANES; i < LANES; i++)
            if (state[i] == 0) { allNonzero = 0; break; }

        if (allNonzero) { allCapacityNonzero = 1; break; }
    }

    if (allCapacityNonzero) {
        printf("All capacity lanes became nonzero after %d blocks.\n", blocksAbsorbed);
    } else {
        printf("After %d absorbed message blocks, the capacity lanes are STILL\n", maxBlocksToTry);
        printf("all-zero (checked): ");
        for (int i = RATE_LANES; i < LANES; i++) printf("%llu ", state[i]);
        printf("\n\n");
        printf("CONCLUSION: if the permutation step is ignored, the capacity\n");
        printf("lanes NEVER become nonzero, no matter how many blocks are\n");
        printf("absorbed -- because only the permutation mixes bits between the\n");
        printf("rate and the capacity. Absorbing message blocks alone only ever\n");
        printf("touches the rate portion of the state.\n");
    }
    return 0;
}
