#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <thread>
#include <chrono>
#include <cstring>
#include <cstdlib>

#define AT_PORT "/dev/smd8"
#define TIMEOUT 10

void readFromATPort(char *shared_mem) {
	
    int at_fd = open(AT_PORT, O_RDONLY | O_NONBLOCK);
    if (at_fd == -1) {
        std::cerr << "Failed to open AT port: " << AT_PORT << std::endl;
        return;
    }

    auto start_time = std::chrono::steady_clock::now();
    bool response_received = false;

    // Read from AT_PORT and store the result in shared memory
    while (true) {
        // Check if 10 seconds have passed
        auto current_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = current_time - start_time;

        if (elapsed_seconds.count() >= TIMEOUT) {
            break;
        }

        // Try to read from the non-blocking file descriptor
        ssize_t bytes_read = read(at_fd, shared_mem, 1024);
        if (bytes_read > 0) {
            shared_mem[bytes_read] = '\0';  // Null-terminate the string
            std::cout << shared_mem << std::endl;

            // Break if "OK", "FAIL", or "ERROR" is found in the response
            if (std::strstr(shared_mem, "OK\r") || std::strstr(shared_mem, "FAIL\r") || std::strstr(shared_mem, "ERROR\r")) {
                response_received = true;
                break;
            }
        }

        // Sleep for a short duration to avoid busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    close(at_fd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " AT_CMDs, ..." << std::endl;
        return 1;
    }

    // Create shared memory using mmap
    int shm_fd = open("/tmp/at_rx_tmp", O_RDWR | O_CREAT, 0666);
    if (shm_fd == -1) {
        std::cerr << "Failed to create shared memory file." << std::endl;
        return 1;
    }
    ftruncate(shm_fd, 1024); // Set the size of shared memory
    char *shared_mem = (char *)mmap(0, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (shared_mem == MAP_FAILED) {
        std::cerr << "Failed to map shared memory." << std::endl;
        close(shm_fd);
        return 1;
    }

    // Clear the shared memory
    std::memset(shared_mem, 0, 1024);

    // Start the reader thread
    std::thread readerThread(readFromATPort, shared_mem);

    // Send the AT command to the port
    std::ofstream atPort(AT_PORT);
    if (!atPort.is_open()) {
        std::cerr << "Failed to open AT port: " << AT_PORT << std::endl;
        munmap(shared_mem, 1024);
        close(shm_fd);
        return 1;
    }
    atPort << argv[1] << "\r\n";
    atPort.close();

    // Wait for the reader thread with a timeout
    if (readerThread.joinable()) {
        if (readerThread.joinable()) {
            readerThread.join();
        }
    }

    // Print the response from the shared memory
    //std::cout << shared_mem << std::endl;

    // Clean up
    munmap(shared_mem, 1024);
    close(shm_fd);
    unlink("/tmp/at_rx_tmp");

    return 0;
}
