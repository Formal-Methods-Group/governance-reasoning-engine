#ifndef METTA_INFERENCE_INFERENCE_ENGINE_HPP
#define METTA_INFERENCE_INFERENCE_ENGINE_HPP

#include "config.hpp"
#include <string>
#include <filesystem>
#include <future>
#include <vector>
#include <utility>

namespace metta_inference {

class InferenceEngine {
public:
    InferenceEngine(const Config& config);
    virtual ~InferenceEngine() = default;
    
    struct Result {
        std::string rawOutput;
        Metrics metrics;
        std::string formattedOutput;
        bool hasLogicalIssues;
    };
    
    virtual Result run(const std::filesystem::path& exampleFile);
    
protected:
    Config config;
};

// Factory function to create the improved V2 inference engine with S-expression parsing
std::unique_ptr<InferenceEngine> createInferenceEngineV2(const Config& config);

}

#endif