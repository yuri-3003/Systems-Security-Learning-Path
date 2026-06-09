/* ============================================================
 * Day 6 - HARD MODE (optional): a tiny malloc / free
 * ============================================================
 *
 * GOAL
 *   Implement my_malloc / my_free that carve memory out of ONE
 *   fixed pool, using a LINKED LIST OF CHUNKS with the metadata
 *   header stored INLINE (right before each returned pointer).
 *   This is essentially how K&R's malloc works.
 *
 * THE DESIGN
 *   The pool is one big array of bytes. We overlay chunks on it.
 *   Every chunk = [ header | payload ]. The header records the
 *   payload size, whether it's free, and a pointer to the next
 *   chunk. my_malloc walks the chunk list for a free one big
 *   enough (first-fit). my_free flips a chunk back to free.
 *
 *     pool: [hdr|payload....][hdr|payload..][hdr|payload.......]
 *            \____ chunk ____/\___ chunk ___/\_____ chunk _____/
 *
 *   Key trick: my_malloc returns a pointer to the PAYLOAD, i.e.
 *   (header + 1). my_free is handed that payload pointer and
 *   steps back one header to find the bookkeeping:
 *   ((Header*)p) - 1.  THAT is why free() never needs a size.
 *
 * BUILD
 *   gcc -g -Wall hard_mode_my_malloc.c -o hard
 *   ./hard
 *
 * Don't rush this. Getting the core working is worth a weekend.
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>

#define POOL_SIZE 4096

typedef struct Header {
    size_t         size;   /* payload bytes in this chunk     */
    int            free;   /* 1 = available, 0 = in use        */
    struct Header *next;   /* next chunk in the pool, or NULL  */
} Header;

static char   pool[POOL_SIZE];
static Header *head = NULL;   /* first chunk; NULL until init  */

/* One-time setup: make the whole pool one big free chunk.
 *
 * TODO:
 *   head = (Header *)pool;
 *   head->size = POOL_SIZE - sizeof(Header);
 *   head->free = 1;
 *   head->next = NULL;
 */
static void pool_init(void) {
    /* TODO */
}

/* Return a pointer to `size` payload bytes, or NULL if none fit.
 *
 * TODO:
 *   1. If head == NULL, call pool_init().
 *   2. Walk chunks: for (Header *c = head; c; c = c->next)
 *   3. First chunk that is free AND big enough (c->size >= size):
 *        - mark it used (c->free = 0)
 *        - (STRETCH) if it's MUCH bigger than needed, SPLIT it:
 *          place a new Header in the leftover bytes and link it in.
 *        - return the PAYLOAD pointer:  (void *)(c + 1)
 *   4. Reached the end with nothing? return NULL (out of memory).
 */
void *my_malloc(size_t size) {
    /* TODO */
    (void)size;
    return NULL;
}

/* Free a payload pointer that my_malloc returned.
 *
 * TODO:
 *   1. if (p == NULL) return;
 *   2. Step back one header:  Header *h = (Header *)p - 1;
 *   3. h->free = 1;
 *   4. (STRETCH) coalesce: if h->next is also free, merge the two
 *      so the pool doesn't fragment into uselessly small chunks.
 */
void my_free(void *p) {
    /* TODO */
    (void)p;
}

/* Debug helper - already written. Use it to watch the pool. */
static void dump_pool(void) {
    int i = 0;
    for (Header *h = head; h; h = h->next, i++)
        printf("  chunk %d @ %p  size=%zu  %s\n",
               i, (void *)h, h->size, h->free ? "FREE" : "USED");
}

int main(void) {
    char *x = my_malloc(100);
    char *y = my_malloc(200);
    printf("after two allocs:\n"); dump_pool();

    my_free(x);
    printf("after freeing x:\n"); dump_pool();

    char *z = my_malloc(50);   /* should reuse x's chunk */
    printf("after alloc z:\n"); dump_pool();

    (void)y; (void)z;
    return 0;
}
