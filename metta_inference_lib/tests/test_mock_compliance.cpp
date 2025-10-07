#include <iostream>
#include <sstream>
#include "metta_inference/inference_engine.hpp"
#include "metta_inference/formatters.hpp"

int main() {
    // Mock the exact output from 6_1_basic_compliance_infer.metta
    metta_inference::InferenceResult result;
    result.rawOutput = "[()]\n[(soa_enpam soa_epam15k)]";
    
    // Use the actual analyzer from the inference engine
    metta_inference::SemanticAnalyzer analyzer;
    auto analysisResult = analyzer.analyze(result.rawOutput);
    result.metrics = analysisResult.toMetrics();
    
    // Format and display like the actual CLI
    metta_inference::InferenceConfiguration config;
    auto formatter = metta_inference::createFormatter(metta_inference::OutputFormat::CLI);
    std::string formatted = formatter->format(config, result.metrics, result.rawOutput, "6_1_basic_compliance_infer");
    
    std::cout << formatted << std::endl;
    
    return 0;
}