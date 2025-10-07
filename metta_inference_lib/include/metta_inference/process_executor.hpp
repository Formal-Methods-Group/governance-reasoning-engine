#ifndef METTA_INFERENCE_PROCESS_EXECUTOR_HPP
#define METTA_INFERENCE_PROCESS_EXECUTOR_HPP

#include <string>
#include <memory>
#include <chrono>
#include <optional>
#include <array>
#include <cstdio>
#include <stdexcept>

namespace metta_inference {

class ProcessExecutor {
public:
    struct ExecutionResult {
        std::string output;
        int exitCode;
        std::chrono::milliseconds duration;
    };

    static ExecutionResult execute(
        const std::string& command, 
        std::optional<std::chrono::milliseconds> timeout = std::nullopt
    );

private:
    static constexpr size_t BUFFER_SIZE = 16384;  // Increased buffer size for better performance
};

}

#endif