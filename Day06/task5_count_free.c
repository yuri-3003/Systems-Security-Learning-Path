/* ============================================================
 * Day 6 - Task 5: Count nodes, and free the whole list SAFELY
 * ============================================================
 *
 * GOAL
 *   Add count_nodes() and free_list() to your linked list.
 *
 * WHY
 *   Freeing a linked list hides the Day 4 use-after-free bug in
 *   plain sight. If you free a node and THEN read its `next`, you
 *   just read freed memory. The fix - save `next` BEFORE freeing -
 *   is a pattern you'll reuse forever. Counting is good practice
 *   walking the chain.
 *
 * BUILD
 *   gcc -g -Wall task5_count_free.c -o task5
 *   ./task5
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int          value;
    struct Node *next;
} Node;

/* ---- Paste your push_front and print_list from Task 3 ---- */
Node *push_front(Node *head, int value) {
    /* TODO: copy from Task 3 */
    (void)value;
    return head;
}
void print_list(Node *head) {
    /* TODO: copy from Task 3 */
    (void)head;
}

/* Return how many nodes are in the list. Iterative (no recursion).
 * TODO. */
int count_nodes(Node *head) {
    /* TODO */
    (void)head;
    return 0;
}

/* Free every node in the list.
 *
 * THE TRAP - do NOT do this:
 *
 *     while (head) { free(head); head = head->next; }
 *
 *   After free(head), reading head->next touches freed memory:
 *   that's the use-after-free from Day 4.
 *
 * TODO: fix it by saving `next` in a temp BEFORE you free.
 */
void free_list(Node *head) {
    /* TODO */
    (void)head;
}

int main(void) {
    Node *head = NULL;
    for (int i = 1; i <= 5; i++)
        head = push_front(head, i);

    print_list(head);
    printf("count = %d (expect 5)\n", count_nodes(head));

    free_list(head);
    printf("freed the list.\n");
    return 0;
}
