/* Experiment 28: Diffie-Hellman Key Exchange
   Each participant selects a secret x, and sends a^x mod q to the other.
   Both sides then compute the same shared secret key = (received value)^ownSecret mod q.

   Discussion question: what if participants sent x*a (mod q) instead of a^x mod q?
   -> That would be INSECURE: given the public value v = x*a mod q and the
      known public constant a, anyone (including Eve) could recover x
      directly via x = v * a^-1 mod q (ordinary modular division), since
      multiplication has an efficient inverse (unlike modular
      exponentiation, which is a one-way function -- the discrete log
      problem). So Eve COULD find the secret numbers, and could then also
      compute the "shared key" herself -- the whole scheme would be broken. */
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
    long q = 353, a = 3;      /* public prime q and primitive root a */
    long xa = 97, xb = 233;   /* Alice's and Bob's secret numbers */

    printf("Public parameters: q = %ld, a = %ld (primitive root)\n\n", q, a);

    long YA = power(a, xa, q);   /* Alice's public value */
    long YB = power(a, xb, q);   /* Bob's public value   */
    printf("Alice's secret xa = %ld  -> sends YA = a^xa mod q = %ld\n", xa, YA);
    printf("Bob's secret   xb = %ld  -> sends YB = a^xb mod q = %ld\n\n", xb, YB);

    long keyAlice = power(YB, xa, q);
    long keyBob   = power(YA, xb, q);
    printf("Alice computes shared key = YB^xa mod q = %ld\n", keyAlice);
    printf("Bob computes shared key   = YA^xb mod q = %ld\n", keyBob);
    printf("Keys match: %s\n\n", (keyAlice == keyBob) ? "YES" : "NO");

    printf("--- If participants instead sent x*a mod q ---\n");
    long vA = (xa * a) % q;
    printf("Alice would send vA = xa*a mod q = %ld\n", vA);
    /* Eve recovers xa by multiplying by the modular inverse of a */
    long ainv = -1;
    for (long t = 1; t < q; t++) if ((a * t) % q == 1) { ainv = t; break; }
    long recoveredXa = (vA * ainv) % q;
    printf("Eve computes a^-1 mod q = %ld, then xa = vA * a^-1 mod q = %ld\n", ainv, recoveredXa);
    printf("Eve fully recovers Alice's secret xa = %ld  -> SYSTEM BROKEN.\n", recoveredXa);

    return 0;
}
