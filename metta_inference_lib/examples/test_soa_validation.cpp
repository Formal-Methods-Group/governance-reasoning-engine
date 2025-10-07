#include <iostream>
#include <vector>
#include <string>
#include "metta_inference/knowledge_io.hpp"

using namespace metta_inference;

int main() {
    std::cout << "Testing State of Affairs Validation\n";
    std::cout << "====================================\n\n";
    
    // Test 1: Valid state of affairs
    std::cout << "Test 1: Valid State of Affairs\n";
    StateOfAffairs validSoa;
    
    // Create a valid eventuality for ALEXANDRA MÆRSK mooring
    validSoa.facts.push_back(Triple{"soa_emam", "type", "soaMoor", "ct-triple"});
    validSoa.facts.push_back(Triple{"soa_emam", "type", "rexist", "ct-triple"});
    validSoa.facts.push_back(Triple{"soa_emam", "soaHas_agent", "soa_ALEXANDRA_MAERSK", "ct-triple"});
    validSoa.facts.push_back(Triple{"soa_emam", "soaHas_location", "soa_berthMICT", "ct-triple"});
    
    // Parse eventualities from facts
    Eventuality e1;
    e1.name = "soa_emam";
    e1.type = "soaMoor";
    e1.modality = "rexist";
    e1.agent = "soa_ALEXANDRA_MAERSK";
    e1.roles["soaHas_location"] = "soa_berthMICT";
    validSoa.eventualities[e1.name] = e1;
    
    std::vector<std::string> errors;
    if (validSoa.validateEventualities(errors)) {
        std::cout << "✓ Valid state of affairs passed validation\n";
    } else {
        std::cout << "✗ Valid state of affairs failed validation:\n";
        for (const auto& error : errors) {
            std::cout << "  - " << error << "\n";
        }
    }
    
    // Test 2: Invalid modality (not rexist)
    std::cout << "\nTest 2: Invalid Modality\n";
    StateOfAffairs invalidModalitySoa;
    
    Eventuality e2;
    e2.name = "soa_epv";
    e2.type = "soaPay";
    e2.modality = "obligatory";  // Wrong! Should be rexist
    e2.agent = "soa_PORT_VESSEL";
    invalidModalitySoa.eventualities[e2.name] = e2;
    
    errors.clear();
    if (!invalidModalitySoa.validateEventualities(errors)) {
        std::cout << "✓ Correctly detected invalid modality:\n";
        for (const auto& error : errors) {
            std::cout << "  - " << error << "\n";
        }
    } else {
        std::cout << "✗ Failed to detect invalid modality\n";
    }
    
    // Test 3: Invalid naming convention
    std::cout << "\nTest 3: Invalid Naming Convention\n";
    StateOfAffairs invalidNameSoa;
    
    Eventuality e3;
    e3.name = "soa_wrongname";  // Should be soa_emam for Moor + ALEXANDRA_MAERSK
    e3.type = "soaMoor";
    e3.modality = "rexist";
    e3.agent = "soa_ALEXANDRA_MAERSK";
    invalidNameSoa.eventualities[e3.name] = e3;
    
    errors.clear();
    if (!invalidNameSoa.validateEventualities(errors)) {
        std::cout << "✓ Correctly detected invalid naming:\n";
        for (const auto& error : errors) {
            std::cout << "  - " << error << "\n";
        }
        std::cout << "  Expected name: " << e3.getExpectedName() << "\n";
    } else {
        std::cout << "✗ Failed to detect invalid naming\n";
    }
    
    // Test 4: Invalid role predicate
    std::cout << "\nTest 4: Invalid Role Predicate\n";
    StateOfAffairs invalidRoleSoa;
    
    Eventuality e4;
    e4.name = "soa_elcs";
    e4.type = "soaLeave";
    e4.modality = "rexist";
    e4.agent = "soa_CONTAINER_SHIP";
    e4.roles["soaHas_invalid_role"] = "some_value";  // Invalid role!
    invalidRoleSoa.eventualities[e4.name] = e4;
    
    errors.clear();
    if (!invalidRoleSoa.validateEventualities(errors)) {
        std::cout << "✓ Correctly detected invalid role:\n";
        for (const auto& error : errors) {
            std::cout << "  - " << error << "\n";
        }
    } else {
        std::cout << "✗ Failed to detect invalid role\n";
    }
    
    // Test 5: Missing required agent
    std::cout << "\nTest 5: Missing Required Agent\n";
    StateOfAffairs missingAgentSoa;
    
    Eventuality e5;
    e5.name = "soa_ep";
    e5.type = "soaPay";
    e5.modality = "rexist";
    // e5.agent is missing!
    missingAgentSoa.eventualities[e5.name] = e5;
    
    errors.clear();
    if (!missingAgentSoa.validateEventualities(errors)) {
        std::cout << "✓ Correctly detected missing agent:\n";
        for (const auto& error : errors) {
            std::cout << "  - " << error << "\n";
        }
    } else {
        std::cout << "✗ Failed to detect missing agent\n";
    }
    
    // Test validation functions directly
    std::cout << "\nTest 6: Direct Validation Functions\n";
    
    std::cout << "Valid eventuality types:\n";
    for (const auto& type : KnowledgeIO::getValidEventualityTypes()) {
        std::cout << "  - " << type << "\n";
        if (type == "soaMoor" || type == "soaPay" || type == "soaLeave") {
            std::cout << "    ✓ Smart Port example type\n";
        }
    }
    
    std::cout << "\nValid modalities:\n";
    for (const auto& mod : KnowledgeIO::getValidModalities()) {
        std::cout << "  - " << mod;
        if (mod == "rexist") {
            std::cout << " (required for state of affairs)";
        }
        std::cout << "\n";
    }
    
    std::cout << "\nSample valid roles:\n";
    const auto& roles = KnowledgeIO::getValidRoles();
    int count = 0;
    for (const auto& role : roles) {
        if (count++ < 5) {  // Show first 5 roles
            std::cout << "  - " << role << "\n";
        }
    }
    std::cout << "  ... and " << (roles.size() - 5) << " more\n";
    
    return 0;
}