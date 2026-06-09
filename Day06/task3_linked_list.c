/* ============================================================
 * Day 6 - Task 3: Build a singly linked list by hand
 * ============================================================
 *
 * GOAL
 *   Implement push_front and print_list for a singly linked list
 *   whose nodes each live on the heap.
 *
 * WHY
 *   A linked list is the first data structure that does NOT live
 *   in one contiguous block. Each node is a separate malloc, tied
 *   to the next by a pointer. This is the shape of the kernel's
 *   own lists, of malloc's free list, and of countless structures
 *   you'll meet in reverse engineering. Get comfortable walking
 *   pointers through the heap and a huge amount of systems work
 *   opens up.
 *
 * BUILD
 *   gcc -g -Wall task3_linked_list.c -o task3
 *   ./task3
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int          value;
    struct Node *next;   /* must say `struct Node` - the typedef
                            name `Node` isn't visible yet in here */
} Node;

/* Create a new node holding `value`, put it at the FRONT of the
 * list, and return the new head.
 *
 * TODO:
 *   1. malloc one Node (check for NULL).
 *   2. Store value.
 *   3. Its next = the OLD head.
 *   4. Return the new node (it's the new head).
 */
Node *push_front(Node *head, int value) {
    /* TODO */
    (void)value;
    return head;
}

/* Walk from head to the end, printing each value.
 * Aim for output like:  5 -> 4 -> 3 -> 2 -> 1 -> NULL
 *
 * TODO: loop a cursor from head until it becomes NULL.
 */
void print_list(Node *head) {
    /* TODO */
    (void)head;
}

int main(void) {
    Node *head = NULL;
    for (int i = 1; i <= 5; i++)
        head = push_front(head, i);

    print_list(head);   /* expect: 5 -> 4 -> 3 -> 2 -> 1 -> NULL */

    /* We intentionally do NOT free here - that's Task 5.
     * So yes, this program leaks for now. You'll fix it next. */
    return 0;
}
