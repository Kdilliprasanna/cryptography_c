/* Experiment 27: Alice encrypts single alphabetic characters (0-25) directly
   with RSA (large e, large n) -- is this secure?
   ANSWER: NO. Even though factoring n is infeasible, the message SPACE is
   only 26 possible values. An eavesdropper who intercepts ciphertext c can
   simply encrypt all 26 possible plaintexts (0..25) with the PUBLIC key
   and compare against c -- a "chosen/known plaintext space" attack that
   needs no factoring at all. This program demonstrates the attack. */
#include <stdio.h>

long power(long base, long exp, long mod) {
    long result = 1; base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    long n = 3599, e = 31;   /* Alice's / Bob's public key (large in practice) */
    long secretLetter = 4;   /* Alice encodes 'E' as 4, unknown to attacker */

    long c = power(secretLetter, e, n);
    printf("Public key: (e=%ld, n=%ld)\n", e, n);
    printf("Alice encrypts letter value %ld -> ciphertext c = %ld\n\n", secretLetter, c);

    printf("Attacker's brute-force over the 26-letter message space:\n");
    for (long m = 0; m < 26; m++) {
        long enc = power(m, e, n);
        if (enc == c)
            printf("  m=%ld ('%c') -> encrypts to %ld  <-- MATCH! Message recovered.\n",
                   m, (char)('A' + m), enc);
    }

    printf("\nConclusion: encoding each character separately with plain RSA\n");
    printf("(no padding, tiny message space) is INSECURE -- the most efficient\n");
    printf("attack is simply to encrypt all 26 possible letters with the public\n");
    printf("key and match against the ciphertext. Proper RSA use requires\n");
    printf("randomized padding (e.g. OAEP) precisely to defeat this attack.\n");
    return 0;
}
