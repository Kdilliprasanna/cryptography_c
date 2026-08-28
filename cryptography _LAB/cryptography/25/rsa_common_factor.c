/* Experiment 25: If a ciphertext block's underlying plaintext value shares
   a common factor with the RSA modulus n = p*q, an attacker (without the
   private key) can recover that shared factor using gcd(block, n) --
   completely breaking that modulus, since once one prime factor is known
   the other follows by division, and phi(n) can then be computed. */
#include <stdio.h>

long gcd(long a, long b) { return b == 0 ? a : gcd(b, a % b); }

int main() {
    long n, block;

    printf("Enter RSA modulus n: ");
    scanf("%ld", &n);
    printf("Enter the plaintext block value that shares a factor with n: ");
    scanf("%ld", &block);

    long g = gcd(block, n);
    if (g == 1 || g == n) {
        printf("\ngcd(block, n) = %ld -- this block does NOT share a nontrivial\n", g);
        printf("factor with n, so this attack does not apply here.\n");
        return 0;
    }

    long p = g, q = n / g;
    printf("\ngcd(block, n) = %ld  -> this IS a nontrivial factor of n!\n", g);
    printf("n = p * q = %ld * %ld\n", p, q);

    long phi = (p - 1) * (q - 1);
    printf("phi(n) = (p-1)(q-1) = %ld\n", phi);
    printf("\nConclusion: YES, this helps the attacker -- knowing that a\n");
    printf("plaintext block shares a factor with n immediately reveals the\n");
    printf("full factorization of n (via a single gcd computation), which\n");
    printf("completely breaks RSA for this modulus (phi(n) can now be\n");
    printf("computed and any private key d derived).\n");
    return 0;
}
