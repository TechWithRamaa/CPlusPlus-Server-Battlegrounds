🌐 ThreadBound Server - Linux C++ Server with Multi-Threaded Client Handling 🧵

Welcome to ThreadBound Server – a simple yet insightful C++ TCP server for Linux! 🚀
This project is built to handle multiple client requests using a thread-per-client model, which allows each client to be handled in its own thread. This is a great starting point for learning about network programming, socket handling, and multi-threading in C++.

### 🔥 Features
- Multi-threaded Client Handling: Each client gets its own thread, allowing multiple simultaneous connections.
- Simple TCP/IP Setup: Easy-to-understand code that sets up basic TCP communication.
- Ever-running Server: The server continuously runs, accepting new client connections.
- Customizable Port (default: 8080): Modify it to fit your needs.

### 🛠️ Requirements
Linux operating system
g++ compiler (or another C++ compiler)
CMake (minimum version 3.16)
Pthread library for handling threads

### 💻 Build Instructions

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/Ctrl-Alt-Serve-with-C++-Servers.git
   ```

2. **Navigate to the project directory**:
   ```bash
   cd linux-servers/ThreadBound-Server-Linux
   ``` 

3. **Create a build directory**:
   ```bash
   mkdir build && cd build
   ```

4. **Generate build files with CMake**:
   ```bash
   cmake ..
   ```

5. **Build the project**:
   ```bash
   make
   ```

6. **Running the Server**:
   ```bash
   ./ThreadBoundServer
   ```
7. **Example Client Request (using curl)**:
   ```bash
   curl http://localhost:8080
   ```

### 📝 How It Works
The server operates by:

Creating a TCP socket.
Binding to a specific IP address and port.
Listening for incoming connections.
Accepting connections and spawning a new thread for each client request.
Handling requests in each thread, then closing the connection once the response is sent.

### 🚧 Limitations of Thread-Per-Client Model
While the thread-per-client model works for learning purposes, it comes with significant limitations when applied in real-world scenarios:

**Thread Overhead**: Each client creates a new thread. With many clients, this leads to excessive overhead in terms of system resources like memory and CPU.
**Scalability Issues**: For high-traffic environments, the server will struggle to scale due to the limit on the number of threads the system can handle simultaneously.
**Performance Bottlenecks**: Creating and destroying threads for each client can lead to increased latency and performance degradation.
**Potential for Thread Exhaustion**: If too many clients connect simultaneously, the server may run out of available threads, leading to dropped connections.
💡 **Real-World Solution**: A better approach for handling high numbers of client requests is to use thread pooling, event-driven models (like select() or epoll), or frameworks like boost::asio.

### 🌍 Real-World Use Cases

Although the thread-per-client model isn't ideal for large-scale production environments, it's a perfect fit for:

Learning Multi-threading in C++
Experimenting with Client-Server Architectures
Prototyping Small Applications

### 🔍 Key Concepts Covered
**Sockets & Networking**: Learn how TCP/IP works at the code level.
**Multi-threading with C++**: Understand the implications of thread management.
**Server-Client Communication**: Experience the nuances of handling requests from multiple clients.

### 📝 License
This project is licensed under the MIT License.

### 🙌 Contributions
Found an issue? Want to add a feature? Feel free to fork the project and submit a pull request. Let's collaborate to make networking and multi-threading fun for everyone! 😄


### 🚀 Closing Thoughts
The ThreadBound Server is a powerful tool for getting hands-on experience with networking and multi-threading in C++. While it's limited in scalability, it's perfect for learning and experimentation. Remember, every big project starts small – and this server is a great stepping stone to mastering server-side development. 🌟

Happy coding! 🎉
