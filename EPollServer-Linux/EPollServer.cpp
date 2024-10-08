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
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>

const int MAX_EVENTS = 10;
const int PORT = 8080;
const int BACKLOG = 128;

// Thread pool to manage worker threads
class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();
    void enqueue(std::function<void()> task);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this] {
                        return this->stop || !this->tasks.empty();
                    });
                    if (this->stop && this->tasks.empty()) return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) worker.join();
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");
        tasks.push(std::move(task));
    }
    condition.notify_one();
}

// Utility to set a socket to non-blocking mode
void setNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

// Function to send HTTP response
void sendHttpResponse(int client_fd, const std::string &content) {
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
    response += "Connection: close\r\n\r\n";
    response += content;

    send(client_fd, response.c_str(), response.size(), 0);
}

// Function to handle a single client connection
void handleClient(int client_fd) {
    char buffer[1024];
    while (true) {
        ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No data to read, try again later
                break;
            } else {
                std::cerr << "recv error: " << strerror(errno) << std::endl;
                close(client_fd);
                return;
            }
        } else if (bytes_read == 0) {
            std::cout << "Client disconnected" << std::endl;
            close(client_fd);
            return;
        } else {
            buffer[bytes_read] = '\0'; // Null-terminate the buffer
            std::cout << "Received request: " << buffer << std::endl;
            sendHttpResponse(client_fd, "Hello, World!"); // Send response
            break; // Exit after handling the request
        }
    }
}

int main() {
    // Create thread pool with 4 threads
    ThreadPool pool(4);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, BACKLOG) == -1) {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return -1;
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Epoll create failed" << std::endl;
        close(server_fd);
        return -1;
    }

    epoll_event event{}, events[MAX_EVENTS];
    event.data.fd = server_fd;
    event.events = EPOLLIN;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        std::cerr << "Epoll ctl failed" << std::endl;
        close(server_fd);
        close(epoll_fd);
        return -1;
    }

    while (true) {
        int n_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < n_fds; i++) {
            if (events[i].data.fd == server_fd) {
                // Accept new client connection
                int client_fd = accept(server_fd, nullptr, nullptr);
                if (client_fd == -1) {
                    std::cerr << "Accept failed" << std::endl;
                    continue;
                }
                setNonBlocking(client_fd);

                // Add new client socket to epoll for monitoring
                event.data.fd = client_fd;
                event.events = EPOLLIN | EPOLLET; // Edge-triggered
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                    std::cerr << "Epoll ctl add client failed" << std::endl;
                    close(client_fd);
                }
            } else {
                // Data available on existing client socket
                int client_fd = events[i].data.fd;
                pool.enqueue([client_fd]() {
                    handleClient(client_fd);
                });
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
    return 0;
}
