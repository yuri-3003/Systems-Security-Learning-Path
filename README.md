# Systems & Security — Learning Path

Long-term path: **Systems Engineer → Security Engineer → Reverse Engineer.**
Mentor-driven. Deep understanding over breadth. WHY before HOW.

## How this works
- Each day gets its own folder (`Day06/`, `Day07/`, …) with a `README.md` plan,
  skeleton `.c` files (you write the bodies), and a `report.md` to fill in.
- Days 1–5 were done before this folder existed — they're summarized in
  [systems_journey.md](systems_journey.md) (the master log).
- When you finish a day: fill in that day's `report.md`, then tell me **"Day X done"**.
  I review it critically, correct misunderstandings, and decide whether we go deeper
  or advance. I do **not** blindly advance.

## Where things are
| What | Where |
|------|-------|
| Master journey log (background, goals, roadmap, day-by-day record) | [systems_journey.md](systems_journey.md) |
| Editor & toolchain setup | [tooling.md](tooling.md) |
| Current day | [Day06/](Day06/README.md) |

## Roadmap
- **Phase 1** — Linux · C · Memory · Debugging  ← *we are here*
- **Phase 2** — Operating Systems · Networking · System Calls
- **Phase 3** — Reverse Engineering · Assembly · Security Fundamentals
- **Phase 4** — Advanced Systems · Exploitation · Binary Analysis

## Build environment
Write and compile the C in your Linux VM. Standard build for everything:
```
gcc -g -Wall file.c -o out
./out
```
`-g` = keep debug symbols (gdb needs them). `-Wall` = show all warnings (treat them as clues, not noise).
