#include <stdio.h>     // For printf and perror
#include <stdlib.h>    // For exit()
#include <string.h>    // For memset
#include <unistd.h>    // For close() and shutdown()
#include <arpa/inet.h> // For socket functions (socket, bind, listen, accept)

// Server constants
#define PORT 8080      // Port number for the server
#define BACKLOG 5      // How many pending connections queue

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[1024] = {0}; // Buffer to store incoming data

    // 1. Create a socket (IPv4, TCP)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Configure server address structure
    address.sin_family = AF_INET;                // IPv4
    address.sin_addr.s_addr = INADDR_ANY;        // Bind to all network interfaces
    address.sin_port = htons(PORT);              // Convert port to network byte order

    // 3. Bind the socket to the IP and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Binding failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4. Listen for incoming connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server is running on port %d and waiting for connections...\n", PORT);

    // Ever-running loop to accept and process clients
    while (1) {
        // 5. Accept a client connection (blocks until a connection is made)
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Connection accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        printf("New connection accepted.\n");

        // 6. Read data sent by the client
        memset(buffer, 0, sizeof(buffer));  // Clear buffer before reading
        int valread = read(new_socket, buffer, 1024);
        printf("Received: %s\n", buffer);

        // 7. Create a proper HTTP response
        const char *response_body = "<html><body><h1>Hello, World!</h1></body></html>";
        const char *http_response_template =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"         // Placeholder for body length
            "Connection: close\r\n"          // Indicate that the connection will be closed
            "\r\n"                           // Blank line to indicate end of headers
            "%s";                            // Placeholder for body

        // Calculate content length (size of the HTML body)
        int content_length = strlen(response_body);

        // Allocate buffer for the full response
        char http_response[1024];
        snprintf(http_response, sizeof(http_response), http_response_template, content_length, response_body);

        // 8. Send the HTTP response to the client
        send(new_socket, http_response, strlen(http_response), 0);
        printf("HTTP response sent to client.\n");

        // 9. Properly shutdown the sending side of the socket
        shutdown(new_socket, SHUT_WR);

        // 10. Close the client socket
        close(new_socket);
    }

    // Close server socket (though, in this case, the server never stops running)
    close(server_fd);

    return 0;
}
