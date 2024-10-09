# CPlusPlus-Server-Battlegrounds

Welcome to the **CPlusPlus-Server-Battlegrounds**! This repository showcases a collection of server implementations in C++, each designed with distinct characteristics and limitations. The aim is to explore different server architectures and their performance implications in handling concurrent client requests.

## Tech Stack

- **Programming Language**: C++
- **Build System**: CMake
- **I/O Model**: Various (e.g., Blocking, Non-blocking, EPoll)
- **Concurrency**: Thread Pooling, Asynchronous I/O, etc.
- **Operating System**: Linux (for EPollServer and other asynchronous implementations)

## Projects Overview

### 1. SingleRequest-Server
- **Characteristics**: Handles one request at a time, focusing on simplicity and ease of understanding.
- **Limitations**: Not suitable for high-load scenarios as it cannot handle concurrent requests.

### 2. ForeverServe-Server
- **Characteristics**: Designed to run indefinitely, maintaining state across multiple client connections and sessions.
- **Limitations**: Resource management can be challenging, leading to potential memory leaks if not handled properly.

### 3. ThreadBound-Server
- **Characteristics**: Ties server threads to specific client connections, ensuring dedicated resources for each client.
- **Limitations**: May lead to inefficient resource utilization and increased overhead in managing threads.

### 4. ConcurrentServe-Server
- **Characteristics**: This server supports concurrent connections, allowing multiple clients to interact with the server simultaneously.
- **Limitations**: May face performance degradation under heavy load due to thread contention.

### 5. NonBlocking-AsyncIO-Server
- **Characteristics**: Implements non-blocking I/O operations, allowing the server to handle requests asynchronously without blocking threads.
- **Limitations**: Complexity in implementation and potential for callback hell if not managed properly.

### 6. EPollServer
- **Characteristics**: Utilizes Linux's `epoll` for efficient I/O event handling, making it suitable for applications with a large number of simultaneous connections.
- **Limitations**: Limited to Linux environments; complexity in managing edge-triggered events.


## Getting Started

To get started with any of the server projects, follow these general steps:

1. **Clone the Repository**
   ```bash
   git clone https://github.com/yourusername/CPlusPlus-Server-Battlegrounds.git
   cd CPlusPlus-Server-Battlegrounds
   ```

2. **Build the Project**
    Each server project uses CMake for building. Navigate to the specific server folder and build the project
   ```bash
   cd EPollServer
   mkdir build && cd build
   cmake ..
   make
   ```

3. **Run a Server**
   ```bash
   ./EPollServer
   ```

4. **Contributing**
  Contributions are welcome! If you have an idea for a new server implementation or improvements to existing projects, feel free to fork the repository and submit a pull request.

5. **License**
   This project is licensed under the MIT License - see the LICENSE file for details.

6. **Acknowledgments**
   Special shout-out to **Educosys** for meticulously curating this course and skillfully teaching concepts from the ground up. Check it out here: [Advanced C++ with Networking Course](https://register.educosys.com/new-courses/26-advanced-c-with-networking).


7. **Dont miss out**
  The rest of the README remains similar to the previous version, ensuring that users have a clear overview of each project, how to get started, and how to contribute. This should create a comprehensive guide for anyone interested in exploring your server implementations.


