# High-Performance Multithreaded Chat System

## Overview
A robust, concurrent TCP chat application developed in **C++20**, demonstrating low-level network programming without reliance on high-level frameworks like Boost.Asio. The project implements a scalable **Client-Server architecture** capable of handling multiple simultaneous users with features like private messaging, broadcasting, and automated load testing (spam bot).

This project highlights manual memory management, thread synchronization using modern C++ primitives, and custom protocol design.

## Key Features

* **Concurrency & Synchronization:**
    * Uses `std::thread` for non-blocking I/O operations (separate threads for reading, parsing, and logging).
    * Implements `std::binary_semaphore` (C++20) for efficient signaling between network receivers and message processors.
    * Thread-safe user management via `std::mutex`.
* **Networking:**
    * Raw **BSD Sockets / Winsock2** implementation.
    * Custom text-based application protocol for minimal overhead.
* **Architecture Components:**
    * **UserManager:** Centralized hub for mapping usernames to active socket handlers, ensuring safe client registration and removal.
    * **MessageLogger:** Asynchronous, detached thread worker that logs chat history to files without blocking the main server loop.
    * **Spam/Load Tester:** Built-in client mode to simulate high-traffic scenarios and test server stability .
* **Messaging Modes:**
    * Global Broadcast (One-to-Many).
    * Private Messaging (One-to-One).
    * System Notifications (Server-to-Client).

## Architecture & Tech Stack

### Components
1.  **ChatServer:** Initializes the socket listener and accepts incoming TCP connections.
2.  **UserManager:** A thread-safe singleton-like class that maintains a `std::map<string, ClientHandler*>` of active users. It prevents race conditions during user login/logout.
3.  **ClientHandler:** Created for each connected user. It manages the specific socket, buffers incoming data, and parses the custom protocol.
4.  **Protocol:** A lightweight delimiter-based protocol:
    * Format: `TYPE|RECIPIENT|SENDER|CONTENT`
    * Example: `P|Alice|Bob|Hello Alice!` .

### Technologies
* **Language:** C++20
* **Libraries:** Standard Library (`<thread>`, `<atomic>`, `<semaphore>`, `<mutex>`, `<map>`), Winsock2 (Windows) / sys/socket (Linux).
* **Build System:** CMake / Manual Compilation via GCC/MSVC.

