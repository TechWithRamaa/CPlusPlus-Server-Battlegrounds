🌐 Client program with Sockets 🧵

This code demonstrates a basic TCP client that connects to a server over a specified port, sends messages, and receives responses. It is useful in projects involving client-server communication, allowing for real-time data exchange between two endpoints. The client continually accepts user input, sends it to the server, and displays the server's response, which can be used for testing and debugging server behavior. This structure could be applied in chat applications, remote command execution, or any project that requires continuous communication with a remote server.

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
   cd CPlusPlus-Server-Battlegrounds/EPoll-Client
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

6. **Running the Client to connect with the server**:
   ```bash
   ./EPollClient
   ```