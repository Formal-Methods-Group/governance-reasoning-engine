#include "metta_inference/inference_engine.hpp"
#include "metta_inference/process_executor.hpp"
#include "metta_inference/module_loader.hpp"
#include "metta_inference/formatters.hpp"
#include "metta_inference/semantic_analyzer.hpp"
#include "metta_inference/sexpr_parser.hpp"
#include "metta_inference/entity_resolver.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <optional>

namespace metta_inference {
namespace fs = std::filesystem;

class InferenceEngineV2 : public InferenceEngine {
public:
    InferenceEngineV2(const Config& config) : InferenceEngine(config) {
        // Initialize configuration-driven components
        initializeConfiguration();
    }
    
    InferenceEngine::Result run(const fs::path& exampleFile) override {
        InferenceEngine::Result result;
        
        try {
            // Validate and prepare
            result = prepareExecution(exampleFile);
            if (!result.rawOutput.empty()) {
                return result;  // Early return on preparation failure
            }
            
            // Execute MeTTa inference
            auto execResult = executeMettaInference(exampleFile);
            result.rawOutput = execResult.output;
            
            // Perform semantic analysis instead of regex parsing
            result.metrics = analyzeOutput(result.rawOutput);
            result.hasLogicalIssues = (result.metrics.conflicts > 0 || result.metrics.violations > 0);
            
            // Format results
            result.formattedOutput = formatResults(result.metrics, result.rawOutput, exampleFile);
            
        } catch (const std::exception& e) {
            cleanupTempFiles();
            throw;
        }
        
        return result;
    }
    
private:
    std::unique_ptr<SemanticAnalyzer> analyzer;
    std::unique_ptr<EntityResolver> resolver;
    std::unique_ptr<DescriptionTemplates> templates;
    InferencePatternDetector patternDetector;
    
    void initializeConfiguration() {
        // Load configuration from file if exists
        fs::path configPath = config.outputDir / ".." / "config" / "inference_config.json";
        if (fs::exists(configPath)) {
            auto& inferConfig = InferenceConfiguration::getInstance();
            inferConfig.loadFromFile(configPath.string());
            
            resolver = std::make_unique<EntityResolver>(inferConfig.getEntityResolver());
            templates = std::make_unique<DescriptionTemplates>(inferConfig.getTemplates());
        } else {
            // Use defaults
            resolver = std::make_unique<EntityResolver>();
            templates = std::make_unique<DescriptionTemplates>();
        }
        
        analyzer = std::make_unique<SemanticAnalyzer>(resolver.get(), templates.get());
    }
    
    InferenceEngine::Result prepareExecution(const fs::path& /* exampleFile */) {
        InferenceEngine::Result result;
        
        if (config.verbose) {
            std::cout << "  [V2] Validating module directories... ";
        }
        
        auto modules = validateModulesWithErrorHandling();
        if (!modules) {
            result.rawOutput = "Error: Failed to validate modules";
            return result;
        }
        
        if (config.verbose) {
            std::cout << "✓\n";
            displayModuleSummary(*modules);
        }
        
        return result;
    }
    
    std::optional<std::vector<ModuleLoader::ModuleInfo>> validateModulesWithErrorHandling() {
        try {
            return ModuleLoader::validateModules(config.modulePaths);
        } catch (const std::exception& e) {
            std::cerr << "Module validation error: " << e.what() << std::endl;
            return std::nullopt;
        }
    }
    
    void displayModuleSummary(const std::vector<ModuleLoader::ModuleInfo>& modules) {
        std::cout << "  Module summary:\n";
        for (const auto& module : modules) {
            std::cout << "    " << module.path.filename() << ": "
                     << module.files.size() << " files ("
                     << module.totalSize << " bytes)\n";
        }
    }
    
    ProcessExecutor::ExecutionResult executeMettaInference(const fs::path& exampleFile) {
        if (config.verbose) {
            std::cout << "  [V2] Creating combined file... ";
        }
        
        fs::path tempFile = createCombinedFileWithValidation(exampleFile);
        
        if (config.verbose) {
            std::cout << "✓\n";
            std::cout << "  [V2] Running MeTTa inference engine... ";
        }
        
        std::string runCmd = "\"" + config.mettaReplPath.string() + "\" \"" + 
                           tempFile.string() + "\" 2>&1";
        
        auto execResult = ProcessExecutor::execute(runCmd,
            std::chrono::milliseconds(Constants::DEFAULT_TIMEOUT_SECONDS * 1000));
        
        validateExecutionResult(execResult, tempFile);
        
        fs::remove(tempFile);
        
        if (config.verbose) {
            std::cout << "✓ (" << execResult.duration.count() << "ms)\n";
        }
        
        return execResult;
    }
    
