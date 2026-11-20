# Automated Cargo Terminal Simulator

Concurrent cargo terminal simulation in C using POSIX threads, semaphores, mutexes and condition variables.

## Description

Simulation of an automated cargo terminal where each truck runs as an independent thread and competes for a limited number of loading docks. A scheduler decides which truck runs next using FIFO or Round Robin, and the system is designed to avoid deadlocks and race conditions while trucks share the docks, the scheduler queue, the event log and the metrics. At the end it reports waiting and turnaround times for every truck.

## Features

- Each truck runs as its own POSIX thread
- Two scheduling algorithms: FIFO and Round Robin with configurable quantum
- Loading docks controlled with a counting semaphore
- Mutexes protecting shared resources (scheduler queue, log and metrics)
- Condition variables so only the active truck in the scheduler can run
- Design that prevents deadlocks and race conditions (documented in the PDF)
- Event log of every truck action
- Metrics per truck and averages: waiting time and turnaround time
- Number of trucks, docks and quantum set from the command line

## Tech Stack

- **Language:** C
- **Concurrency:** POSIX threads (pthreads), semaphores, mutexes, condition variables
- **Build:** GCC, Make
- **Platform:** Linux

## Project Structure

```
├── main.c                  # Entry point and argument parsing
├── include/                # Headers
├── src/
│   ├── camion.c            # Truck thread logic
│   ├── planificador.c      # Scheduler queue
│   ├── planificador_fifo.c # FIFO algorithm
│   ├── planificador_rr.c   # Round Robin algorithm
│   ├── recursos.c          # Docks (semaphore)
│   ├── log.c               # Event log
│   ├── metricas.c          # Waiting and turnaround times
│   └── terminal.c          # Terminal setup and execution
├── Makefile
└── Documentacion_P1_SO.pdf # Full documentation
```

## How to Run

### Prerequisites

- Linux (or WSL on Windows)
- GCC and Make

### Build

```bash
make clean
make
```

### Run

```bash
# FIFO: ./proyecto_so fifo [trucks] [docks]
./proyecto_so fifo 5 3

# Round Robin: ./proyecto_so rr [trucks] [docks] [quantum]
./proyecto_so rr 6 3 2
```

Running `./proyecto_so` without arguments uses FIFO with 5 trucks, 3 docks and quantum 2. The maximum is 20 trucks.

## Team

Built with Jesús Barboza Ríos and José Daniel Garita Guzmán.
