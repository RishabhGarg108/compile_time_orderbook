# Compile-Time Limit Order Book (C++)

A compile-time implementation of a price–time priority limit order book using advanced C++ template metaprogramming.

This project uses a familiar HFT domain problem to build deep intuition for type-level programming, recursive templates, and compile-time algorithm design.

> ⚠️ **This is an educational project, not a production trading system.**

---

## Overview

The project models a limit order book (LOB) **entirely at compile time**, where:

- Orders, price levels, and book sides are represented as **types**
- All operations (insert, cancel, modify) are resolved during **template instantiation**
- No runtime state, mutation, or dynamic memory is involved

Each component of a real-world order book is mapped to a compile-time construct.

---

## What Is Implemented

- Limit orders with:
  - ID
  - Side
  - Price
  - Quantity
- **Price–time priority**
- **Bids** sorted by **descending price**
- **Asks** sorted by **ascending price**
- **FIFO ordering** at the same price

### Compile-time support for:

- Order insertion
- Order cancellation
- Order modification (with time priority reset)
- Fully type-driven order book composition

Every node in the book — orders, levels, sides — exists as a **type**, not an object.

---

## Why Compile-Time?

This project is intentionally unrealistic by design.

### Interview-Oriented Motivation

Low-latency / HFT C++ interviews often test:

- Ability to model complex invariants
- Comfort with recursive templates and partial specialization
- Understanding of compile-time execution and instantiation mechanics

If you can reason about a limit order book built from template recursion, most “hard” C++ interview problems become straightforward.

### Skill Transfer

The techniques used here directly apply to:

- Policy-based design
- Static dispatch
- Compile-time configuration
- Zero-overhead abstractions

---

## How This Project Stands Out

- Uses a realistic financial system instead of toy TMP examples
- Designed as a learning artifact, not just a code dump
- Closely aligned with expectations for low-latency C++ roles

> This is not about clever tricks — it’s about **thinking in types**.

---

## Project Structure (High Level)

```text
OrderBook
 ├── BidSide   (sorted by descending price)
 │     └── PriceLevels → Orders
 └── AskSide   (sorted by ascending price)
       └── PriceLevels → Orders
```

---

## Build
Building this is as simple as running ```g++ orderbook.cpp && ./a.out```

It should give an output similar to this:
```
book0: 
Bids: 
Asks: 
book1: 
Bids: 
100: (1 0 100 5), 
Asks: 
book2: 
Bids: 
100: (1 0 100 5), 
Asks: 
102: (5 1 102 10), 
book3: 
Bids: 
100: (1 0 100 5), 
Asks: 
101: (6 1 101 5), 
102: (5 1 102 10), 
book4: 
Bids: 
100: (1 0 100 5), 
99: (4 0 99 10), 
Asks: 
101: (6 1 101 5), 
102: (5 1 102 10), 
book5: 
Bids: 
101: (3 0 101 10), 
100: (1 0 100 5), 
99: (4 0 99 10), 
Asks: 
101: (6 1 101 5), 
102: (5 1 102 10), 
book6: 
Bids: 
101: (3 0 101 10), 
100: (1 0 100 5), 
99: (4 0 99 10), (2 0 99 10), 
Asks: 
101: (6 1 101 5), 
102: (5 1 102 10), 
book7: 
Bids: 
101: (3 0 101 10), 
100: (1 0 100 5), (4 0 100 10), 
99: (2 0 99 10), 
Asks: 
101: (6 1 101 5), 
102: (5 1 102 10), 
book8: 
Bids: 
101: (3 0 101 10), 
100: (1 0 100 5), 
99: (2 0 99 10), 
Asks: 
100: (4 1 100 10), 
101: (6 1 101 5), 
102: (5 1 102 10), 
book9: 
Bids: 
101: (3 0 101 10), 
100: (1 0 100 5), 
99: (2 0 99 10), 
Asks: 
100: (4 1 100 10), 
102: (5 1 102 10), (6 1 102 100), 
book10: 
Bids: 
101: (3 0 101 10), 
100: (1 0 100 5), 
Asks: 
100: (4 1 100 10), 
102: (5 1 102 10), (6 1 102 100), 
book11: 
Bids: 
101: (3 0 101 10), 
100: (1 0 100 5), 
Asks: 
102: (5 1 102 10), (6 1 102 100), 
book12: 
Bids: 
101: (3 0 101 10), 
100: (1 0 100 5), 
Asks: 
102: (5 1 102 10), 
```
