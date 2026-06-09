# Tooling — editor & toolchain (Linux VM)

> The short version: **the editor is secondary; the toolchain (`gcc` / `make` / `gdb`)
> is the thing.** Pick an editor that keeps the terminal in front of you, not one that
> hides it. Don't spend days configuring — spend them writing C.

---

## The toolchain (install this first — it matters more than the editor)
Debian/Ubuntu (adjust for your distro):
```
sudo apt update
sudo apt install build-essential gdb git make
```
- `build-essential` → `gcc`, `g++`, `make`, and the libc headers.
- `gdb` → the debugger you'll basically live in.
- You already build with `gcc -g -Wall file.c -o out`. `make` automates that (see the Day06 Makefile).

---

## Editor recommendation

### Primary: Neovim + the terminal (the "hard path", and the right one)
Why this fits *your* goals specifically:
- **Your path ends on machines with no GUI** — remote servers, CTF boxes, embedded
  targets. The terminal is their native habitat. The editor that's *always already there*
  on every Linux box is `vi`/`vim`. Learning it now is an investment that never stops paying.
- It **forces** you to drive `gcc`/`make`/`gdb` yourself instead of clicking a button you
  don't understand. That friction is the learning.
- It's the hard path you explicitly asked for, and it compounds.

Setup:
```
sudo apt install neovim
vimtutor          # <-- do this FIRST. 25 minutes. Non-negotiable.
```
Keep the config **minimal** at the start. Stock Neovim + `vimtutor` muscle memory is plenty
for writing C. Resist the urge to install 30 plugins — that's procrastination disguised as
productivity. Add an LSP (`clangd`) for autocomplete *later*, once the motions are automatic.

### Pragmatic fallback: VS Code
If Vim friction is actively stealing focus from learning C in these first weeks, use VS Code
**inside the VM** (or via Remote-SSH from your host) with the Microsoft **C/C++** extension.
It has a clean visual gdb frontend.

**But make one rule if you go this way:** learn to *build and debug from the command line
first*, then let the GUI be a convenience — never a crutch. If you can't do it in a bare
terminal, you don't understand it yet.

> Verdict: start in whichever lets you keep momentum, but plan to be comfortable in Neovim
> by the time you hit the reverse-engineering phase. You will not regret it.

---

## gdb: don't sleep on this
gdb has a built-in split-screen source view — use it from day one:
```
gdb -tui ./prog        # or, inside gdb:  layout src
```
You'll see your C source, current line, and breakpoints while you step. Massive upgrade
over plain gdb for learning.

(Later, in the security/RE phase, you'll supercharge gdb with **pwndbg** or **GEF** —
register/stack/heap views built for exploitation. Don't install them yet; stock gdb +
`-tui` is the right tool for learning C.)

---

## Reverse-engineering tools — LATER (Phase 3+), don't install now
Noting these so you know what's coming, not so you go install them today:
- **Ghidra** — free, full-featured disassembler/decompiler (NSA). Your main RE workhorse.
- **radare2 / rizin** — terminal-native RE framework. Fits the Neovim philosophy.
- **IDA** (you've touched it) — industry standard; the free version is fine to keep around.
- **pwndbg / GEF** — gdb plugins for exploitation.

Installing these now would just be noise. They belong to a phase where you'll actually use them.

---

## The trap to avoid
The single most common way beginners stall here: **spending a weekend perfecting an editor
config instead of learning the subject.** A blinking cursor in stock Neovim and a working
`gcc` is a complete environment. Tune later, build now.

---

## Continuing the mentorship from inside the VM

**Key idea:** the chat transcript lives on the Windows host and does *not* travel. But the
mentorship's real state does — it's in two places, on purpose:
- the `Learning Path/` files (this folder), and
- the memory files Claude keeps.

Give any Claude session access to those and it picks up exactly where we are.
**Continuity = the files, not the chat window.**

### Option A — simplest: compile in the VM, chat with Claude on Windows
Share this folder into the VM (steps below), do your `gcc`/`gdb` work in Linux, and alt-tab
to the existing Claude window on Windows to report "Day X done". Zero new setup, memory stays
intact. Only downside: two windows.

### Option B — Claude Code running inside the VM (nicer once you're settled)
1. Get the files into the VM (shared folder or git, below).
2. Install Node 18+ and Claude Code:
   ```
   npm install -g @anthropic-ai/claude-code
   ```
   Then run `claude` in the project directory and authenticate on first launch.
3. First launch, tell it:
   > "You're my long-term systems/security mentor — read Learning Path/systems_journey.md
   > and tooling.md, then continue from Day 6."

   It rebuilds full context from the journey log and re-saves its memory on the Linux side.
   (Memory doesn't auto-copy between machines, but since it mirrors the journey file,
   re-reading restores it.)

### Sharing `E:\Projects\Assist` into the VM (VMware)
1. VM powered **off** → VM Settings → Options → **Shared Folders** → enable → add
   `E:\Projects\Assist`.
2. In Linux, install the guest tools (Debian/Ubuntu):
   ```
   sudo apt install open-vm-tools open-vm-tools-desktop
   ```
3. The share shows up under `/mnt/hgfs/Assist`. If it's empty, mount it manually:
   ```
   sudo vmhgfs-fuse .host:/ /mnt/hgfs -o allow_other
   ```
4. **Alternative — git** (you already know it): push from Windows, `git clone` / `git pull`
   in the VM. Live shared folders are better for rapid back-and-forth; git is better as a
   backup and history.

> Recommendation: start with **Option A** (no setup, nothing can break), and graduate to
> **Option B** once you're comfortable living in the Linux terminal. Either way, set up the
> shared folder first — both options want it.
