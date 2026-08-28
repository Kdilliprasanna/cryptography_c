/* Experiment 26: Bob's RSA private key leaks. He keeps the SAME modulus n
   and just generates a NEW public/private key pair (e2, d2). Is this safe?

   ANSWER: NO. Whoever learned d1 (paired with e1, n) can use the standard
   "factor n given e and d" algorithm to recover n's prime factors p, q --
   and once p, q (hence phi(n)) are known, ANY future key pair using the
   SAME modulus n is instantly breakable, because d2 = e2^-1 mod phi(n)
   can be recomputed directly. Bob must generate a brand-new modulus n. */
#include <stdio.h>

long gcd(long a, long b) { return b == 0 ? a : gcd(b, a % b); }
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
long modInverse(long e, long phi) {
    long x, y; extgcd(e, phi, &x, &y);
    return ((x % phi) + phi) % phi;
}

/* Standard algorithm: factor n given that (e*d - 1) is a multiple of phi(n) */
void factorFromED(long n, long e, long d, long *p, long *q) {
    long k = e * d - 1;
    long t = k;
    while (t % 2 == 0) t /= 2;
    for (long a = 2; a < n; a++) {
        long tt = t;
        long x = power(a, tt, n);
        while (tt < k) {
            long y = (x * x) % n;
            if (y == 1 && x != 1 && x != n - 1) {
                *p = gcd(x - 1, n);
                *q = n / (*p);
                return;
            }
            x = y; tt *= 2;
        }
    }
    *p = 0; *q = 0;
}

int main() {
    long n = 3599, e1 = 31, d1 = 3031;   /* Bob's original (now leaked) key pair */

    printf("Original leaked key pair: n=%ld, e1=%ld, d1=%ld\n", n, e1, d1);

    long p, q;
    factorFromED(n, e1, d1, &p, &q);
    printf("Attacker factors n using (e1, d1) -> p=%ld, q=%ld\n", p, q);

    long phi = (p - 1) * (q - 1);
    printf("phi(n) recovered = %ld\n\n", phi);

    /* Bob generates a NEW key pair with the SAME n */
    long e2 = 17;
    while (gcd(e2, phi) != 1) e2++;
    long d2 = modInverse(e2, phi);
    printf("Bob's NEW key pair (same modulus n): e2=%ld, d2=%ld\n", e2, d2);

    long d2_attacker = modInverse(e2, phi);
    printf("Attacker recomputes d2 using recovered phi(n) -> d2=%ld\n", d2_attacker);

    printf("\nConclusion: %s\n",
           (d2 == d2_attacker) ? "NOT SAFE -- attacker derives the new private key instantly!"
                               : "safe");
    printf("Bob MUST generate a brand new modulus n, not just a new (e,d) pair.\n");
    return 0;
}
