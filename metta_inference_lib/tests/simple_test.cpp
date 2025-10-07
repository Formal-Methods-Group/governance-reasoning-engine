#include <iostream>
#include <string>
#include "metta_inference/inference_engine.hpp"

using namespace metta_inference;

int main() {
    InferenceEngine engine;
    InferenceConfiguration config;
    
    // Create a mock result with the compliance output
    InferenceEngine::InferenceResult mockResult;
    mockResult.rawOutput = "[()]\n[(soa_enpam soa_epam15k)]";
    
    // Analyze the output
    SemanticAnalyzer analyzer;
    auto analysis = analyzer.analyze(mockResult.rawOutput);
    mockResult.metrics = analysis.toMetrics();
    
    // Display results
    std::cout << "Raw output: " << mockResult.rawOutput << std::endl;
    std::cout << "Compliance relations found: " << mockResult.metrics.compliances << std::endl;
    
    if (mockResult.metrics.compliances > 0) {
        std::cout << "SUCCESS: Compliance relationship detected!" << std::endl;
        for (const auto& comp : analysis.compliances) {
            std::cout << "  - " << comp.obligation << " is complied with by " << comp.entity << std::endl;
        }
    } else {
        std::cout << "FAILED: No compliance relationships found" << std::endl;
    }
    
    return 0;
}