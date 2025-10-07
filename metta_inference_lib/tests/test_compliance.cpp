#include <iostream>
#include "metta_inference/semantic_analyzer.hpp"

int main() {
    // Test with the actual output from compliance example
    std::string rawOutput = "[()]\n[(soa_enpam soa_epam15k)]";
    
    metta_inference::SemanticAnalyzer analyzer;
    auto result = analyzer.analyze(rawOutput);
    
    std::cout << "Compliance relationships found: " << result.compliances.size() << std::endl;
    
    if (!result.compliances.empty()) {
        for (const auto& comp : result.compliances) {
            std::cout << "- Obligation: " << comp.obligation << std::endl;
            std::cout << "  Entity: " << comp.entity << std::endl;
            std::cout << "  Fulfilled by: " << comp.fulfilledBy << std::endl;
        }
    }
    
    return 0;
}