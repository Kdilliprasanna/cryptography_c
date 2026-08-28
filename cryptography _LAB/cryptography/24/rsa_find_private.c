/* Experiment 24: Given RSA public key (e, n), find the private key d.
   e = 31, n = 3599
   Step 1: factor n = p * q by trial division.
   Step 2: compute phi(n) = (p-1)(q-1).
   Step 3: use the extended Euclidean algorithm to find d = e^-1 mod phi(n). */
#include <stdio.h>

long extgcd(long a, long b, long *x, long *y) {
    if (b == 0) { *x = 1; *y = 0; return a; }
    long x1, y1;
    long g = extgcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

long modInverse(long e, long phi) {
    long x, y;
    extgcd(e, phi, &x, &y);
    return ((x % phi) + phi) % phi;
}

int main() {
    long n = 3599, e = 31;
    long p = 0, q = 0;

    for (long i = 2; i * i <= n; i++) {
        if (n % i == 0) { p = i; q = n / i; break; }
    }

    printf("n = %ld factors as p = %ld, q = %ld\n", n, p, q);

    long phi = (p - 1) * (q - 1);
    printf("phi(n) = (p-1)(q-1) = %ld\n", phi);

    long d = modInverse(e, phi);
    printf("Private key d = e^-1 mod phi(n) = %ld\n", d);

    printf("\nVerification: (e * d) mod phi(n) should equal 1 -> %ld\n", (e * d) % phi);

    /* quick round-trip test */
    long m = 12;
    long c = 1;
    for (long i = 0; i < e; i++) c = (c * m) % n;
    long m2 = 1;
    for (long i = 0; i < d; i++) m2 = (m2 * c) % n;
    printf("Round-trip test: m=%ld -> encrypt with e -> c=%ld -> decrypt with d -> m'=%ld\n", m, c, m2);

    return 0;
}
