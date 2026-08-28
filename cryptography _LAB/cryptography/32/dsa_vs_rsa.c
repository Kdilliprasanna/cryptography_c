/* Experiment 32: DSA uses a fresh random k for every signature, so signing
   the SAME message twice gives DIFFERENT signatures. RSA signing is
   deterministic (same message -> same signature every time).
   Implication: DSA signatures give no information to an observer even if
   the same message is signed repeatedly (better resistance to certain
   attacks/replay-pattern analysis), whereas identical RSA signatures for
   repeated messages could leak the fact that the same message was signed
   again. This program demonstrates both with small toy parameters
   (NOT cryptographically secure sizes -- for illustration only). */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long power(long base, long exp, long mod) {
    long result = 1; base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}
long extgcd(long a, long b, long *x, long *y) {
    if (b == 0) { *x = 1; *y = 0; return a; }
    long x1, y1; long g = extgcd(b, a % b, &x1, &y1);
    *x = y1; *y = x1 - (a / b) * y1; return g;
}
long modInverse(long e, long m) {
    long x, y; extgcd(e, m, &x, &y);
    return ((x % m) + m) % m;
}

/* --- Toy DSA-like scheme --- */
long p = 283, q = 47, g = 60, x_priv = 24; /* q | (p-1); g has order q mod p */

void dsaSign(long hashMsg, long k, long *r, long *s) {
    *r = power(g, k, p) % q;
    long kinv = modInverse(k, q);
    *s = (kinv * (hashMsg + x_priv * (*r))) % q;
}

/* --- Toy RSA-like signing (deterministic) --- */
long n_rsa = 3599, d_rsa = 3031;
long rsaSign(long hashMsg) { return power(hashMsg, d_rsa, n_rsa); }

int main() {
    srand((unsigned)time(NULL));
    long hashMsg = 12345 % q;

    printf("=== DSA-style signing (random k each time) ===\n");
    long r1, s1, r2, s2;
    long k1 = 1 + rand() % (q - 1);
    long k2;
    do { k2 = 1 + rand() % (q - 1); } while (k2 == k1);

    dsaSign(hashMsg, k1, &r1, &s1);
    dsaSign(hashMsg, k2, &r2, &s2);

    printf("Signature #1 (k=%ld): (r=%ld, s=%ld)\n", k1, r1, s1);
    printf("Signature #2 (k=%ld): (r=%ld, s=%ld)\n", k2, r2, s2);
    printf("Same message, DIFFERENT signatures: %s\n\n",
           (r1 != r2 || s1 != s2) ? "YES (as expected)" : "NO (unlucky k collision)");

    printf("=== RSA-style signing (deterministic) ===\n");
    long sig1 = rsaSign(hashMsg % n_rsa);
    long sig2 = rsaSign(hashMsg % n_rsa);
    printf("Signature #1: %ld\n", sig1);
    printf("Signature #2: %ld\n", sig2);
    printf("Same message, SAME signature both times: %s\n",
           (sig1 == sig2) ? "YES (as expected)" : "NO (unexpected)");

    printf("\nImplication: an eavesdropper who sees two RSA signatures on the\n");
    printf("same message can immediately tell they are the same message\n");
    printf("(identical signature bytes). With DSA, repeated signing of the\n");
    printf("same message produces unrelated-looking signatures each time,\n");
    printf("so no such pattern is leaked.\n");
    return 0;
}
