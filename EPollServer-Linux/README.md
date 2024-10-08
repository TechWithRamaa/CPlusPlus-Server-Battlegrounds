# Non-Blocking Async Server with Epoll

## Overview

This project implements a non-blocking asynchronous server using `epoll`, a scalable I/O event notification mechanism in Linux. The server utilizes a thread pool to handle client connections efficiently, allowing it to manage multiple simultaneous connections without blocking.

## Key Concepts

### 1. Epoll

**Epoll** is a Linux kernel feature that provides a scalable way to handle large numbers of file descriptors. Unlike traditional blocking I/O, `epoll` allows the server to monitor multiple sockets simultaneously, waking up only when there is data available for reading or writing. This significantly improves performance, especially under high load conditions.

### 2. Multiplexing

**Multiplexing** refers to the ability to manage multiple connections or data streams over a single communication channel. In the context of this server, multiplexing allows the server to handle many client connections using a single thread, reducing the overhead of context switching and improving resource utilization.

### 3. Thread Pool

A **Thread Pool** is used to manage a collection of worker threads that can execute tasks concurrently. This design pattern helps to efficiently utilize system resources and handle I/O-bound tasks without creating new threads for each request.

## Tasks in Context

In the context of a client request, **tasks** refer to the specific operations that need to be performed to process the incoming data from a client. These typically include:
- **Reading Data**: Retrieving the client's request data from the socket.
- **Processing Business Logic**: Implementing any necessary operations based on the received data (e.g., handling HTTP requests, performing computations).
- **Writing Data**: Sending a response back to the client based on the processed data.

Each of these tasks is executed in a worker thread from the thread pool, allowing the server to handle multiple requests concurrently.

## Flow of Execution

1. **Server Initialization**:
   - The server creates a socket and binds it to a specified port (8080).
   - It listens for incoming connections and sets up an `epoll` instance to monitor the server socket for incoming connections.

2. **Accepting Connections**:
   - When a new client connects, the server accepts the connection and sets the client socket to non-blocking mode.
   - The client socket is added to the `epoll` instance for monitoring.

3. **Handling Events**:
   - The server enters an infinite loop, waiting for events on the monitored sockets using `epoll_wait`.
   - When an event occurs (e.g., data available to read), the server processes the event:
     - If the event is on the server socket, it accepts the new client connection.
     - If the event is on a client socket, the server enqueues a task to handle the client's request.

4. **Task Execution**:
   - Each task is executed by a worker thread from the thread pool. The task involves:
     - **Reading Data**: The worker thread reads data from the client socket using `recv`.
     - **Processing Logic**: The data received is processed (e.g., echoing back the received data).
     - **Writing Data**: The response is sent back to the client using `send`.

5. **Graceful Handling**:
   - The worker thread checks for disconnections and errors. If the client disconnects, the socket is closed appropriately.

## Conclusion

This server design leverages non-blocking I/O and a thread pool to efficiently manage multiple client connections. The use of `epoll` and multiplexing techniques allows the server to handle a high volume of concurrent requests while maintaining low latency and high throughput.

## Getting Started

1. Clone the repository.
2. Compile the program.
3. Run the server.
4. Connect using a client (e.g., `telnet` or a custom client) to test the functionality.

## Limitations and Future Improvements

- Currently, the server only echoes data back. Implementing more complex business logic can enhance its functionality.
- Additional error handling and logging can be integrated for better monitoring.
- HTTP request parsing can be added to handle web requests more effectively.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
