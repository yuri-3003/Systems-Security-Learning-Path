# Day 6 build file.  (Run in your Linux VM.)
#
# Usage:
#   make          build everything
#   make task1    build just one target
#   make clean    delete the compiled binaries
#
# CFLAGS:
#   -g     keep debug info so gdb can show source + variable names
#   -Wall  turn on all common warnings (treat warnings as clues)

CC     = gcc
CFLAGS = -g -Wall

BINS = task1 task2 task3 task5 hard

all: $(BINS)

task1: task1_reuse_slots.c
	$(CC) $(CFLAGS) $< -o $@

task2: task2_metadata.c
	$(CC) $(CFLAGS) $< -o $@

task3: task3_linked_list.c
	$(CC) $(CFLAGS) $< -o $@

task5: task5_count_free.c
	$(CC) $(CFLAGS) $< -o $@

hard: hard_mode_my_malloc.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BINS)

.PHONY: all clean
