#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024
#define FIFO_PATH "/tmp/hello_epoll_fifo"

int main() {
    int epoll_fd, stdin_fd, fifo_fd;
    struct epoll_event ev, events[MAX_EVENTS];
    char buffer[BUFFER_SIZE];

    // Create a named pipe (FIFO)
    unlink(FIFO_PATH);  // Remove if it already exists
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        std::cerr << "Error creating FIFO: " << strerror(errno) << std::endl;
        return 1;
    }
    std::cout << "Created FIFO at: " << FIFO_PATH << std::endl;
    std::cout << "From another terminal, run: echo 'Hello from FIFO' > " << FIFO_PATH << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    // Create epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Error creating epoll: " << strerror(errno) << std::endl;
        unlink(FIFO_PATH);
        return 1;
    }

    // Set stdin to non-blocking mode
    stdin_fd = STDIN_FILENO;
    int flags = fcntl(stdin_fd, F_GETFL, 0);
    fcntl(stdin_fd, F_SETFL, flags | O_NONBLOCK);

    // Add stdin to epoll
    ev.events = EPOLLIN;
    ev.data.fd = stdin_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, stdin_fd, &ev) == -1) {
        std::cerr << "Error adding stdin to epoll: " << strerror(errno) << std::endl;
        close(epoll_fd);
        unlink(FIFO_PATH);
        return 1;
    }
    std::cout << "Added stdin (fd=" << stdin_fd << ") to epoll" << std::endl;

    // Open FIFO in non-blocking mode for reading
    fifo_fd = open(FIFO_PATH, O_RDONLY | O_NONBLOCK);
    if (fifo_fd == -1) {
        std::cerr << "Error opening FIFO: " << strerror(errno) << std::endl;
        close(epoll_fd);
        unlink(FIFO_PATH);
        return 1;
    }

    // Add FIFO to epoll
    ev.events = EPOLLIN;
    ev.data.fd = fifo_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fifo_fd, &ev) == -1) {
        std::cerr << "Error adding FIFO to epoll: " << strerror(errno) << std::endl;
        close(fifo_fd);
        close(epoll_fd);
        unlink(FIFO_PATH);
        return 1;
    }
    std::cout << "Added FIFO (fd=" << fifo_fd << ") to epoll" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Waiting for events... (type 'quit' to exit)" << std::endl;
    std::cout << std::endl;

    // Event loop
    bool running = true;
    while (running) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            std::cerr << "Error in epoll_wait: " << strerror(errno) << std::endl;
            break;
        }

        // Process all ready file descriptors
        for (int i = 0; i < nfds; i++) {
            int fd = events[i].data.fd;

            if (events[i].events & EPOLLIN) {
                ssize_t bytes_read = read(fd, buffer, BUFFER_SIZE - 1);

                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';

                    if (fd == stdin_fd) {
                        std::cout << "[STDIN] Received: " << buffer;

                        // Check for quit command
                        if (strncmp(buffer, "quit", 4) == 0) {
                            running = false;
                            break;
                        }
                    } else if (fd == fifo_fd) {
                        std::cout << "[FIFO] Received: " << buffer;
                    }
                } else if (bytes_read == 0) {
                    // EOF - for FIFO, this happens when writer closes
                    if (fd == fifo_fd) {
                        std::cout << "[FIFO] Writer closed, reopening..." << std::endl;
                        close(fifo_fd);

                        // Reopen FIFO
                        fifo_fd = open(FIFO_PATH, O_RDONLY | O_NONBLOCK);
                        if (fifo_fd == -1) {
                            std::cerr << "Error reopening FIFO: " << strerror(errno) << std::endl;
                            running = false;
                            break;
                        }

                        // Re-add to epoll
                        ev.events = EPOLLIN;
                        ev.data.fd = fifo_fd;
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fifo_fd, &ev) == -1) {
                            std::cerr << "Error re-adding FIFO to epoll: " << strerror(errno) << std::endl;
                            running = false;
                            break;
                        }
                    }
                } else {
                    // Error reading
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        std::cerr << "Error reading from fd " << fd << ": " << strerror(errno) << std::endl;
                    }
                }
            }

            if (events[i].events & (EPOLLERR | EPOLLHUP)) {
                std::cerr << "Error/Hangup on fd " << fd << std::endl;
            }
        }
    }

    // Cleanup
    std::cout << std::endl
              << "Cleaning up..." << std::endl;
    close(fifo_fd);
    close(epoll_fd);
    unlink(FIFO_PATH);

    return 0;
}
