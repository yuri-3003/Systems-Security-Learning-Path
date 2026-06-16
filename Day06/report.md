# Day 6 — Report

Fill this in as you go. When you're done, tell me **"Day 6 done"** and that it's in here —
I'll read it and review critically before we advance.

## What I built (check off)
- [ ] Task 1 — slot reuse (does the 4th alloc reuse the freed slot, not grow?)
- [ ] Task 2 — metadata gap experiment
- [ ] Task 3 — linked list (push_front + print_list)
- [ ] Task 4 — gdb walkthrough
- [ ] Task 5 — count_nodes + safe free_list
- [ ] Hard mode (optional) — my_malloc / my_free

## Task 2 — the number
Gap between two 1-byte allocations: `32` bytes.
Where did the extra bytes go? 
Address of a was 0x5a7c14eec2a0 and b was c0 at the end.
So total 32 bytes but I have a questiosn why do we need (void *)a instead of a when print.
And also imagine I defined two int a, b; and if we track those address then gap between those two addresses might be 32?
## Task 4 — gdb observations
- `sizeof(Node)` = `16 bytes`
- node addresses close / far apart: `______`
- anything else you noticed:

## Reflection answers
1. Why doesn't `free()` need the size?
   As you said when we malloc it sets the header for that and stores size all information in the header so when we clean we just need to clean that header. And return value is the that header address.

2. Two limitations of the array free list that a linked list removes?
   When we free the linked list, we free all of them but I think that's not efficient and we need to reuse the freed memory and there's no code for that. Also when we try to free one of that linked list then we nearly have to map all items at worst case.

3. What exactly breaks in `free_list` if you don't save `next` first? Name the bug.
   If we don't save the 'next' first then we lost the track of linked list because we free the head Node so it means header of head Node is removed and don't track memory anymore.

4. Why `size_t` and not `int` for a size?
   Because int means it can store negative value but it can't be negative so that can cause an issue. I think size_t is the type for the size.

5. (just a thought) If an attacker overwrote a freed node's `next` pointer…?
   As I said if we free the node then we lose the header of that node and header place will be different with the original one so it points wrong data. I can cause issue.

## Where I struggled / what felt shaky
(Be honest — this is what I use to decide whether we go deeper or move on.)
Here's my think. When we store the variable, we use some kind of header structure to store it. And that size is 32 bytes(in my case) and when we try to use that value then the system automatically reads that header info and get the information so my idea is, is there anyway to see that header structure? And also int *p = malloc(sizeof(int)); then we print %p with &p, this means the address of p, right? And it's size is 32 bytes? Also *p is 32 bytes?

## Anything that surprised me

