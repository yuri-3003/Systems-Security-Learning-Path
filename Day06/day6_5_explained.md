# Day 6.5 — explained slowly (pointer vs address vs value, and the malloc header)

Being confused here is normal — this is the hardest idea in early systems programming.
Read this once, run `header_demo.c`, then read it again. It will click.

---

## Part 1 — the only mental model you need

Take this one line:
```c
int *p = malloc(sizeof(int));   // sizeof(int) is 4
```
This creates **two separate pieces of memory in two different places**:

```
   STACK  (where local variables live)         HEAP  (where malloc hands out memory)
  ┌────────────────────────────────┐          ┌──────────────────────────────────────┐
  │ p   (a variable, 8 bytes)       │          │ ...[ header (8B) ][ your int (4B) ]...  │
  │ it HOLDS the value 0x...2a0  ───┼────────► │                    ▲                   │
  │ p itself LIVES at 0x...e098     │          │                    p points HERE       │
  └────────────────────────────────┘          └──────────────────────────────────────┘
```

Now four expressions, four *different* things:

| You write | It means | Example value | Size |
|-----------|----------|---------------|------|
| `p`   | the **value** inside p = the heap address malloc returned | `0x...2a0` | `sizeof(p)` = **8** (it's a pointer) |
| `&p`  | the **address of the variable p itself** (on the stack)   | `0x...e098` | — |
| `*p`  | the **int** sitting at that heap address                  | `42`       | `sizeof(*p)` = **4** (an int) |
| header| allocator bookkeeping, 8 bytes **before** `p`             | `0x21`     | the "chunk" is ~32 bytes total |

**The three mistakes to unlearn (all from your report):**

1. *"`&p` is the address of the allocated memory."* — **No.** `&p` is where the *pointer
   variable* lives, on the stack. The allocated memory is at `p` (the value), on the heap.
   `p` and `&p` are two unrelated addresses.
2. *"its size is 32 bytes / `*p` is 32 bytes."* — **No.** The pointer is 8 bytes, the int is
   4 bytes. The **32** is the allocator's *chunk* size (overhead + rounding). It is not the
   size of your pointer or your int. Don't attach "32" to `p` or `*p`.
3. *"malloc returns the header address."* — **No** (you already fixed this — keeping it here
   so it stays fixed). malloc returns the **payload** address. The header sits 8 bytes *before*
   it. `free(p)` steps backward to read that header.

If you remember only one sentence: **`p` is what it points to, `&p` is where it lives, `*p` is
what's there — and the header is the allocator's note, hidden just before `p`.**

---

## Part 2 — prove it with no gdb at all

Compile and run `header_demo.c`:
```
gcc -g -Wall header_demo.c -o header_demo
./header_demo
```
You'll see something like:
```
p          = 0x5555555592a0   <- heap address (payload)
&p         = 0x7fffffffe098   <- the variable p lives here (stack) - TOTALLY different
*p         = 42
sizeof(p)  = 8
sizeof(*p) = 4
```
Stop and notice: **`p` and `&p` are completely different addresses.** One is a heap address
(starts `0x5555...`), the other a stack address (starts `0x7fff...`). That alone kills the
"&p is the allocated memory" confusion. And `sizeof` confirms: pointer 8, int 4. No 32 anywhere.

That's the whole concept. Everything below is just *seeing the header* to make "free needs no
size" concrete. If gdb fights you, you've still understood Day 6.5 from the part above.

---

## Part 3 — see the hidden header in gdb (the `0x21`)

```
gdb ./header_demo
```
Inside gdb:
```
break 18          # the free(p) line, so p is still valid. Run `list` if your number differs.
run
```
Now p is alive. Look at the three entities, then the header:
```
print p                    # the payload pointer, e.g.  (int *) 0x5555555592a0
print &p                   # the variable's address,   e.g. (int **) 0x7fffffffe098   (different!)
print *p                   # 42

x/xg (char *)p - 8         # show 1 giant (8-byte) word, 8 bytes BEFORE p: the size field
```
You should see exactly this kind of line:
```
0x555555559298:  0x0000000000000021
```
That `0x21` is the malloc **size field** — the header. Notice the address `...298` is exactly
`p (...2a0)` minus 8. The header really does sit right before your pointer.

### Decoding `0x21` (this is the payoff)
`0x21` in binary is `0010 0001`. glibc steals the **low 3 bits** of the size field for flags:
```
   0010 0001
   ^^^^^ ^^^
   │     └── low 3 bits = flags. Here only bit 0 is set:
   │          bit 0 = "previous chunk is in use"  (PREV_INUSE)
   └──────── the rest, with the low 3 bits cleared, is the CHUNK SIZE
```
Clear the low 3 bits: `0x21 & ~0x7` = `0x20` = **32**.

So the header says: *"this chunk is **32 bytes**, and the previous chunk is in use."* That **32**
is exactly the gap you measured between two allocations in Task 2. The allocator wrote the size
into a header right before your data — which is why `free(p)` only needs `p`: it reads the size
back out of `(char*)p - 8`. **That is the entire reason free() takes no size argument.**

(Optional: `x/2xg (char *)p - 16` shows two words — the first is `prev_size`, usually `0` and
not interesting right now; the second is the same `0x21`. Stick with the single-word version if
two columns is confusing.)

---

## Part 4 — the two things that look weird

**Why `(char *)p` and not just `p`?**
Pointer arithmetic moves in units of the pointed-to type. `p` is an `int *`, so `p - 8` would
move back `8 × sizeof(int)` = **32 bytes** — too far. Casting to `char *` makes each step **1
byte** (because `sizeof(char) == 1`), so `(char *)p - 8` moves back exactly 8 bytes to the size
field. Always cast to `char *` when you want to move a precise number of bytes.

**Why `- 8` (and `- 16`)?**
The size field lives in the 8 bytes immediately before the payload → `p - 8`. The word before
*that* (`p - 16`) is `prev_size`. We subtract 8 to land on the field that matters.

---

## What to put in report.md
- The size-field value you saw (should be `0x21`) and that it decodes to **32 + the in-use bit**.
- One line, in your own words: what `p`, `&p`, and `*p` each are.

Once that sentence is yours, you're done with Day 6.5 — and Day 7 is waiting.
