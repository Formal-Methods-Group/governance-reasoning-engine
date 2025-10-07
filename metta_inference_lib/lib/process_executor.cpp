#include "metta_inference/process_executor.hpp"
#include <sys/wait.h>  // For WEXITSTATUS
#include <sys/select.h> // For select()
#include <fcntl.h>      // For fcntl()
#include <unistd.h>     // For fileno()
#include <signal.h>     // For kill()
#include <cstring>      // For strerror()

namespace metta_inference {

ProcessExecutor::ExecutionResult ProcessExecutor::execute(
    const std::string& command,
    std::optional<std::chrono::milliseconds> timeout) {
    
    auto startTime = std::chrono::steady_clock::now();
    ExecutionResult result;
    
    // Pre-allocate space for output to reduce allocations
    result.output.reserve(65536);  // Reserve 64KB initially
    
    FILE* rawPipe = popen(command.c_str(), "r");
    if (!rawPipe) {
        throw std::runtime_error("Failed to execute command: " + command);
    }
    
    // Get file descriptor and set to non-blocking mode
    int fd = fileno(rawPipe);
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    
    std::array<char, BUFFER_SIZE> buffer;
    fd_set readfds;
    struct timeval tv;
    bool timedOut = false;
    
    while (true) {
        if (timeout) {
            auto elapsed = std::chrono::steady_clock::now() - startTime;
            if (elapsed > *timeout) {
                timedOut = true;
                break;
            }
            
            // Calculate remaining timeout for select
            auto remaining = *timeout - elapsed;
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(remaining).count();
            auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
                remaining - std::chrono::seconds(seconds)).count();
            
            tv.tv_sec = seconds;
            tv.tv_usec = microseconds;
        } else {
            // No timeout, wait indefinitely
            tv.tv_sec = 60;  // Check every minute anyway
            tv.tv_usec = 0;
        }
        
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        
        int selectResult = select(fd + 1, &readfds, nullptr, nullptr, &tv);
        
        if (selectResult < 0) {
            // Error in select
            pclose(rawPipe);
            throw std::runtime_error("Error waiting for command output: " + std::string(strerror(errno)));
        } else if (selectResult == 0) {
            // Timeout occurred in select
            if (timeout) {
                auto elapsed = std::chrono::steady_clock::now() - startTime;
                if (elapsed > *timeout) {
                    timedOut = true;
                    break;
                }
            }
            continue;  // Continue waiting
        } else {
            // Data is available to read
            ssize_t bytesRead = read(fd, buffer.data(), buffer.size() - 1);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                result.output += buffer.data();
            } else if (bytesRead == 0) {
                // EOF reached
                break;
            } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                // Real error occurred
                pclose(rawPipe);
                throw std::runtime_error("Error reading command output: " + std::string(strerror(errno)));
            }
        }
    }
    
    if (timedOut) {
        // Kill the process if it timed out
        // Note: This is a best-effort attempt, popen doesn't give us the PID directly
        pclose(rawPipe);
        throw std::runtime_error("Command timed out");
    }
    
    // Get exit code
    int status = pclose(rawPipe);
    result.exitCode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    result.duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - startTime
    );
    
    return result;
}

}