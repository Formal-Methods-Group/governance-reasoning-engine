#include "metta_inference/inference_engine.hpp"
#include "metta_inference/config.hpp"
#include <iostream>

namespace mi = metta_inference;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <metta_file>\n";
        return 1;
    }
    
    try {
        // Configure inference
        mi::Config config;
        config.exampleFile = argv[1];
        config.outputFormat = mi::OutputFormat::Pretty;
        config.verbose = true;
        
        // Run inference with V2 engine (S-expression parsing)
        auto engine = mi::createInferenceEngineV2(config);
        auto result = engine->run(config.exampleFile);
        
        // Display results
        std::cout << result.formattedOutput << "\n";
        
        // Return exit code based on logical issues
        return result.hasLogicalIssues ? 2 : 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}