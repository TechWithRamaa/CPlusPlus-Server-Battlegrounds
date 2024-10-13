#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed.\n";
        return -1;
    }

    // Server address setup
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection to server failed.\n";
        return -1;
    }

     std::cout << "Connected to server...";

    char buffer[1024];
    std::string message;

    // Loop to send and receive messages
    while (true) {
        std::cout << "Enter message (type 'q' to quit): ";
        std::getline(std::cin, message);

        // Send message to server
        send(sockfd, message.c_str(), message.length(), 0);

        // Check if the client wants to quit
        if (message == "q") {
            break;
        }

        // Receive response from server
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            std::cout << "Received from server: " << buffer << std::endl;
        } else {
            std::cerr << "Error receiving data from server." << std::endl;
            break;
        }
    }

    // Close the socket
    close(sockfd);
    std::cout << "Connection closed." << std::endl;
    return 0;
}
