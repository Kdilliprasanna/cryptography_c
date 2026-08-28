# Cryptography & Network Security Lab — Programs 1 to 40

Each numbered folder contains one self-contained C program implementing the
corresponding experiment from the lab manual (CSA51 CNS List of Programs).

## How to compile and run any program

    cd <folder-number>
    gcc -o prog <filename>.c -lm
    ./prog

(`-lm` is only needed for a few programs that use `<math.h>`; it is harmless
to include for all of them.)

Programs 22 and 23 share `sdes_common.h` — keep that header in the same
folder as the `.c` file when compiling.

## Program index

| # | File | Topic |
|---|------|-------|
| 1  | caesar.c | Caesar cipher |
| 2  | monoalphabetic.c | Monoalphabetic substitution cipher (random key) |
| 3  | playfair.c | Playfair cipher (keyword-built matrix) |
| 4  | vigenere.c | Polyalphabetic (Vigenère) substitution cipher |
| 5  | affine.c | Affine cipher |
| 6  | affine_break.c | Cryptanalysis of an affine cipher from letter frequencies |
| 7  | freq_decode.c | Frequency analysis of an arbitrary symbol-substitution ciphertext |
| 8  | keyword_cipher.c | Monoalphabetic cipher generated from a keyword |
| 9  | playfair_decode.c | Playfair decryption (PT-109 style message) |
| 10 | playfair_fixed_matrix.c | Playfair encryption with a given fixed matrix |
| 11 | playfair_keyspace.c | Playfair cipher key-space size (~2^n) |
| 12 | hill_cipher.c | Hill cipher encrypt/decrypt with a 2x2 key |
| 13 | hill_known_plaintext.c | Known-plaintext attack on the Hill cipher |
| 14 | vigenere_otp.c | One-time-pad version of the Vigenère cipher |
| 15 | freq_attack_additive.c | Automated frequency attack on an additive cipher |
| 16 | freq_attack_mono.c | Automated frequency attack on a monoalphabetic cipher |
| 17 | des_key_schedule.c | DES key schedule (K1..K16, and reversed for decryption) |
| 18 | des_subkey_bits.c | Proof that DES subkey halves come from disjoint C/D bits |
| 19 | cbc_mode.c | CBC mode encryption/decryption demo |
| 20 | ecb_cbc_error.c | Error propagation: ECB vs CBC |
| 21 | padding.c | Block-cipher padding scheme |
| 22 | sdes_cbc.c (+ sdes_common.h) | S-DES in CBC mode (matches given test vector) |
| 23 | sdes_ctr.c (+ sdes_common.h) | S-DES in Counter (CTR) mode |
| 24 | rsa_find_private.c | Recover RSA private key d from (e, n) |
| 25 | rsa_common_factor.c | RSA break via a plaintext sharing a factor with n |
| 26 | rsa_leaked_key.c | Why reusing n after a key leak is unsafe |
| 27 | rsa_small_message_attack.c | Attack on RSA over a tiny message space (0-25) |
| 28 | diffie_hellman.c | Diffie-Hellman key exchange (+ insecure variant demo) |
| 29 | sha3_lanes.c | SHA-3 capacity-lane simulation |
| 30 | cbc_mac_forgery.c | CBC-MAC forgery property demonstration |
| 31 | cmac_subkeys.c | CMAC subkey (K1, K2) generation |
| 32 | dsa_vs_rsa.c | DSA (randomized) vs RSA (deterministic) signatures |
| 33 | des_full.c | Full DES implementation (verified against standard test vector) |
| 34 | padding2.c | Padding for ECB/CBC/CFB modes (variant of #21) |
| 35 | vigenere_otp2.c | Vigenère one-time pad (variant of #14) |
| 36 | affine2.c | Affine cipher (variant of #5) |
| 37 | freq_attack_mono2.c | Monoalphabetic frequency attack (variant of #16) |
| 38 | hill_known_plaintext2.c | Hill known-plaintext attack (variant of #13) |
| 39 | freq_attack_additive2.c | Additive-cipher frequency attack (variant of #15) |
| 40 | freq_attack_mono3.c | Monoalphabetic frequency attack (variant of #16, again) |

Note: the source lab-manual list itself repeats several experiment topics
(e.g. #34/#35/#36/#37/#38/#39/#40 restate earlier questions almost verbatim),
so those programs mirror the earlier ones using different sample data.

## Verified test vectors

These programs were checked against known-correct values during development:

- **#6** recovers key (a=3, b=15) from the given B→e, U→t frequency clue.
- **#8** keyword CIPHER reproduces the exact plain/cipher alphabet given in
  the problem statement.
- **#12** Hill cipher round-trips the full sample sentence correctly.
- **#13 / #38** recover the exact key matrix [[9,4],[5,7]] from two known
  digraph pairs taken from program #12's own output.
- **#14 / #35** reproduce ciphertext `beokjdmsxzpmh` for "send more money"
  with the given key stream, and solve part (b) exactly.
- **#17** K1 matches the standard DES textbook reference value for the
  classic test key `133457799BBCDFF1`.
- **#22** S-DES CBC output for IV=10101010 exactly matches the given
  ciphertext `11110100 00001011`.
- **#23** S-DES CTR mode round-trips correctly for the given test data.
- **#24** recovers d=3031 from e=31, n=3599 (p=59, q=61).
- **#28** Diffie-Hellman with q=353, a=3 (classic textbook example) —
  both sides compute the same shared key (160).
- **#33** full DES: key `133457799BBCDFF1` + plaintext `0123456789ABCDEF`
  produces ciphertext `85E813540F0AB405`, the standard published test vector.

All 40 programs compile cleanly with `gcc` (tested on gcc 13, Ubuntu 24.04)
and were run at least once to confirm sensible output before packaging.
