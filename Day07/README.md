# Day 7 — Doubly linked lists, and how the kernel actually does lists

> You can build and walk a singly linked list now. Today: add a second pointer, unlock
> O(1) removal of *any* node, and meet the exact idiom the Linux kernel uses for every
> list it keeps. This is where "a linked list" turns into "the data structure an OS runs on."

---

## WHY (read first)

Your Day 6 list was **singly** linked: each node knew only its `next`. That has a sharp
limitation — to *remove* a node you need its predecessor, but a node can't look backward, so
you must walk from the head to find "the node before this one." Removal is **O(n)**.

Add a `prev` pointer and each node knows both neighbours. Now, given just a pointer to a node,
you can splice it out by rewiring its two neighbours to point past it — **O(1)**, no search:

```
... ⇄ [A] ⇄ [B] ⇄ [C] ⇄ ...      remove B:
                                  A->next = C;  C->prev = A;   (B is now unlinked)
```

Why does an OS care? Schedulers, LRU caches, timer queues, the list of running processes —
all need to yank an arbitrary element out *right now*, by pointer, without scanning. That's
doubly linked lists, usually circular.

**The kernel's twist — intrusive lists (hard mode).** Instead of "a node that holds a pointer
to your data," the kernel **embeds** a tiny `{prev, next}` link *inside* your data struct, and
the list threads through your objects. To get from a link back to the object containing it, it
uses a macro called `container_of` (pointer arithmetic on the struct's layout). One allocation
instead of two, better cache locality, and one object can sit on several lists at once. You'll
build a miniature of this today.

---

## Tasks

Build in your VM, `gcc -g -Wall`. Bodies are yours to write.

| # | File | What |
|---|------|------|
| 1 | [task1_doubly_linked.c](task1_doubly_linked.c) | `push_front`, `remove_node` (O(1)), `print_forward`, `print_backward` |
| 2 | (gdb, below) | Walk the list both directions; verify the back-pointers line up |
| ★ | [hard_mode_intrusive_list.c](hard_mode_intrusive_list.c) | The kernel's `container_of` trick — recover an object from an embedded link |

### Task 2 — gdb: prove the symmetry
After building the list, in gdb:
```
break print_forward
run
print *head                       # {value, prev, next}
print head->next->prev            # should equal head  (the back-pointer is correct)
print *head->next->next           # step forward two
```
The invariant to *see* with your own eyes: for any node `n`, `n->next->prev == n`. If that ever
breaks, your list is corrupt. (Record one such pair of addresses in your report.)

---

## Reflection questions (answer in [report.md](report.md))

1. Why is removing a node O(1) in a doubly linked list, but O(n) in a singly linked one when you
   start from just the node?
2. After `remove_node(n)`, write the two pointer assignments that must happen to its neighbours.
   What goes wrong if you do them in the wrong order, or forget the head/tail edge cases?
3. Why does `push_front` take a `Node **head` (pointer to the head pointer) instead of `Node *head`?
   (Tie it back to pass-by-address from Day 2.)
4. Why would the kernel *embed* the list link inside the object (intrusive) rather than allocate a
   separate "node holds a pointer to data" struct? Give two reasons.
5. *(Security seed — just think)* "Unlinking" a node writes `n->next` into `*(n->prev + offset)`.
   If an attacker controls a node's `prev` and `next`, an unlink becomes "write a value I choose to
   an address I choose." Why is that one of the most dangerous primitives in exploitation?

---

## Reporting
Fill in [report.md](report.md), then send **"Day 7 done."** Quick 5-minute warm-up first if you
like: re-run the Day 6.5 header peek (`x/2xg (char*)a - 16`) now that you know how to read it —
confirm you see `0x21` and that it decodes to 32. Then dive into Day 7.
