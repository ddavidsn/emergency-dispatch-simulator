# Emergency Dispatch Simulator

A command-driven emergency incident dispatch simulation written in **C**. It assigns available response units to incidents by severity, keeps track of interventions, and supports undoing the latest unfinished dispatch.

> **Context:** Developed for a university Data Structures course. This repository contains my implementation, not the university's assignment PDF, grading scripts, or supplied test suite. Some error-handling macros are based on a Devmind laboratory example, as credited in the source code.

## What it does

- Registers incidents with **high**, **medium**, or **low** priority.
- Dispatches available response units to queued incidents, serving higher-priority incidents first.
- Tracks incident and unit status, including completed interventions.
- Undoes the most recent unfinished dispatch, restoring the unit and incident to their queues.
- Answers commands that display incident, unit, and intervention information.

## Data structures

| Structure | Purpose |
| --- | --- |
| Three linked queues | Pending incidents, separated by severity |
| Linked queue | Currently available response units |
| Stack | Dispatch history for `UNDO_LAST_DISPATCH` |
| Sentinel-based circular doubly linked lists | Incident and intervention records |
| Array of records | Response-unit metadata |

The project is split into implementation modules for dispatch commands, core structures, and the linked queue/stack operations.

## Build and run

Requires a C11 compiler (GCC) and `make`. The program reads **`tema1.in`** from the working directory and writes **`tema1.out`**.

```bash
make build
cp examples/demo.in tema1.in
./tema1
cat tema1.out
```

On Windows, run the commands in WSL or a Unix-like environment with GCC and Make. You can also use `copy examples\demo.in tema1.in` in Windows Command Prompt, and then run the executable built by your installed C toolchain.

See [`examples/demo.in`](examples/demo.in) and [`examples/demo.expected.txt`](examples/demo.expected.txt) for a small example with priority handling and undo.

## Commands

`ADD_INCIDENT`, `CHECK_UNITS`, `DISPATCH`, `UNDO_LAST_DISPATCH`, `SOLVED_INCIDENT`, `SHOW_UNIT`, `SHOW_INCIDENT`, `SHOW_INTERVENTIONS`.

Commands and response units are specified in the input file, following the course assignment format. The first line is the number of units; after those unit records comes the number of commands.

## Files

- `main.c` — opens input/output files, initializes and destroys the system.
- `functions.c/.h` — command handling and dispatch logic.
- `structures.c/.h` — incident/intervention structures and memory management.
- `queuesnstacks.c/.h` — linked queue and stack implementations.
- `Makefile` — build, run, and clean targets.
- `examples/` — newly written demonstration input and its expected output.

## Verification and scope

The unchanged implementation **compiled with GCC** and matched **20 of 20** supplied course reference-output tests in a local review. The course tests are not redistributed here. This does not guarantee correctness for all inputs or constitute a memory-safety audit.

Before publishing publicly, confirm your course's policy on sharing assignment solutions. This is an educational simulation, **not a real emergency-response system**.