    fs::path createCombinedFileWithValidation(const fs::path& exampleFile) {
        try {
            return ModuleLoader::createCombinedFile(config.modulePaths, exampleFile, config.verbose);
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to create combined file: " + std::string(e.what()));
        }
    }
    
    void validateExecutionResult(const ProcessExecutor::ExecutionResult& execResult, const fs::path& tempFile) {
        if (execResult.exitCode != 0) {
            fs::remove(tempFile);
            throw std::runtime_error("Inference engine failed with exit code: " +
                                   std::to_string(execResult.exitCode) + 
                                   "\nOutput: " + execResult.output);
        }
    }
    
    Metrics analyzeOutput(const std::string& output) {
        if (config.verbose) {
            std::cout << "  [V2] Performing semantic analysis... ";
        }
        
        // Use semantic analyzer instead of regex patterns
        auto analysisResult = analyzer->analyze(output);
        
        if (config.verbose) {
            std::cout << "✓\n";
            displayAnalysisPreview(analysisResult);
        }
        
        return analysisResult.toMetrics();
    }
    
    void displayAnalysisPreview(const SemanticAnalyzer::AnalysisResult& result) {
        std::cout << "    Analysis preview:\n";
        if (!result.inferredFacts.empty()) {
            std::cout << "      - Inferred facts: " << result.inferredFacts.size() << "\n";
        }
        if (!result.contradictions.empty()) {
            std::cout << "      - Contradictions: " << result.contradictions.size() << "\n";
        }
        if (!result.conflicts.empty()) {
            std::cout << "      - Conflicts: " << result.conflicts.size() << "\n";
        }
        if (!result.violations.empty()) {
            std::cout << "      - Violations: " << result.violations.size() << "\n";
        }
        if (!result.compliances.empty()) {
            std::cout << "      - Compliances: " << result.compliances.size() << "\n";
        }
    }
    
    std::string formatResults(const Metrics& metrics, const std::string& rawOutput,
                             const fs::path& exampleFile) {
        auto formatter = FormatterFactory::create(config.outputFormat);
        return formatter->format(config, metrics, rawOutput, exampleFile.stem().string());
    }
    
    void cleanupTempFiles() {
        fs::path tempFile = fs::temp_directory_path() /
                          ("metta_combined_" + std::to_string(getpid()) + ".metta");
        if (fs::exists(tempFile)) {
            try {
                fs::remove(tempFile);
            } catch (...) {
                // Ignore cleanup errors
            }
        }
    }
    
    // Enhanced error handling methods
    class InferenceError : public std::runtime_error {
    public:
        enum class Type {
            ModuleValidation,
            FileCreation,
            Execution,
            Analysis,
            Configuration
        };
        
        InferenceError(Type type, const std::string& message)
            : std::runtime_error(message), errorType(type) {}
        
        Type getType() const { return errorType; }
        
    private:
        Type errorType;
    };
    
    void handleError(const InferenceError& error) {
        std::cerr << "\n[ERROR] ";
        switch (error.getType()) {
            case InferenceError::Type::ModuleValidation:
                std::cerr << "Module validation failed: ";
                break;
            case InferenceError::Type::FileCreation:
                std::cerr << "File operation failed: ";
                break;
            case InferenceError::Type::Execution:
                std::cerr << "Execution failed: ";
                break;
            case InferenceError::Type::Analysis:
                std::cerr << "Analysis failed: ";
                break;
            case InferenceError::Type::Configuration:
                std::cerr << "Configuration error: ";
                break;
        }
        std::cerr << error.what() << std::endl;
    }
};

// Factory method to create the improved inference engine
std::unique_ptr<InferenceEngine> createInferenceEngineV2(const Config& config) {
    return std::make_unique<InferenceEngineV2>(config);
}

}