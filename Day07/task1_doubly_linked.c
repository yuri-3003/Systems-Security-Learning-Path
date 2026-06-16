/* ============================================================
 * Day 7 - Task 1: Doubly linked list + O(1) removal
 * ============================================================
 *
 * GOAL
 *   A linked list where every node knows BOTH neighbours, so any
 *   node can be removed in O(1) given only a pointer to it.
 *
 * WHY
 *   The `prev` pointer is what lets you splice a node out without
 *   walking the list to find its predecessor. That O(1) removal is
 *   why schedulers, caches, and kernels use doubly linked lists.
 *
 * BUILD
 *   gcc -g -Wall task1_doubly_linked.c -o task1
 *   ./task1
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int          value;
    struct Node *prev;
    struct Node *next;
} Node;

/* Insert a new node at the front.
 *
 * Takes Node ** (a pointer to the caller's head pointer) so it can
 * UPDATE the head - same pass-by-address idea as Day 2.
 *
 * TODO:
 *   1. malloc a node (check NULL), set value.
 *   2. node->prev = NULL;  node->next = *head;
 *   3. if (*head) (*head)->prev = node;   // old head now points back
 *   4. *head = node;
 */
void push_front(Node **head, int value) {
    /* TODO */
    (void)head; (void)value;
}

/* Unlink node `n` from the list in O(1) and free it.
 *
 * TODO:
 *   1. Fix the LEFT neighbour:
 *        if (n->prev) n->prev->next = n->next;
 *        else         *head = n->next;        // n was the head
 *   2. Fix the RIGHT neighbour:
 *        if (n->next) n->next->prev = n->prev;
 *   3. free(n);
 */
void remove_node(Node **head, Node *n) {
    /* TODO */
    (void)head; (void)n;
}

/* Walk via ->next, printing forward. */
void print_forward(Node *head) {
    /* TODO */
    (void)head;
}

/* Walk via ->prev from the tail, printing backward.
 * (Find the tail first, or pass it in - your choice.) */
void print_backward(Node *head) {
    /* TODO */
    (void)head;
}

int main(void) {
    Node *head = NULL;
    for (int i = 1; i <= 5; i++)
        push_front(&head, i);

    print_forward(head);    /* expect: 5 4 3 2 1 */

    /* Find the node holding 3 and remove it in O(1). */
    Node *n = head;
    while (n && n->value != 3)
        n = n->next;
    if (n)
        remove_node(&head, n);

    print_forward(head);    /* expect: 5 4 2 1 */
    print_backward(head);   /* expect: 1 2 4 5  (same list, other way) */

    /* (Leaks the rest - add a free loop if you want; mind the
     *  Day 6 save-next-before-free rule.) */
    return 0;
}
