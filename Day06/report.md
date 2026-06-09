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
Gap between two 1-byte allocations: `______` bytes.
Where did the extra bytes go? 

## Task 4 — gdb observations
- `sizeof(Node)` = `______`
- node addresses close / far apart: `______`
- anything else you noticed:

## Reflection answers
1. Why doesn't `free()` need the size?
   

2. Two limitations of the array free list that a linked list removes?
   

3. What exactly breaks in `free_list` if you don't save `next` first? Name the bug.
   

4. Why `size_t` and not `int` for a size?
   

5. (just a thought) If an attacker overwrote a freed node's `next` pointer…?
   

## Where I struggled / what felt shaky
(Be honest — this is what I use to decide whether we go deeper or move on.)


## Anything that surprised me

