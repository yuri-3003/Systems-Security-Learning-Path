# Day 6 — Allocator inefficiencies, metadata, and linked lists

> Goal of the day: feel *why* a fixed array can't be a real allocator, discover the
> embedded-header trick, then build and debug your first heap-linked data structure.

---

## First — two corrections from Day 5

**1. You said "a memory manager must track *allocated* blocks." That's half of it.**
A real allocator works harder tracking **freed** blocks. `free()` does **not** return
memory to the OS — it keeps the region and records "this is available again" so the next
`malloc()` can reuse it. That record is the **free list**, and it's almost always a linked
list. This single idea is the foundation of heap exploitation later. Hold onto it.

**2. Your struct used `int size`. It should be `size_t size`.**
`size_t` is the unsigned, platform-width type C uses for all sizes (`sizeof`, `malloc`,
`strlen`). `int` invites two classic bugs: negative sizes, and overflow on 64-bit when a
size exceeds `INT_MAX`. Sizes and byte counts are always `size_t`.

---

## WHY (read before touching code)

Your Day 5 tracker used a **fixed array** of blocks. Interrogate that and you invent the
linked list yourself:

- **Count is unknown at compile time** → a fixed array caps how many allocations you can
  track. Real programs do millions. You need something that grows.
- **Freeing leaves holes** → mark slot 3 free and there's a gap mid-array. To reuse it you
  must linear-scan for free slots. Slow, and still capped.
- **The fix** → chain each block to the next with a pointer. The structure grows without
  limit, and the "free list" is just the nodes currently marked free.

**The embedded-metadata trick** (how real allocators actually do it):
When you call `p = malloc(20)`, the allocator reserves `20 + sizeof(header)` bytes. It
writes a small **header** (size + a `next` pointer) at the front, then returns a pointer
*just past* the header. On `free(p)` it computes `header = (char*)p - sizeof(header)` to
find the bookkeeping. **That's why `free()` needs only the pointer, never the size — the
size is hidden in the header right before your data.** The linked list lives *inside* the
memory it manages.

That leap — from "separate array of bookkeeping" to "metadata embedded in the memory
itself" — is the heart of Day 6.

---

## Tasks

Write the bodies yourself. Each file has the spec, a runnable `main`, and build commands.

| # | File | What |
|---|------|------|
| 1 | [task1_reuse_slots.c](task1_reuse_slots.c) | Evolve the tracker: mark slots free, reuse them before growing |
| 2 | [task2_metadata.c](task2_metadata.c) | Measure the gap between two allocations — see the hidden header |
| 3 | [task3_linked_list.c](task3_linked_list.c) | Build `push_front` + `print_list` by hand |
| 4 | [gdb_walkthrough.md](gdb_walkthrough.md) | Walk the list node-by-node in gdb (RE in miniature) |
| 5 | [task5_count_free.c](task5_count_free.c) | `count_nodes` + a `free_list` that dodges the Day-4 use-after-free |
| ★ | [hard_mode_my_malloc.c](hard_mode_my_malloc.c) | **Optional / hard:** a real `my_malloc`/`my_free` over a pool (K&R-style) |

Build helper: `make` (see [Makefile](Makefile)) builds them all; `make task1` builds one.

---

## Reflection questions (answer in [report.md](report.md))

1. Why does `free()` not require you to pass the size of the block?
2. What two limitations of the array-based free list does a linked list remove?
3. In `free_list`, what exactly breaks if you don't save `next` before `free(head)`? Name the bug.
4. Why is `size_t` the right type for a size, and what breaks with `int`?
5. *(Seed for later — just think)* The `next` pointers live in heap memory you control. If an
   attacker could overwrite a freed node's `next`, what might they trick the allocator into doing?

---

## Reporting
Fill in [report.md](report.md) — include your gdb observations, the byte-gap number from
Task 2, and the reflection answers. Then tell me **"Day 6 done."** I'll review before we move on.
