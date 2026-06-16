# Day 6 — Mentor review

Verdict up front: **your code is strong, your concepts have a few real gaps, and one of
those gaps is foundational enough that we'll do a short consolidation (Day 6.5) before
Day 7.** This is the "go deeper, don't blindly advance" call — and it's the right one,
because the thing you're fuzzy on is *the* core idea of this whole path: pointer vs.
address vs. value.

---

## What you did well (credit where it's due)

- **Task 1 `my_alloc` is genuinely clever.** Letting the loop fall through so that `i`
  naturally lands on `tracker_count` when no free slot is found — and reusing the same
  write path for both cases — is elegant. Most people write two branches. You found one. 👏
- **Task 5 `free_list` is correct** — you saved `next` *before* `free(head)`. That was the
  whole trap and you walked straight past it. This is the most important line of the day.
- **Task 2 pointer subtraction** (`b - a` on `char*`) is exactly right, and you connected the
  32-byte gap across Task 2 and Task 4 (gdb) on your own. That cross-check is good instinct.

---

## Bugs to fix (you fix them — don't ask me to)

**1. `count_nodes` returns the wrong thing.** [task5_count_free.c](task5_count_free.c), last line of the function:
```c
    return 0;     // <-- you counted into `cnt`, then threw it away
```
Did you *run* task5? The `main` prints `count = %d (expect 5)`. With this bug it prints
`count = 0 (expect 5)` — the harness is literally telling you it's wrong. **Lesson: always
compile, run, and read the output against the `expect` comments. Code that isn't run is a
guess.** Fix: `return cnt;`.

**2. `my_alloc` has no bounds check — this is a buffer overflow.** [task1_reuse_slots.c](task1_reuse_slots.c):
when every slot is in use and `tracker_count == MAX_BLOCKS`, your loop finishes with
`flag == 0` and `i == 100`, then you write `tracker[100]` — one past the end of a 100-element
array. On your learning path this *is* the lesson: an unchecked index is how buffer overflows
happen. Add: if `flag == 0 && tracker_count >= MAX_BLOCKS`, return `NULL`.

**3. Missing the observability prints.** The spec asked `my_alloc`/`my_free` to print
`"reused slot N"` / `"new slot N"` / `"freed slot N"`. Without them you can't actually *see*
the reuse happen — and that was the entire point of Task 1. Add them and re-run; watch the
4th alloc reuse slot 1.

(Minor: leftover `(void)size;` / `(void)head;` lines are harmless no-ops now that the params
are used — delete them. `my_free` should also `break;` after the match and warn if `p` isn't
found.)

---

## The big concept fix: pointer vs. address vs. value vs. chunk size

Your report asked, almost verbatim:
> "`int *p = malloc(sizeof(int));` then I print `%p` with `&p` — that's the address of `p`,
> right? And its size is 32 bytes? Also `*p` is 32 bytes?"

This is **four different things tangled into one**, and untangling them is the most important
thing you'll do this week. Here is the whole picture for `int *p = malloc(sizeof(int));`:

```
   STACK                                  HEAP
  ┌───────────────┐                      ┌──────────────────────────────────┐
  │ p             │                      │ [ chunk header ][ your int (4B) ] │
  │ value: 0x...c0│ ───────────────────► │                  ^               │
  │ &p:    0x...08│                      │                  p points here   │
  └───────────────┘                      └──────────────────────────────────┘
```

| Expression   | What it is                                    | Size on 64-bit |
|--------------|-----------------------------------------------|----------------|
| `&p`         | address *of the pointer variable* (on the stack) | n/a (it's an address) |
| `p`          | the value *stored in* p = the heap address malloc returned | `sizeof(p)` = **8** bytes |
| `*p`         | the `int` living at that heap address          | `sizeof(*p)` = **4** bytes |
| the chunk    | what the allocator reserved (header + alignment) | ~**32** bytes |

So, point by point:
- `&p` is **not** the allocated memory. It's where the *pointer itself* lives, on the stack.
  `p` and `&p` are two different addresses.
- **Nothing here is "32 bytes" in the way you meant.** The pointer is 8 bytes. The int is
  4 bytes. The *32* is the allocator's minimum **chunk** size — overhead, not the size of your
  data. Don't attach "32" to the int or the pointer; it belongs to the heap chunk.

**Also a direct correction to Reflection #1:** you wrote "malloc's return value is the header
address." It's the opposite — **malloc returns the address just *past* the header (the
payload)**, and `free()` steps *backward* from your pointer to find the header. You'll prove
this to yourself in Day 6.5 below.

