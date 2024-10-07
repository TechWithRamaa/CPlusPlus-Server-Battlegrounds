#include <stdio.h>   // For printf and perror
#include <stdlib.h>  // For exit()
#include <string.h>  // For memset
#include <unistd.h>  // For close()
#include <arpa/inet.h> // For socket functions and structures (socket, bind, listen, accept, etc.)

#define PORT 8080    // Port number for the server
#define BACKLOG 5    // How many connections can be pending for accept()

int main() {
    int server_fd, new_socket;               // File descriptors for the server and new client connection
    struct sockaddr_in address;              // Structure to hold server's address information
    int addrlen = sizeof(address);           // Size of the address structure
    char buffer[1024] = {0};                 // Buffer to store received data

    // The HTTP response that will be sent to the client
    const char *http_response = 
        "HTTP/1.1 200 OK\r\n"                // Status line: HTTP version and status code
        "Content-Type: text/plain\r\n"       // Header: Content type (plain text)
        "Content-Length: 14\r\n"             // Header: Length of the response body
        "Connection: close\r\n"              // Header: Connection closure
        "\r\n"                               // Empty line to indicate the end of the headers
        "Hello, World!\n";                   // The actual response body

    // 1. Create a TCP socket using the IPv4 protocol (AF_INET) and stream type (SOCK_STREAM)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // 2. Configure the server address
    address.sin_family = AF_INET;            // Set address family to IPv4 (AF_INET)
    address.sin_addr.s_addr = INADDR_ANY;    // Bind the socket to all available network interfaces (INADDR_ANY)
    address.sin_port = htons(PORT);          // Convert port to network byte order (big-endian)

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
    printf("Server listening on port %d...\n", PORT);
    
    // 5. Accept a connection from a client (this call blocks until a client connects)
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Connection accepted.\n");

    // 6. Read data sent by the client (optional, depending on the client's request)
    int valread = read(new_socket, buffer, 1024);
    printf("Received: %s\n", buffer);

    // 7. Send a proper HTTP response to the client
    send(new_socket, http_response, strlen(http_response), 0);
    printf("HTTP response sent to client.\n");

    // 8. Close the client socket when done
    close(new_socket);

    // 9. Close the server socket as well
    close(server_fd);

    return 0;
}
