#ifndef METTA_INFERENCE_CONFIG_HPP
#define METTA_INFERENCE_CONFIG_HPP

#include <filesystem>
#include <vector>
#include <string>
#include <atomic>
#include <cstdlib>  // for std::getenv

namespace metta_inference {

namespace fs = std::filesystem;

enum class OutputFormat {
    Pretty,
    JSON,
    CSV,
    Markdown
};

// Configuration constants
struct Constants {
    static constexpr int DEFAULT_TIMEOUT_SECONDS = 3600;
    static constexpr size_t MAX_FILE_SIZE_MB = 100;
    static constexpr size_t INITIAL_OUTPUT_RESERVE_SIZE = 65536;
};

struct Config {
    bool verbose = false;
    OutputFormat outputFormat = OutputFormat::Pretty;
    bool saveOutput = false;
    fs::path outputDir = "./inference_results";
    bool showRaw = false;
    fs::path exampleFile;
    
    std::vector<fs::path> modulePaths;
    fs::path mettaReplPath;
    
    Config() {
        // Use environment variables with fallback defaults
        const char* mettaBase = std::getenv("METTA_BASE_PATH");
        if (mettaBase) {
            fs::path basePath(mettaBase);
            modulePaths = {
                basePath / "base",
                basePath / "knowledge",
                basePath / "reason"
            };
        } else {
            // Fallback to relative paths
            modulePaths = {
                fs::path("../base"),
                fs::path("../knowledge"),
                fs::path("../reason")
            };
        }
        
        const char* replPath = std::getenv("METTA_REPL_PATH");
        if (replPath) {
            mettaReplPath = fs::path(replPath);
        } else {
            // Fallback to common location
            mettaReplPath = fs::path("metta-repl");
        }
    }
};

struct ConflictDetail {
    std::string entity1;
    std::string entity2;
    std::string description;
};

struct ViolationDetail {
    std::string violator;
    std::string violated_rule;
    std::string description;
};

struct ContradictionDetail {
    std::string entity1;
    std::string entity2;
    std::string description;
};

struct Metrics {
    int contradictions = 0;
    int contradictionPairs = 0;  // Unique contradiction pairs
    int compliances = 0;
    int conflicts = 0;
    int violations = 0;
    int inferredFacts = 0;  // New facts inferred by the engine
    std::vector<std::string> inferredStateOfAffairs;  // List of inferred SOA facts
    
    // Detailed relationship information
    std::vector<ConflictDetail> conflictDetails;
    std::vector<ViolationDetail> violationDetails;
    std::vector<ContradictionDetail> contradictionDetails;
    
    int total() const {
        return contradictionPairs + compliances + conflicts + violations + inferredFacts;
    }
    
    bool hasPositiveInferences() const {
        return inferredFacts > 0 || compliances > 0;
    }
    
    bool hasNegativeInferences() const {
        return contradictions > 0 || conflicts > 0 || violations > 0;
    }
};

}

#endif