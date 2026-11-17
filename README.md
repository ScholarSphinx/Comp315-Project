# Multi-Threaded Order and Inventory Management System

A multi-threaded C++ application that simulates a warehouse environment where multiple customer orders and inventory operations run at the same time without corrupting stock data.
Built to demonstrate how concurrent programming works in a real business scenario, where several users updating the same inventory at once would normally cause errors.

**Duration:** Feb 2026 - May 2026  
**Tech Stack:** C++ | SFML

## Problem It Solves

In a real warehouse, multiple staff members process orders at the same time. If two people try to sell the last item in stock at the same moment, the system can oversell or show the wrong stock level.
This is called a **race condition**.
This project solves that by using threads and locks to make sure only one operation updates shared inventory at a time, while still allowing everything else to run in parallel.

## Features

### Order Management
- Create multiple customer orders concurrently
- Process orders in parallel using separate threads
- Track order status: Pending > Processing > Completed / Failed
- Reject orders if stock is insufficient

### Inventory Management
- Add, update and view inventory items
- Real-time stock level updates as orders are processed
- Prevents negative stock and duplicate updates
- Thread-safe stock deduction and restocking

### Concurrency Control
- **Threads** to simulate multiple customers and staff working simultaneously
- **Mutexes / Locks** to protect shared inventory data
- **Smart Pointers** (`unique_ptr` / `shared_ptr`) for safe memory management, no manual `new` / `delete`
- **Thread joining** to ensure all operations complete cleanly before shutdown

### SFML User Interface
- **Visual warehouse dashboard** built with SFML, no console-only interface
- Live inventory visualisation showing item name, stock level and status
- Click to run order simulation and watch it process in real time
- Colour-coded stock levels: Yellow = Precautionary Stock Count, Green = Satisfactory Stock Count, Red = Out of Stock/Extremely Low Stock Count
- Live activity log showing which thread processed which order
- Stock bars that change colours visually as orders are completed, so race conditions would be visible if they happened
- Pie chart representing ratio of successful to failed orders

## How Multithreading Works In This Project

1. Each customer order runs on its own thread
2. When a thread needs to update inventory, it locks the mutex
3. Only that thread can change the stock while it holds the lock
4. Once done, it unlocks and the next thread can proceed
5. This prevents two threads from reading and writing the same stock value at the same time

Without this, two orders for the last 1 item could both succeed — leaving stock at -1.

**Example scenario tested:**
- Starting stock: 10 units
- 5 threads each order 3 units at the same time
- Expected result: 3 orders succeed, 2 fail, final stock = 1
- Without mutexes: All 5 could succeed, final stock = -5 (wrong)

## Tech Used

- **Language:** C++
- **GUI:** SFML
- **Concurrency:** `std::thread`, `std::mutex`, `std::lock_guard`
- **Memory:** `std::unique_ptr`, `std::shared_ptr`
- **IDE / Build:** Visual Studio / g++
- **Version Control:** Git & GitHub
