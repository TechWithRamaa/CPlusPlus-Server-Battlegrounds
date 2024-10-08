#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h> // For fcntl()
#include <signal.h> // For signal handling
#include "ctpl_stl.h" // Include the thread pool library

#define PORT 8080
#define BACKLOG 5
#define THREAD_POOL_SIZE 4 // Define how many threads the pool should have
#define TIMEOUT_SECONDS 5 // Timeout duration in seconds

// Function to handle the client connection
void handle_client(int client_socket) {
    char buffer[1024] = {0};
    const char *hello = "HTTP/1.1 200 OK\r\nContent-Length: 20\r\n\r\nHello from server";

    // Set receive and send timeout options
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SECONDS; // Set timeout duration
    timeout.tv_usec = 0; // Microseconds

    // Set receive timeout
    if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
        perror("setsockopt(SO_RCVTIMEO) failed");
        close(client_socket);
        return;
    }

    // Set send timeout
    if (setsockopt(client_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
        perror("setsockopt(SO_SNDTIMEO) failed");
        close(client_socket);
        return;
    }

    while (true) {
        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));

        // Read data from the client
        int valread = read(client_socket, buffer, sizeof(buffer));

        // If valread <= 0, the client has closed the connection or an error occurred
        if (valread <= 0) {
            std::cout << "Client disconnected or read timeout occurred.\n";
            break; // Exit the loop, client connection will be closed below
        }
        std::cout << "Received: " << buffer << std::endl;

        // Send a message back to the client
        int sent_bytes = send(client_socket, hello, strlen(hello), 0);
        if (sent_bytes < 0) {
            std::cerr << "Error sending message to client.\n";
            break; // Exit the loop, client connection will be closed below
        }
        std::cout << "Hello message sent to client.\n";
    }

    // Close the client socket after the client disconnects
    close(client_socket);
    std::cout << "Connection closed by server thread.\n";
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    // Create a thread pool with a fixed number of threads
    ctpl::thread_pool pool(THREAD_POOL_SIZE);

    // 1. Create a TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // 2. Configure the server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    // 3. Bind the socket to the IP address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    // 4. Listen for incoming connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    std::cout << "Server listening on port " << PORT << "...\n";

    // 5. Loop to accept and handle client connections
    while (true) {
        std::cout << "Waiting for a new connection..." << std::endl;
        // Accept a new connection
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0) {
            std::cerr << "accept failed: " << strerror(errno) << std::endl;
            continue; // Go back to waiting for a new connection
        }
        std::cout << "Connection accepted from client.\n";
        
        // Submit the client handling task to the thread pool
        pool.push([new_socket](int) { handle_client(new_socket); });
    }
    
    // 7. Close the server socket (never reached in this case)
    close(server_fd);
    return 0;
}
