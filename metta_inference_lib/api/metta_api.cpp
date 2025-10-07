#include "metta_api.hpp"
#include "metta_inference/inference_engine.hpp"
#include "metta_inference/config.hpp"
#include <chrono>
#include <fstream>
#include <sstream>

namespace metta_api {

namespace fs = std::filesystem;
namespace mi = metta_inference;

class MettaAPI::Impl {
public:
    mi::Config config;
    
    Impl() {
        config.outputFormat = mi::OutputFormat::JSON;
    }
};

MettaAPI::MettaAPI() : pImpl(std::make_unique<Impl>()) {}
MettaAPI::~MettaAPI() = default;

void MettaAPI::setMettaReplPath(const std::string& path) {
    pImpl->config.mettaReplPath = path;
}

void MettaAPI::setDefaultModulePaths(const std::vector<std::string>& paths) {
    pImpl->config.modulePaths.clear();
    for (const auto& path : paths) {
        pImpl->config.modulePaths.push_back(fs::path(path));
    }
}

void MettaAPI::setVerbose(bool verbose) {
    pImpl->config.verbose = verbose;
}

InferenceResponse MettaAPI::runInference(const InferenceRequest& request) {
    InferenceResponse response;
    auto startTime = std::chrono::steady_clock::now();
    
    try {
        // Create temporary file with example content
        fs::path tempFile = fs::temp_directory_path() / 
                           ("metta_api_example_" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + ".metta");
        
        std::ofstream outFile(tempFile);
        if (!outFile.is_open()) {
            response.error = "Failed to create temporary file";
            return response;
        }
        outFile << request.exampleContent;
        outFile.close();
        
        // Set up config
        auto localConfig = pImpl->config;
        localConfig.exampleFile = tempFile;
        localConfig.verbose = request.verbose;
        
        if (!request.modulePaths.empty()) {
            localConfig.modulePaths.clear();
            for (const auto& path : request.modulePaths) {
                localConfig.modulePaths.push_back(fs::path(path));
            }
        }
        
        if (request.outputFormat == "pretty") {
            localConfig.outputFormat = mi::OutputFormat::Pretty;
        } else if (request.outputFormat == "json") {
            localConfig.outputFormat = mi::OutputFormat::JSON;
        } else if (request.outputFormat == "csv") {
            localConfig.outputFormat = mi::OutputFormat::CSV;
        } else if (request.outputFormat == "markdown") {
            localConfig.outputFormat = mi::OutputFormat::Markdown;
        }
        
        // Run inference with V2 engine (S-expression parsing)
        auto engine = mi::createInferenceEngineV2(localConfig);
        auto result = engine->run(tempFile);
        
        // Clean up temp file
        fs::remove(tempFile);
        
        // Fill response
        response.success = true;
        response.metrics.contradictions = result.metrics.contradictions;
        response.metrics.compliances = result.metrics.compliances;
        response.metrics.conflicts = result.metrics.conflicts;
        response.metrics.violations = result.metrics.violations;
        response.formattedOutput = result.formattedOutput;
        response.rawOutput = result.rawOutput;
        response.hasLogicalIssues = result.hasLogicalIssues;
        
        auto endTime = std::chrono::steady_clock::now();
        response.processingTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            endTime - startTime).count();
        
    } catch (const std::exception& e) {
        response.error = e.what();
        response.success = false;
    }
    
    return response;
}

InferenceResponse MettaAPI::runInferenceFromFile(const std::string& filePath, 
                                                 const InferenceRequest& request) {
    InferenceResponse response;
    auto startTime = std::chrono::steady_clock::now();
    
    try {
        // Validate file exists
        if (!fs::exists(filePath)) {
            response.error = "File not found: " + filePath;
            return response;
        }
        
        // Set up config
        auto localConfig = pImpl->config;
        localConfig.exampleFile = fs::path(filePath);
        localConfig.verbose = request.verbose;
        
        if (!request.modulePaths.empty()) {
            localConfig.modulePaths.clear();
            for (const auto& path : request.modulePaths) {
                localConfig.modulePaths.push_back(fs::path(path));
            }
        }
        
        if (!request.outputFormat.empty()) {
            if (request.outputFormat == "pretty") {
                localConfig.outputFormat = mi::OutputFormat::Pretty;
            } else if (request.outputFormat == "json") {
                localConfig.outputFormat = mi::OutputFormat::JSON;
            } else if (request.outputFormat == "csv") {
                localConfig.outputFormat = mi::OutputFormat::CSV;
            } else if (request.outputFormat == "markdown") {
                localConfig.outputFormat = mi::OutputFormat::Markdown;
            }
        }
        
        // Run inference with V2 engine (S-expression parsing)
        auto engine = mi::createInferenceEngineV2(localConfig);
        auto result = engine->run(localConfig.exampleFile);
        
        // Fill response
        response.success = true;
        response.metrics.contradictions = result.metrics.contradictions;
        response.metrics.compliances = result.metrics.compliances;
        response.metrics.conflicts = result.metrics.conflicts;
        response.metrics.violations = result.metrics.violations;
        response.formattedOutput = result.formattedOutput;
        response.rawOutput = result.rawOutput;
        response.hasLogicalIssues = result.hasLogicalIssues;
        
        auto endTime = std::chrono::steady_clock::now();
        response.processingTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            endTime - startTime).count();
        
    } catch (const std::exception& e) {
        response.error = e.what();
        response.success = false;
    }
    
    return response;
}

bool MettaAPI::validateModulePath(const std::string& path) const {
    return fs::exists(path) && fs::is_directory(path);
}

bool MettaAPI::validateMettaReplPath(const std::string& path) const {
    if (!fs::exists(path) || !fs::is_regular_file(path)) {
        return false;
    }
    
    std::error_code ec;
    auto perms = fs::status(path, ec).permissions();
    if (ec) return false;
    
    return (perms & fs::perms::owner_exec) != fs::perms::none ||
           (perms & fs::perms::group_exec) != fs::perms::none ||
           (perms & fs::perms::others_exec) != fs::perms::none;
}

std::vector<std::string> MettaAPI::listMettaFiles(const std::string& directory) const {
    std::vector<std::string> files;
    
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        return files;
    }
    
    std::error_code ec;
    for (const auto& entry : fs::directory_iterator(directory, ec)) {
        if (ec) continue;
        
        if (entry.is_regular_file() && entry.path().extension() == ".metta") {
            files.push_back(entry.path().string());
        }
    }
    
    return files;
}

BatchProcessor::BatchProcessor(MettaAPI& api) : api(api) {}

std::vector<BatchProcessor::BatchResult> BatchProcessor::processDirectory(
    const std::string& directory,
    const InferenceRequest& baseRequest) {
    
    auto files = api.listMettaFiles(directory);
    return processFiles(files, baseRequest);
}

std::vector<BatchProcessor::BatchResult> BatchProcessor::processFiles(
    const std::vector<std::string>& files,
    const InferenceRequest& baseRequest) {
    
    std::vector<BatchResult> results;
    
    for (const auto& file : files) {
        BatchResult result;
        result.filename = fs::path(file).filename().string();
        result.response = api.runInferenceFromFile(file, baseRequest);
        results.push_back(result);
    }
    
    return results;
}

}