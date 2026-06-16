# Day 7 — Report

Fill in as you go, then tell me **"Day 7 done."**

## What I built (check off)
- [ ] Task 1 — doubly linked list: push_front, remove_node (O(1)), print_forward, print_backward
- [ ] Task 2 — gdb: walked both directions, verified `n->next->prev == n`
- [ ] Hard mode (optional) — container_of recovers the struct from an embedded link

## Task 2 — gdb observations
- A node and its `->next->prev` (should be the same address):
  node = `0x________`   node->next->prev = `0x________`
- anything else you noticed:

## Reflection answers
1. Why O(1) removal here but O(n) in a singly linked list (starting from just the node)?
   

2. The two pointer assignments `remove_node` must make to the neighbours — and what breaks
   with wrong order / forgotten head or tail edge cases?
   

3. Why does `push_front` take `Node **head` instead of `Node *head`? (tie to Day 2)
   

4. Two reasons the kernel embeds the link inside the object (intrusive) instead of a separate
   node-holds-pointer struct?
   

5. (just a thought) Why is an attacker-controlled unlink ("write a chosen value to a chosen
   address") such a dangerous primitive?
   

## (Hard mode) container_of in my own words
What did the macro do with the address? What was `offsetof(struct task, list)`?


## Where I struggled / what felt shaky
(Be honest — this drives whether we go deeper or move on.)


## Anything that surprised me

