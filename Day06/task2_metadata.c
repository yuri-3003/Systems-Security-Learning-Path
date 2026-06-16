/* ============================================================
 * Day 6 - Task 2: See the allocator's hidden metadata
 * ============================================================
 *
 * GOAL
 *   Prove to yourself that malloc reserves MORE than you ask for.
 *
 * WHY
 *   malloc writes a small header (size + bookkeeping) right
 *   before the pointer it returns, and rounds sizes up for
 *   alignment. That hidden header is why free() needs only the
 *   pointer and never the size. You can't read the header
 *   portably (it's implementation-private), but you CAN measure
 *   the gap between two allocations and see it's bigger than you
 *   asked for.
 *
 * BUILD
 *   gcc -g -Wall task2_metadata.c -o task2
 *   ./task2
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char *a = malloc(5);   /* ask for ONE byte */
    char *b = malloc(5);   /* ask for ONE byte */

    if (!a || !b) { perror("malloc"); return 1; }

    printf("a = %p, value=%p\n", (void *)a, a);
    printf("b = %p, value=%p\n", (void *)b, b);

    /* TODO: compute the distance in BYTES between b and a, and print it.
     *
     *   Hint: subtract the two char* pointers.
     *   Question for your report: why char* and not int* or void*?
     *   (What does pointer subtraction measure for each type?)
     *
     *   long gap = ... ;
     *   printf("gap between two 1-byte allocations = %ld bytes\n", gap);
     */
    long gap = b - a;
    printf("gap between two 1-byte allocations = %ld bytes\n", gap);

    /* You asked for 1 byte; the gap is much larger. Where did the
     * extra bytes go? Write the number down in report.md. */

    free(a);
    free(b);
    return 0;
}
