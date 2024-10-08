#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "ctpl_stl.h" // Include the thread pool library
#define PORT 8080
#define BACKLOG 5
#define THREAD_POOL_SIZE 4 // Define how many threads the pool should have

// Function to handle the client connection
void handle_client(int id, int client_socket) {
    (void)id; // To avoid unused parameter warning if you're not using the thread ID
    char buffer[1024] = {0};
    const char *http_response = "HTTP/1.1 200 OK\r\nContent-Length: 18\r\nContent-Type: text/plain\r\n\r\nHello from server";
    
    while (true) {
        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));

        // Read data from the client
        int valread = read(client_socket, buffer, 1024);

        // If valread <= 0, the client has closed the connection or an error occurred
        if (valread <= 0) {
            std::cout << "Client disconnected or error occurred.\n";
            break;
        }
        std::cout << "Received: " << buffer << std::endl;

        // Send a well-formed HTTP response back to the client
        send(client_socket, http_response, strlen(http_response), 0);
        std::cout << "HTTP response sent to client.\n";
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
        std::cout << "Waiting for a new connection...\n";

        // Accept a new connection
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0) {
            std::cerr << "accept failed: " << strerror(errno) << std::endl;
            continue; // Go back to waiting for a new connection
        }

        std::cout << "Connection accepted from client.\n";

        // 6. Submit the client handling task to the thread pool using a lambda function
        pool.push([new_socket](int id) {
            handle_client(id, new_socket);
        });
    }

    // 7. Close the server socket (never reached in this case)
    close(server_fd);
    return 0;
}
