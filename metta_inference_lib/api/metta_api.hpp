#ifndef METTA_API_HPP
#define METTA_API_HPP

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <filesystem>

namespace metta_api {

struct InferenceRequest {
    std::string exampleContent;
    std::vector<std::string> modulePaths;
    std::string outputFormat = "json";
    bool verbose = false;
};

struct InferenceMetrics {
    int contradictions = 0;
    int compliances = 0;
    int conflicts = 0;
    int violations = 0;
    
    int total() const {
        return contradictions + compliances + conflicts + violations;
    }
};

struct InferenceResponse {
    bool success = false;
    std::string error;
    InferenceMetrics metrics;
    std::string formattedOutput;
    std::string rawOutput;
    bool hasLogicalIssues = false;
    double processingTimeMs = 0.0;
};

class MettaAPI {
public:
    MettaAPI();
    ~MettaAPI();
    
    void setMettaReplPath(const std::string& path);
    void setDefaultModulePaths(const std::vector<std::string>& paths);
    void setVerbose(bool verbose);
    
    InferenceResponse runInference(const InferenceRequest& request);
    InferenceResponse runInferenceFromFile(const std::string& filePath, 
                                          const InferenceRequest& request = {});
    
    bool validateModulePath(const std::string& path) const;
    bool validateMettaReplPath(const std::string& path) const;
    
    std::vector<std::string> listMettaFiles(const std::string& directory) const;
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

class BatchProcessor {
public:
    BatchProcessor(MettaAPI& api);
    
    struct BatchResult {
        std::string filename;
        InferenceResponse response;
    };
    
    std::vector<BatchResult> processDirectory(const std::string& directory,
                                              const InferenceRequest& baseRequest);
    
    std::vector<BatchResult> processFiles(const std::vector<std::string>& files,
                                         const InferenceRequest& baseRequest);
    
private:
    MettaAPI& api;
};

}

#endif