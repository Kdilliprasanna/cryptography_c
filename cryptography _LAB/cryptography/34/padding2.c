/* Experiment 34: Padding for ECB, CBC, and CFB modes.
   (Same padding requirement as Experiment 21 -- repeated here as a
   separate exercise in the lab manual.)
   For ECB, CBC and CFB, the plaintext must be a whole multiple of the
   block/segment size. Padding rule: append a single 1-bit, then as many
   0-bits as needed to complete the final block (byte version: 0x80
   followed by 0x00 bytes). Good practice pads EVERY message, even one
   that already ends on an exact block boundary -- this removes any
   ambiguity for the receiver when stripping the padding back off. */
#include <stdio.h>
#include <string.h>

#define BLOCK 8

int addPadding(unsigned char *data, int len, unsigned char *out) {
    memcpy(out, data, len);
    out[len] = 0x80;                       /* the leading 1-bit */
    int total = len + 1;
    while (total % BLOCK != 0) out[total++] = 0x00;
    return total;
}

int removePadding(unsigned char *data, int len) {
    int i = len - 1;
    while (i >= 0 && data[i] == 0x00) i--;
    if (i >= 0 && data[i] == 0x80) return i;
    return len;  /* no valid padding found */
}

void printBlocks(unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % BLOCK == 0) printf(" | ");
    }
    printf("\n");
}

int main() {
    unsigned char msg1[] = "HELLOWORLD";           /* 10 bytes, not a multiple of 8 */
    unsigned char msg2[] = "EXACTLY16BYTES!!";      /* 16 bytes, ALREADY a multiple of 8 */
    unsigned char out[100];
    int n;

    printf("Message 1: \"%s\" (%zu bytes)\n", msg1, strlen((char*)msg1));
    n = addPadding(msg1, (int)strlen((char*)msg1), out);
    printf("Padded (%d bytes, %d blocks): ", n, n / BLOCK);
    printBlocks(out, n);
    printf("Unpadded length recovered: %d\n\n", removePadding(out, n));

    printf("Message 2: \"%s\" (%zu bytes -- already a multiple of block size!)\n",
           msg2, strlen((char*)msg2));
    n = addPadding(msg2, (int)strlen((char*)msg2), out);
    printf("Padded (%d bytes, %d blocks): ", n, n / BLOCK);
    printBlocks(out, n);
    printf("Notice an ENTIRE EXTRA BLOCK (80 00 00 00 00 00 00 00) was added\n");
    printf("even though the message already fit exactly -- this is intentional,\n");
    printf("so the receiver's unpadding rule never has to guess.\n");
    printf("Unpadded length recovered: %d\n", removePadding(out, n));

    return 0;
}
