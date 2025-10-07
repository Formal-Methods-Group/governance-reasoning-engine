#include "metta_inference/knowledge_io.hpp"
#include <iostream>
#include <filesystem>

namespace mi = metta_inference;
namespace fs = std::filesystem;

void printSeparator(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n\n";
}

int main(int argc, char* argv[]) {
    try {
        // Test file path
        fs::path testFile = (argc > 1) ? fs::path(argv[1]) 
                          : fs::path("/app/example/2_smart_port_example.metta");
        
        std::cout << "Testing Knowledge I/O functionality with: " << testFile << "\n";
        
        if (!fs::exists(testFile)) {
            std::cerr << "Error: Test file does not exist: " << testFile << "\n";
            return 1;
        }
        
        // Test 1: Read complete document
        printSeparator("Test 1: Reading Complete MeTTa Document");
        auto doc = mi::KnowledgeIO::readMettaDocument(testFile);
        
        std::cout << "Found " << doc.norms.size() << " norms\n";
        std::cout << "Found " << doc.stateOfAffairs.facts.size() << " state of affairs facts\n";
        
        // Test 2: Display extracted norms
        printSeparator("Test 2: Extracted Norms");
        int normCount = 0;
        for (const auto& norm : doc.norms) {
            std::cout << "Norm #" << ++normCount << ": " << norm.name << "\n";
            std::cout << "  Parameters: ";
            for (const auto& param : norm.parameters) {
                std::cout << param << " ";
            }
            std::cout << "\n";
            std::cout << "  Conditions: " << norm.conditions.size() << "\n";
            std::cout << "  Consequences: " << norm.consequences.size() << "\n\n";
        }
        
        // Test 3: Display state of affairs
        printSeparator("Test 3: State of Affairs Facts");
        for (const auto& fact : doc.stateOfAffairs.facts) {
            std::cout << "  " << fact.toString() << "\n";
        }
        
        // Test 4: Write norms to separate file
        printSeparator("Test 4: Writing Norms to File");
        fs::path normsOutput = "./test_norms_output.metta";
        mi::KnowledgeIO::writeNormsToFile(doc.norms, normsOutput);
        std::cout << "Norms written to: " << normsOutput << "\n";
        
        // Test 5: Write state of affairs to separate file
        printSeparator("Test 5: Writing State of Affairs to File");
        fs::path soaOutput = "./test_soa_output.metta";
        mi::KnowledgeIO::writeStateOfAffairsToFile(doc.stateOfAffairs, soaOutput);
        std::cout << "State of affairs written to: " << soaOutput << "\n";
        
        // Test 6: Write complete document
        printSeparator("Test 6: Writing Complete Document");
        fs::path completeOutput = "./test_complete_output.metta";
        mi::KnowledgeIO::writeMettaDocument(doc, completeOutput);
        std::cout << "Complete document written to: " << completeOutput << "\n";
        
        // Test 7: Create new norm programmatically
        printSeparator("Test 7: Creating New Norm Programmatically");
        mi::Norm newNorm;
        newNorm.name = "test-norm";
        newNorm.parameters = {"$agent", "$action"};
        newNorm.description = "Test norm created programmatically";
        
        mi::Condition cond1;
        cond1.variable = "True";
        cond1.expression = "ct-triple $agent type Agent";
        newNorm.conditions.push_back(cond1);
        
        mi::Triple consequence;
        consequence.subject = "$agent";
        consequence.predicate = "can-perform";
        consequence.object = "$action";
        newNorm.consequences.push_back(consequence);
        
        std::cout << "Created norm:\n" << newNorm.toString() << "\n";
        
        // Test 8: Create new state of affairs programmatically
        printSeparator("Test 8: Creating New State of Affairs");
        mi::StateOfAffairs newSoa;
        newSoa.description = "Test state of affairs";
        
        mi::Triple fact1;
        fact1.subject = "test_agent";
        fact1.predicate = "type";
        fact1.object = "Agent";
        newSoa.facts.push_back(fact1);
        
        mi::Triple fact2;
        fact2.subject = "test_agent";
        fact2.predicate = "located-at";
        fact2.object = "test_location";
        newSoa.facts.push_back(fact2);
        
        std::cout << "Created state of affairs:\n" << newSoa.toString() << "\n";
        
        // Test 9: Write the new structures to file
        printSeparator("Test 9: Writing New Structures");
        std::vector<mi::Norm> newNorms = {newNorm};
        mi::KnowledgeIO::writeNormsToFile(newNorms, "./test_new_norm.metta");
        mi::KnowledgeIO::writeStateOfAffairsToFile(newSoa, "./test_new_soa.metta");
        std::cout << "New structures written successfully\n";
        
        std::cout << "\n=== All tests completed successfully! ===\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred\n";
        return 1;
    }
    
    return 0;
}