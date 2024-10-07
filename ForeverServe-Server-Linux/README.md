🖥️ Forever Running Server - C++ TCP Server for Fun & Learning! 🚀

Welcome to **Forever Running Server**, a **minimalistic C++ TCP server** designed to serve continuosly client requests in an infinite loop! This project is perfect for those who want to deepen their understanding of socket programming and network communication. It's lightweight, simple, and sends a proper HTTP response to client requests!

🌟 Features
* Simple TCP server built with C++.
* Handles multiple connections in a loop — easy and straightforward.
* Responds with "Hello, World!" via a proper HTTP/1.1 response.
* Customizable port (default: 8080).

🔧 Requirements
- Linux operating system
- C++ compiler (GCC recommended)
- CMake (minimum version 3.16)

💻 Build Instructions

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/Ctrl-Alt-Serve-with-C++-Servers.git
   ```

2. **Navigate to the project directory**:
   ```bash
   cd linux-servers/forever-running-server
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
   ./ForeverRunningServer
   ```
7. **Example Client Request (using curl)**:
   ```bash
   curl http://localhost:8080
   ```

### 🛠️ How It Works

The server listens for incoming connections on port 8080 and sends back a simple HTTP response. Here's what happens under the hood:

1. Creates a TCP socket.
2. Binds the socket to INADDR_ANY (all available network interfaces).
3. Listens for a connection.
4. Accepts the connection and reads the client's request.
5. Responds with a 200 OK HTTP message and a "Hello, World!" message.
6. Closes the connection and goes back to listening for the next one.


### 🌍 Real-World Use Cases

While this server only handles a single connection, it's a great starting point for:

* Building a small web server.
* Learning the basics of networking with sockets.
* Exploring how HTTP works at a low level.
* Developing more complex server applications.

### 📝 License
This project is licensed under the MIT License.

### 🙌 Contributions
Feel free to fork, modify, and make a pull request! Let's make networking more fun and accessible. 

Enjoy coding, and remember, every great server starts with "Hello, World!" 😎