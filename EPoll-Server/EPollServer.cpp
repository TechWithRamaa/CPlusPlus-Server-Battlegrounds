#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include "ctpl_stl.h" // Include CTPL thread pool library

const int MAX_EVENTS = 10;
const int PORT = 8080;
const int BACKLOG = 128;
const int TIMEOUT = 5000; // 5 seconds

// Utility function to set a socket to non-blocking mode
void setNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

// Function to create a listening socket
int createSocket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    addr.sin_port = htons(PORT);

    if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << "Bind failed" << std::endl;
        close(sockfd);
        return -1;
    }

    if (listen(sockfd, BACKLOG) == -1) {
        std::cerr << "Listen failed" << std::endl;
        close(sockfd);
        return -1;
    }

    setNonBlocking(sockfd);
    return sockfd;
}

// Function to handle client communication
void handleClient(int client_fd) {
    char readBuffer[1024];
    char writeBuffer[4096];

    while (true) {
        memset(readBuffer, 0, sizeof(readBuffer));
        memset(writeBuffer, 0, sizeof(readBuffer));

        ssize_t bytes_received = recv(client_fd, readBuffer, sizeof(readBuffer) - 1, 0);

        if (bytes_received > 0) {
            readBuffer[bytes_received] = '\0';
            std::string message(readBuffer);
            std::cout << "Received from client (Socket FD: " << client_fd << ") - : " << message << std::endl;

            // Check for termination message
            if (message.find("q") != std::string::npos) {
                std::cout << "Client requested to close the connection." << std::endl;
                break;
            } 
            std::string response = "Have a good day!!";
            strncpy(writeBuffer, response.c_str(), sizeof(writeBuffer) - 1); 
            send(client_fd, writeBuffer, strlen(writeBuffer), 0);
        } else if (bytes_received == 0) {
            // Client closed the connection
            std::cout << "Client disconnected." << std::endl;
            break;
        } else if (errno != EWOULDBLOCK && errno != EAGAIN) {
            std::cerr << "Recv error occurred!" << std::endl;
            break;
        }
    }
    close(client_fd); // Close the client connection
    std::cout << "Connection closed with client." << std::endl;
}

int main() {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Epoll create failed" << std::endl;
        return -1;
    }

    // Create listening socket
    int listen_fd = createSocket();
    if (listen_fd == -1) {
        return -1;
    }

    std::cout << "server started at PORT " << PORT << std::endl;

    // Setup CTPL Thread Pool with 4 threads
    ctpl::thread_pool pool(8);

    // Register listening socket with epoll
    epoll_event event{}, events[MAX_EVENTS];
    event.events = EPOLLIN | EPOLLET | EPOLLOUT ;
    event.data.fd = listen_fd;
    
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) == -1) {
        std::cerr << "Failed to add listen socket to epoll" << std::endl;
        close(listen_fd);
        return -1;
    }

    // Dedicated thread to monitor epoll events
    std::thread epoll_thread([&]() {
        while (true) {
            int n_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
            for (int i = 0; i < n_fds; i++) {
                if (events[i].data.fd == listen_fd) {
                    // New client connection
                    int client_fd = accept(listen_fd, nullptr, nullptr);
                    if (client_fd != -1) {
                        std::cout << "New client connected" << std::endl;
                        setNonBlocking(client_fd);

                        // Register the new client socket for EPOLLIN
                        epoll_event client_event{};
                        client_event.events = EPOLLIN | EPOLLET | EPOLLOUT; // Edge-triggered
                        client_event.data.fd = client_fd;

                        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event) == -1) {
                            std::cerr << "Failed to add client socket to epoll" << std::endl;
                            close(client_fd);
                        }
                    } else {
                        std::cerr << "Accept failed" << std::endl;
                    }
                } else {
                    // Existing client has sent data, handle in a separate thread from the pool
                    int client_fd = events[i].data.fd;
                    if (events[i].events & EPOLLIN) {
                        pool.push([client_fd](int id) {
                            handleClient(client_fd);
                        });
                    }
                }
            }
        }
    });

    epoll_thread.join();
    close(listen_fd);
    close(epoll_fd);
    return 0;
}
