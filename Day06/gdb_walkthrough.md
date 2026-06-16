# Day 6 — Task 4: Walk the linked list in gdb

This is reverse engineering in miniature. You'll follow pointers through the heap by
hand — exactly what you'll do in IDA later, just with friendlier output.

## Setup
Compile Task 3 **with** debug symbols (the `-g` matters), then launch gdb:

```
gcc -g -Wall task3_linked_list.c -o task3
gdb ./task3
```

## Inside gdb
```
break print_list        # stop the instant the list is fully built
run
```
Now the list exists and `head` points at it. Walk the chain:
```
print head              # address of the first node
print *head             # the whole first node: {value, next}
print head->value       # just the value (expect 5)
print head->next        # address of the second node
print *head->next       # second node (expect value 4)
print *head->next->next # third node (value 3)
```
Keep chaining `->next` until you reach the node whose `next` is `0x0`.
That `0x0` is the NULL that terminates the list.

Handy extras:
```
ptype Node              # show the struct's layout
print sizeof(Node)      # how many bytes is one node?
x/2xg head              # examine 2 giant (8-byte) words at head:
                        #   word 1 = value (+padding), word 2 = next pointer
```

## Record your observations (fill in)
- Address of `head`:          `0x555555559320`
- Address of `head->next`:    `0x555555559300`
- Are the node addresses close together or far apart? `Only 32 bytes`
  (Each node is a *separate* malloc. What does that tell you about where heap
  allocations land relative to each other?)
- `sizeof(Node)` = `16` bytes.
  An `int` (4) + a pointer (8 on 64-bit) = 12, but sizeof is probably **16**.
  Where did the extra 4 bytes go? (Hint: alignment — same reason Task 2's gap was padded.)

## The point
Each `->next` is a raw heap address. The list isn't one block — it's nodes scattered
across the heap, stitched together by pointers. Lose a `next` and you've leaked
everything after it. **Overwrite** a `next` and you redirect the walk — file that away
for the security phase.
