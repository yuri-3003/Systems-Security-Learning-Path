/* ============================================================
 * Day 6 - Task 1: Reuse freed slots
 * ============================================================
 *
 * GOAL
 *   Evolve your Day 5 memory tracker so that freeing a block
 *   marks its slot reusable, and a later alloc reuses a free
 *   slot before consuming a fresh one.
 *
 * WHY
 *   This is the simplest possible "free list": instead of
 *   forgetting freed memory, we remember it so it can be handed
 *   back. Real allocators do exactly this - just with a linked
 *   list instead of a fixed array. Feeling this array's limits
 *   (fixed capacity, linear scan to find a free slot) is what
 *   motivates the linked list in Task 3.
 *
 * BUILD
 *   gcc -g -Wall task1_reuse_slots.c -o task1
 *   ./task1
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>

#define MAX_BLOCKS 100

typedef struct {
    void   *ptr;    /* address malloc gave us            */
    size_t  size;   /* how many bytes (size_t, not int!) */
    int     used;   /* 1 = live, 0 = free / reusable     */
} Block;

static Block tracker[MAX_BLOCKS];
static int   tracker_count = 0;   /* one past the highest slot ever used */

/* Allocate `size` bytes, record them in the tracker, return the pointer.
 *
 * TODO:
 *   1. malloc(size). If it returns NULL, handle that (return NULL).
 *   2. FIRST scan slots [0, tracker_count) for one with used == 0.
 *      If found: reuse it (set ptr, size, used = 1),
 *                print "reused slot N", return the pointer.
 *   3. Otherwise use slot `tracker_count`, then tracker_count++,
 *      print "new slot N", return the pointer.
 *      (Guard against running past MAX_BLOCKS.)
 */
void *my_alloc(size_t size) {
    /* TODO: your code here */
    (void)size;
    int i, flag;
    for (i = flag = 0; i < tracker_count; i++) {
	if (tracker[i].used == 0) {
		flag = 1;
		break;
	}
    }
    tracker[i].ptr = malloc(size);
    tracker[i].size = size;
    tracker[i].used = 1;
    if (flag == 0)
	tracker_count++;

    return tracker[i].ptr;
}

/* Free `p` and mark its tracker slot reusable.
 *
 * TODO:
 *   1. Scan the tracker for the slot whose ptr == p.
 *   2. free(p), set that slot's used = 0, print "freed slot N".
 *   3. If p isn't found, print a warning (that would mean the
 *      caller freed something we never tracked - a real bug).
 */
void my_free(void *p) {
    /* TODO: your code here */
    (void)p;
    int i;
    for (i = 0; i < tracker_count; i++) {
	if (tracker[i].ptr == p) {
		free(p);
		tracker[i].ptr = NULL;
		tracker[i].size = 0;
		tracker[i].used = 0;
	}
    }

}

int main(void) {
    /* This script should demonstrate slot reuse.
     * Expected: the 4th alloc (d) REUSES the slot freed by b,
     * instead of growing the tracker to a new slot. */
    void *a = my_alloc(10);
    void *b = my_alloc(20);
    void *c = my_alloc(30);

    my_free(b);              /* b's slot becomes reusable        */
    void *d = my_alloc(25);  /* should reuse b's slot, not grow  */

    printf("a=%p b=%p c=%p d=%p\n", a, b, c, d);

    my_free(a);
    my_free(c);
    my_free(d);
    return 0;
}
