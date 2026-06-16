/* ============================================================
 * Day 7 - HARD MODE: intrusive lists + container_of
 * ============================================================
 *
 * GOAL
 *   Recover a pointer to a containing struct from a pointer to a
 *   member embedded inside it. This is the heart of how the Linux
 *   kernel's <linux/list.h> works.
 *
 * THE IDEA
 *   Instead of "a node that holds your data", you EMBED a tiny link
 *   struct INSIDE your data struct. The list threads through your
 *   objects directly. Given only a pointer to the embedded link, you
 *   find the object by subtracting the member's offset within the
 *   struct - that's what container_of does.
 *
 *       struct task { int pid; struct list_head list; };
 *
 *       &task ──► [ pid | list.prev | list.next ]
 *                          ▲
 *                          └─ a list_head* points HERE; subtract
 *                             offsetof(struct task, list) to get &task
 *
 * BUILD
 *   gcc -g -Wall hard_mode_intrusive_list.c -o hard
 *   ./hard
 * ============================================================ */

#include <stdio.h>
#include <stddef.h>   /* offsetof */

struct list_head {
    struct list_head *prev;
    struct list_head *next;
};

/* Given a pointer to a MEMBER, return a pointer to the CONTAINING struct.
 *   ptr    - pointer to the member
 *   type   - type of the containing struct
 *   member - name of the member within that struct
 *
 * It works by: take the member's address as raw bytes, step back by the
 * member's offset inside the struct, and reinterpret that as the struct. */
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

struct task {
    int              pid;
    struct list_head list;    /* the link lives INSIDE the object */
};

int main(void) {
    struct task t = { .pid = 1234 };
    struct list_head *lp = &t.list;   /* pretend this link is all we have */

    /* TODO:
     *   1. Use container_of to recover a `struct task *` from `lp`.
     *   2. Print its pid - expect 1234.
     *
     *      struct task *recovered = container_of(lp, struct task, list);
     *      printf("pid = %d\n", recovered->pid);
     *
     * Then in your report, explain in your own words what the macro did
     * with the address. (Hint: what is offsetof(struct task, list)?
     * Print it: printf("offset = %zu\n", offsetof(struct task, list)); )
     */
    (void)lp;
    return 0;
}
