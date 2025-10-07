#include "metta_api.hpp"
#include <iostream>

int main() {
    try {
        // Create API instance
        metta_api::MettaAPI api;
        
        // Configure the API
        api.setVerbose(true);
        
        // Example: Run inference from a file
        metta_api::InferenceRequest request;
        request.outputFormat = "json";
        request.verbose = false;
        
        // You can override module paths if needed
        // request.modulePaths = {"/path/to/base", "/path/to/knowledge"};
        
        // Run inference
        auto response = api.runInferenceFromFile("example.metta", request);
        
        if (response.success) {
            std::cout << "Inference successful!\n";
            std::cout << "Metrics:\n";
            std::cout << "  Contradictions: " << response.metrics.contradictions << "\n";
            std::cout << "  Compliances: " << response.metrics.compliances << "\n";
            std::cout << "  Conflicts: " << response.metrics.conflicts << "\n";
            std::cout << "  Violations: " << response.metrics.violations << "\n";
            std::cout << "  Total: " << response.metrics.total() << "\n";
            std::cout << "  Processing time: " << response.processingTimeMs << "ms\n\n";
            std::cout << "Formatted output:\n" << response.formattedOutput << "\n";
        } else {
            std::cerr << "Inference failed: " << response.error << "\n";
        }
        
        // Example: Batch processing
        metta_api::BatchProcessor batch(api);
        auto results = batch.processDirectory("./examples", request);
        
        std::cout << "\nBatch processing results:\n";
        for (const auto& result : results) {
            std::cout << "  " << result.filename << ": ";
            if (result.response.success) {
                std::cout << "OK (total metrics: " << result.response.metrics.total() << ")\n";
            } else {
                std::cout << "FAILED - " << result.response.error << "\n";
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}