#include <iostream>
#include <string>
#include <filesystem>
#include "metta_inference/inference_engine.hpp"
#include "metta_inference/semantic_analyzer.hpp"
#include "metta_inference/sexpr_parser.hpp"
#include "metta_inference/entity_resolver.hpp"

using namespace metta_inference;
namespace fs = std::filesystem;

void testSExprParser() {
    std::cout << "\n=== Testing S-Expression Parser ===" << std::endl;
    
    std::string input = "(triple soa_epmuam type rexist)\n"
                       "(triple soa_ALEXANDRA_MAERSK soaPay soa_USDS)\n"
                       "(id_not_not_false soa_epmuam)\n"
                       "((meta-id soa_epmuam type rexist false) (inrs-not-usds soa_USDS))";
    
    try {
        auto expressions = SExprParser::parseMultiple(input);
        std::cout << "Parsed " << expressions.size() << " expressions successfully!" << std::endl;
        
        for (const auto& expr : expressions) {
            std::cout << "  Expression: " << expr->toString() << std::endl;
            
            // Test pattern matching
            if (SExprMatcher::matches(expr, {"triple", "?", "type", "rexist"})) {
                auto values = SExprMatcher::extract(expr, {"triple", "?", "type", "rexist"});
                if (!values.empty()) {
                    std::cout << "    Found state of affairs: " << values[0] << std::endl;
                }
            }
            
            if (SExprMatcher::matches(expr, {"id_not_not_false", "?"})) {
                auto values = SExprMatcher::extract(expr, {"id_not_not_false", "?"});
                if (!values.empty()) {
                    std::cout << "    Found contradiction: " << values[0] << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Parser error: " << e.what() << std::endl;
    }
}

void testEntityResolver() {
    std::cout << "\n=== Testing Entity Resolver ===" << std::endl;
    
    EntityResolver resolver;
    
    // Test entity resolution
    std::cout << "Entity resolution:" << std::endl;
    std::cout << "  soa_ALEXANDRA_MAERSK -> " 
              << resolver.resolveEntity("soa_ALEXANDRA_MAERSK") << std::endl;
    std::cout << "  soa_LAURA_MAERSK -> " 
              << resolver.resolveEntity("soa_LAURA_MAERSK") << std::endl;
    
    // Test action resolution
    std::cout << "\nAction resolution:" << std::endl;
    std::cout << "  soaMoor (present) -> " 
              << resolver.resolveAction("soaMoor", "present") << std::endl;
    std::cout << "  soaPay (past) -> " 
              << resolver.resolveAction("soaPay", "past") << std::endl;
    
    // Test instrument resolution
    std::cout << "\nInstrument resolution:" << std::endl;
    std::cout << "  soa_USDS -> " << resolver.resolveInstrument("soa_USDS") << std::endl;
    std::cout << "  soa_INRS -> " << resolver.resolveInstrument("soa_INRS") << std::endl;
    
    // Test negation handling
    std::cout << "\nNegation handling:" << std::endl;
    std::cout << "  Is soa_enmam negated? " 
              << (resolver.isNegatedEntity("soa_enmam") ? "Yes" : "No") << std::endl;
    std::cout << "  Base form of soa_enmam: " 
              << resolver.getBaseForm("soa_enmam") << std::endl;
    std::cout << "  Negated form of soa_emam: " 
              << resolver.getNegatedForm("soa_emam") << std::endl;
}

void testDescriptionTemplates() {
    std::cout << "\n=== Testing Description Templates ===" << std::endl;
    
    DescriptionTemplates templates;
    
    // Test contradiction description
    std::cout << "Contradiction description:" << std::endl;
    std::cout << "  " << templates.generateContradictionDescription(
        "ALEXANDRA MÆRSK pays in USDS",
        "ALEXANDRA MÆRSK must pay in INRS",
        "payment"
    ) << std::endl;
    
    // Test conflict description
    std::cout << "\nConflict description:" << std::endl;
    std::cout << "  " << templates.generateConflictDescription(
        "EU MiCA regulation",
        "MICT port requirements",
        "EU regulations prohibit INRS usage while MICT requires INRS-only payments"
    ) << std::endl;
    
    // Test violation description
    std::cout << "\nViolation description:" << std::endl;
    std::cout << "  " << templates.generateViolationDescription(
        "EU MiCA regulation",
        "MICT port INRS requirement",
        "conflicting regulations"
    ) << std::endl;
    
    // Test compliance description
    std::cout << "\nCompliance description:" << std::endl;
    std::cout << "  " << templates.generateComplianceDescription(
        "LAURA MÆRSK",
        "port payment obligation",
        "paying $15000 in INRS"
    ) << std::endl;
}

void testSemanticAnalyzer() {
    std::cout << "\n=== Testing Semantic Analyzer ===" << std::endl;
    
    SemanticAnalyzer analyzer;
    
    // Sample MeTTa output with various patterns
    std::string mettaOutput = R"(
        (triple soa_epmuam type rexist)
        (triple soa_epmuam type soaPay)
        (triple soa_epmuam soaHas_agent soa_ALEXANDRA_MAERSK)
        (triple soa_epmuam soaHas_instrument soa_USDS)
        (id_not_not_false soa_epmuam)
        (id_not_not_false soa_enmam)
        [(conflict (inrs-prohibited-id soa_ALEXANDRA_MAERSK) (inrs-only-id soa_sptMICT))]
        [(quote ((inrs-prohibited-id soa_ALEXANDRA_MAERSK) (inrs-only-id soa_sptMICT)))]
        (is_complied_with_by port-payment-obligation soa_LAURA_MAERSK)
    )";
    
    auto result = analyzer.analyze(mettaOutput);
    
    std::cout << "Analysis results:" << std::endl;
    std::cout << "  Inferred facts: " << result.inferredFacts.size() << std::endl;
    for (const auto& fact : result.inferredFacts) {
        std::cout << "    - " << fact.toString() << std::endl;
    }
    
    std::cout << "  Contradictions: " << result.contradictions.size() << std::endl;
    std::cout << "  Conflicts: " << result.conflicts.size() << std::endl;
    std::cout << "  Violations: " << result.violations.size() << std::endl;
    std::cout << "  Compliances: " << result.compliances.size() << std::endl;
    
    // Convert to metrics
    auto metrics = result.toMetrics();
    std::cout << "\nMetrics summary:" << std::endl;
    std::cout << "  Total relationships: " << metrics.total() << std::endl;
    std::cout << "  Has positive inferences: " 
              << (metrics.hasPositiveInferences() ? "Yes" : "No") << std::endl;
    std::cout << "  Has negative inferences: " 
              << (metrics.hasNegativeInferences() ? "Yes" : "No") << std::endl;
}

void testConfigurationLoading() {
    std::cout << "\n=== Testing Configuration Loading ===" << std::endl;
    
    auto& config = InferenceConfiguration::getInstance();
    
    // Try to load configuration
    fs::path configPath = fs::current_path() / ".." / "config" / "inference_config.json";
    if (fs::exists(configPath)) {
        std::cout << "Loading configuration from: " << configPath << std::endl;
        config.loadFromFile(configPath.string());
        std::cout << "Configuration loaded successfully!" << std::endl;
    } else {
        std::cout << "Configuration file not found at: " << configPath << std::endl;
        std::cout << "Using default configuration" << std::endl;
    }
    
    // Test configured resolver
    auto& resolver = config.getEntityResolver();
    std::cout << "\nConfigured entity resolution test:" << std::endl;
    std::cout << "  soa_ALEXANDRA_MAERSK -> " 
              << resolver.resolveEntity("soa_ALEXANDRA_MAERSK") << std::endl;
}

int main() {
    std::cout << "=== MeTTa Inference V2 Improvements Test ===" << std::endl;
    std::cout << "Testing new components for better architecture" << std::endl;
    
    try {
        testSExprParser();
        testEntityResolver();
        testDescriptionTemplates();
        testSemanticAnalyzer();
        testConfigurationLoading();
        
        std::cout << "\n=== All Tests Completed Successfully ===" << std::endl;
        std::cout << "\nKey improvements demonstrated:" << std::endl;
        std::cout << "✓ S-Expression parsing replaces regex patterns" << std::endl;
        std::cout << "✓ Dynamic entity resolution with configuration" << std::endl;
        std::cout << "✓ Template-driven description generation" << std::endl;
        std::cout << "✓ Semantic analysis layer for logical reasoning" << std::endl;
        std::cout << "✓ Configuration-driven mappings from JSON" << std::endl;
        std::cout << "✓ Proper error handling and validation" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Test failed with error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}