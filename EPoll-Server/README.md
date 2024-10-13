🌐 Multi-Threaded Server using EPoll for Multiplexing IO 🧵

This program demonstrates a multi-threaded server using epoll for efficient I/O event handling and a thread pool to manage concurrent client connections. The server listens for incoming client connections on port 8080 and handles multiple clients asynchronously by registering client sockets with epoll. It utilizes non-blocking sockets and edge-triggered events (EPOLLET), allowing the server to efficiently manage high-load environments with minimal resource usage. The thread pool, managed by the CTPL library, ensures that client interactions such as receiving and sending data are processed in parallel, optimizing performance and scalability for real-time applications.

### 🛠️ Requirements
Linux operating system
g++ compiler (or another C++ compiler)
CMake (minimum version 3.16)

### 💻 Build Instructions

1. **Clone the repository**:
   ```bash
   git clone https://github.com/TechWithRamaa/CPlusPlus-Server-Battlegrounds.git
   ```

2. **Navigate to the project directory**:
   ```bash
   cd CPlusPlus-Server-Battlegrounds/EPoll-Server
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
   ./EPollServer
   ```

### 📝 License
This project is licensed under the MIT License.



