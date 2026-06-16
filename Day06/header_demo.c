/* ============================================================
 * Day 6.5 - header_demo: see pointer vs address vs value,
 *           and the hidden malloc header.
 *
 * BUILD
 *   gcc -g -Wall header_demo.c -o header_demo
 *   ./header_demo
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *p = malloc(sizeof(int));   /* ask for 4 bytes (one int) */
    *p = 42;                        /* store a value so it's not garbage */

    printf("p          = %p   <- heap address malloc gave us (the PAYLOAD)\n", (void *)p);
    printf("&p         = %p   <- where the pointer VARIABLE p lives (the STACK)\n", (void *)&p);
    printf("*p         = %d                  <- the int stored at p\n", *p);
    printf("sizeof(p)  = %zu                   <- p is a pointer (8 bytes)\n", sizeof(p));
    printf("sizeof(*p) = %zu                   <- *p is an int (4 bytes)\n", sizeof(*p));

    free(p);                        /* <- put your gdb breakpoint on THIS line */
    return 0;
}
