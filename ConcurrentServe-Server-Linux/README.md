# Multi-Threaded TCP Server

This project implements a simple multi-threaded TCP server using a thread pool. The server listens for incoming client connections and responds with a pre-defined message. It can handle multiple clients concurrently by using a fixed number of worker threads, making it suitable for lightweight, concurrent request handling.

## Features

- **Thread Pool**: A thread pool is used to manage multiple client connections, efficiently handling requests in parallel.
- **Socket Programming**: Uses low-level TCP socket programming to listen on a port and accept incoming connections.
- **HTTP-like Response**: Sends a well-formed HTTP response to clients, making it suitable for basic interaction with HTTP clients like web browsers.

## Dependencies

- **POSIX Sockets**: For network communication, this server uses the `socket`, `bind`, `listen`, and `accept` system calls from the POSIX standard.
- **CTPL (C++ Thread Pool Library)**: A small, header-only C++ library that provides a thread pool for managing worker threads efficiently. It allows easy submission of tasks to be executed by the threads.

  - **Library Location**: [ctpl_stl](https://github.com/vit-vit/CTPL)

## Use Cases

This server is suitable for the following scenarios:

1. **Lightweight Multi-Client Handling**: The thread pool model allows the server to handle multiple client connections without creating a new thread for every request, reducing overhead and improving scalability.
   
2. **Basic Web Responses**: Since it returns a simple HTTP-like response, the server can be used to interact with HTTP clients or perform lightweight web serving tasks.

3. **Educational Purposes**: The project demonstrates basic TCP socket programming concepts, multi-threading with a thread pool, and low-level server-client communication, making it a good educational resource for learning about network programming in C++.

4. **Prototype or Test Server**: This server can be used in scenarios where a basic TCP server is required for testing purposes, such as testing client applications that interact with servers via sockets.

## Limitations

- **Not a Full HTTP Server**: Although the server returns a well-formed HTTP response, it is not an actual HTTP server. It does not support HTTP request parsing, routing, or any advanced HTTP features (e.g., handling GET/POST requests, HTTP headers, etc.).
  
- **Limited Scalability**: The server uses a fixed-size thread pool (`THREAD_POOL_SIZE`). For environments requiring dynamic scaling (e.g., handling hundreds or thousands of concurrent connections), a more advanced event-driven server architecture would be more suitable.

- **No SSL/TLS Support**: The server does not support encrypted communication (SSL/TLS), which means it is not suitable for secure communication out of the box.

- **Blocking I/O**: The server uses blocking system calls like `read` and `accept`, which might not be efficient under heavy load or with a large number of clients. For higher performance and scalability, a non-blocking or asynchronous I/O model would be more appropriate.

- **No Error Recovery**: The server doesn't implement advanced error handling or recovery mechanisms. If a client disconnects unexpectedly or if there is a network error, the server simply closes the connection.