---

## Your other two good questions

**"Why `(void *)a` instead of `a` in `printf("%p", ...)`?"**
`%p` is defined to take a `void *`. Your `a` is a `char *`. On normal machines they have the
same representation so it "works," but per the C standard passing the wrong pointer type to a
variadic function like `printf` is **undefined behavior**. The cast makes it correct, not just
lucky. (Habit worth keeping: make the types *say* what's true.)

**"If I had two locals `int a, b;`, would their gap also be ~32?"**
No — and this is a great instinct to test. Those live on the **stack**, which has *no malloc
header and no chunk rounding*. Their gap is typically just 4 bytes (often adjacent), the
compiler may reorder them, and since the stack grows downward the difference can even be
negative. **The 32-byte gap is a heap-allocator artifact, not a law of memory.** Stack and
heap are laid out by completely different machinery. Write a 5-line program with two stack
ints and two heap ints, print all four addresses, and see the difference for yourself.

---

## Day 6.5 — Consolidation task (do this before Day 7)

Your report literally asked: *"is there any way to see that header structure?"* **Yes. Let's
go see it.** This single experiment nails the pointer model AND the metadata idea AND explains
your 32-byte gap — all at once.

In `task2_metadata.c`, build with `-g` and open gdb:
```
gcc -g -Wall task2_metadata.c -o task2
gdb ./task2
break 44            # the line after both mallocs (adjust if needed)
run

print a             # the heap PAYLOAD address malloc gave you
print &a            # address of the pointer VARIABLE a — a DIFFERENT, stack address
print sizeof(a)     # 8  (it's a pointer)

# Now look at the bytes RIGHT BEFORE your pointer — the hidden header:
x/2xg (char*)a - 16
```
That last command prints two 8-byte words: `prev_size` then the chunk's **`size` field**.
For your `malloc(5)` you should see the size word is **`0x21`**. Decode it:
- `0x21` = 33 = `0b100001`
- the low 3 bits are flags (the `1` = "previous chunk in use")
- mask them off → `0x20` = **32** → *that's your gap.* The allocator wrote "this chunk is
  32 bytes" into the header, and that header sits 8 bytes before the pointer it handed you.

Record in your report: the value of the size word, and confirm it decodes to 32. When you've
seen `0x21` with your own eyes, you'll understand `free()` forever: it reads that word to know
how big the block was — which is exactly why it doesn't need a size argument.

> ⚠️ Reading another allocator's private header bytes like this is fine *for learning in gdb*,
> but real code must never poke at them — the layout is glibc-specific and undocumented. We're
> doing it to demystify, not to rely on it.

---

## Quick scores on the reflection answers
1. free()/no size — **half right.** Right that size is in the header; wrong that malloc returns
   the header. (Corrected above.)
2. array vs linked list — **partial.** You named the linear-scan cost, but a linked free list
   *also* gets scanned, so that's not what it fixes. The real wins: (a) **no fixed capacity** —
   it grows without a `MAX_BLOCKS` ceiling, and (b) the bookkeeping lives **inline** in the
   memory, so there's no separate tracker array at all.
3. free_list bug — **you didn't name it.** The name is **use-after-free**: after `free(head)`,
   reading `head->next` reads memory you no longer own. (Your *code* avoided it — make sure your
   *words* can too.)
4. size_t — **good.** Unsigned (no negatives). Add: it's also wide enough not to overflow on
   64-bit the way a 32-bit `int` would past 2 GB.
5. attacker overwrites `next` — **right idea, vague.** Sharpen it: if you control a `next`
   pointer, you control where the next traversal (or, in a real allocator, the next handed-out
   chunk) *points* — i.e. you can make the program read or write memory of your choosing. That
   is the seed of heap exploitation (freelist / tcache poisoning). File it away.

---

## Decision
**Hold at Day 6.** Fix the three code issues, do the Day 6.5 gdb experiment, and update
[report.md](report.md) with the `0x21` finding and a one-line restatement of the `&p` / `p` /
`*p` distinction in your own words. Send **"Day 6.5 done"** and if the pointer model is solid,
we move to **Day 7** (where these linked nodes start becoming the structures the kernel itself
runs on).
