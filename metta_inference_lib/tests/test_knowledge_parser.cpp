#include "metta_inference/knowledge_io.hpp"
#include <iostream>
#include <filesystem>

int main() {
    using namespace metta_inference;
    
    try {
        std::string testFile = "/app/example/2_smart_port_example.metta";
        std::cout << "Testing s-expression based parsing...\n\n";
        
        // Test reading the document
        auto doc = KnowledgeIO::readMettaDocument(testFile);
        
        std::cout << "Norms found: " << doc.norms.size() << "\n";
        for (const auto& norm : doc.norms) {
            std::cout << "  - Norm: " << norm.name << " with " 
                      << norm.parameters.size() << " parameters and "
                      << norm.conditions.size() << " conditions\n";
        }
        
        std::cout << "\nTriples found: " << doc.stateOfAffairs.facts.size() << "\n";
        for (const auto& triple : doc.stateOfAffairs.facts) {
            std::cout << "  - " << triple.tripleType << ": (" 
                      << triple.subject << " " << triple.predicate 
                      << " " << triple.object << ")\n";
        }
        
        std::cout << "\nEventualities found: " << doc.stateOfAffairs.eventualities.size() << "\n";
        for (const auto& [name, eventuality] : doc.stateOfAffairs.eventualities) {
            std::cout << "  - " << name << ": type=" << eventuality.type 
                      << ", agent=" << eventuality.agent << "\n";
        }
        
        std::cout << "\n✅ S-expression parsing successful!\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}