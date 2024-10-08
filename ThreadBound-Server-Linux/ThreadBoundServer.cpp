#include <iostream>
#include <thread>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>

#define PORT 8080
#define BACKLOG 5

// Function to handle the client connection in a separate thread
void handle_client(int client_socket) {
    char buffer[1024] = {0};

    // Read data sent by the client
    int valread = read(client_socket, buffer, 1024);
    if (valread < 0) {
        std::cerr << "Error reading from client." << std::endl;
        close(client_socket);
        return;
    }
    std::cout << "Received: " << buffer << std::endl;

    // Properly formatted HTTP response
    const char *http_response = 
        "HTTP/1.1 200 OK\r\n"                // Status line: HTTP version and status code
        "Content-Type: text/plain\r\n"       // Header: Content type (plain text)
        "Content-Length: 14\r\n"             // Header: Length of the response body
        "Connection: close\r\n"              // Header: Connection closure
        "\r\n"                               // Empty line to indicate the end of the headers
        "Hello, World!\n";                   // The actual response body

    // Send the response
    ssize_t sent_bytes = send(client_socket, http_response, strlen(http_response), 0);
    if (sent_bytes < 0) {
        std::cerr << "Error sending response." << std::endl;
    }
    std::cout << "HTTP response sent to client." << std::endl;

    // Close the client socket
    close(client_socket);
    std::cout << "Connection closed." << std::endl;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

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

        // 6. Create a new thread for each client
        std::thread client_thread(handle_client, new_socket);
        client_thread.detach(); // Detach the thread to automatically clean up when it's done
    }

    // 7. Close the server socket (never reached in this case)
    close(server_fd);
    return 0;
}
