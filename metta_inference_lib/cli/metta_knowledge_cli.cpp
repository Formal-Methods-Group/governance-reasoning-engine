#include "CLI11.hpp"
#include "metta_inference/knowledge_io.hpp"
#include "metta_inference/inference_engine.hpp"
#include "metta_inference/config.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace mi = metta_inference;
namespace fs = std::filesystem;

namespace Color {
    inline constexpr std::string_view RED = "\033[0;31m";
    inline constexpr std::string_view GREEN = "\033[0;32m";
    inline constexpr std::string_view CYAN = "\033[0;36m";
    inline constexpr std::string_view YELLOW = "\033[0;33m";
    inline constexpr std::string_view NC = "\033[0m";
    inline constexpr std::string_view BOLD = "\033[1m";
}

class MettaKnowledgeCLI {
private:
    void printNormDetails(const mi::Norm& norm) {
        std::cout << Color::BOLD << "Norm: " << Color::NC << norm.name << "\n";
        
        if (!norm.description.empty()) {
            std::cout << "  " << Color::CYAN << "Description: " << Color::NC 
                     << norm.description << "\n";
        }
        
        if (!norm.parameters.empty()) {
            std::cout << "  " << Color::CYAN << "Parameters: " << Color::NC;
            for (const auto& param : norm.parameters) {
                std::cout << param << " ";
            }
            std::cout << "\n";
        }
        
        if (!norm.conditions.empty()) {
            std::cout << "  " << Color::CYAN << "Conditions (" 
                     << norm.conditions.size() << "):" << Color::NC << "\n";
            for (const auto& cond : norm.conditions) {
                std::cout << "    " << cond.toString() << "\n";
            }
        }
        
        if (!norm.consequences.empty()) {
            std::cout << "  " << Color::CYAN << "Consequences (" 
                     << norm.consequences.size() << "):" << Color::NC << "\n";
            for (const auto& cons : norm.consequences) {
                std::cout << "    " << cons.toString() << "\n";
            }
        }
    }
    
    void printStateOfAffairs(const mi::StateOfAffairs& soa) {
        if (!soa.description.empty()) {
            std::cout << Color::CYAN << "Description: " << Color::NC 
                     << soa.description << "\n";
        }
        
        std::cout << Color::CYAN << "Facts (" << soa.facts.size() 
                 << "):" << Color::NC << "\n";
        for (const auto& fact : soa.facts) {
            std::cout << "  " << fact.toString() << "\n";
        }
        
        if (!soa.eventualities.empty()) {
            std::cout << "\n" << Color::CYAN << "Eventualities (" 
                     << soa.eventualities.size() << "):" << Color::NC << "\n";
            for (const auto& [name, eventuality] : soa.eventualities) {
                std::cout << "  " << Color::BOLD << name << Color::NC << ":\n";
                std::cout << "    Type: " << eventuality.type << "\n";
                std::cout << "    Modality: " << eventuality.modality << "\n";
                std::cout << "    Agent: " << eventuality.agent << "\n";
                if (!eventuality.roles.empty()) {
                    std::cout << "    Roles:\n";
                    for (const auto& [role, value] : eventuality.roles) {
                        std::cout << "      " << role << ": " << value << "\n";
                    }
                }
            }
        }
    }

public:
    int run(int argc, char* argv[]) {
        CLI::App app{"MeTTa Knowledge I/O Tool (Norms and State of Affairs)"};
        app.require_subcommand(1);
        
        // Extract command
        auto* extract = app.add_subcommand("extract", 
            "Extract norms and state of affairs from MeTTa file");
        
        std::string extractInput;
        std::string extractOutput;
        bool extractNorms = false;
        bool extractSoa = false;
        bool extractValidate = false;
        
        extract->add_option("input", extractInput, "Input MeTTa file")
               ->required()
               ->check(CLI::ExistingFile);
        extract->add_option("-o,--output", extractOutput, 
               "Output file (if not specified, prints to stdout)");
        extract->add_flag("-n,--norms", extractNorms, 
               "Extract norms");
        extract->add_flag("-s,--soa", extractSoa, 
               "Extract state of affairs");
        extract->add_flag("--validate", extractValidate,
               "Validate state of affairs during extraction");
        
        extract->callback([&]() {
            if (!extractNorms && !extractSoa) {
                extractNorms = extractSoa = true;  // Default: extract both
            }
            
            try {
                auto doc = mi::KnowledgeIO::readMettaDocument(extractInput);
                
                if (!extractOutput.empty()) {
                    // Write to file
                    std::ofstream out(extractOutput);
                    if (!out.is_open()) {
                        throw std::runtime_error("Cannot open output file: " + extractOutput);
                    }
                    
                    if (extractNorms && !doc.norms.empty()) {
                        out << "; ========== NORMS ==========\n\n";
                        for (const auto& norm : doc.norms) {
                            out << norm.toString() << "\n";
                        }
                    }
                    
                    if (extractSoa && !doc.stateOfAffairs.facts.empty()) {
                        if (extractNorms && !doc.norms.empty()) {
                            out << "\n";
                        }
                        out << "; ========== STATE OF AFFAIRS ==========\n\n";
                        out << doc.stateOfAffairs.toString();
                    }
                    
                    // Validate if requested
                    if (extractValidate && !doc.stateOfAffairs.eventualities.empty()) {
                        std::vector<std::string> errors;
                        if (!doc.stateOfAffairs.validateEventualities(errors)) {
                            std::cout << Color::YELLOW << "Warning: State of Affairs validation issues:\n" << Color::NC;
                            for (const auto& error : errors) {
                                std::cout << "  • " << error << "\n";
                            }
                        }
                    }
                    
                    std::cout << Color::GREEN << "✓" << Color::NC 
                             << " Extracted to: " << extractOutput << "\n";
                } else {
                    // Print to stdout
                    if (extractNorms) {
                        std::cout << Color::BOLD << "\n=== NORMS (" 
                                 << doc.norms.size() << ") ===" << Color::NC << "\n\n";
                        for (const auto& norm : doc.norms) {
                            printNormDetails(norm);
                            std::cout << "\n";
                        }
                    }
                    
                    if (extractSoa) {
                        std::cout << Color::BOLD << "\n=== STATE OF AFFAIRS ===" 
                                 << Color::NC << "\n\n";
                        printStateOfAffairs(doc.stateOfAffairs);
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << Color::RED << "Error: " << e.what() 
                         << Color::NC << "\n";
                return;
            }
        });
        
        // Create command
        auto* create = app.add_subcommand("create", 
            "Create new norms or state of affairs file");
        
        std::string createType;
        std::string createOutput;
        
        create->add_option("type", createType, "Type to create")
              ->required()
              ->check(CLI::IsMember({"norm", "soa", "both"}));
        create->add_option("-o,--output", createOutput, "Output file")
              ->required();
        
        create->callback([&]() {
            try {
                mi::Norm exampleNorm;
                mi::StateOfAffairs exampleSoa;
                
                if (createType == "norm" || createType == "both") {
                    // Create example norm
                    exampleNorm.name = "example-norm";
                    exampleNorm.description = "Example norm template";
                    exampleNorm.parameters = {"$agent", "$action", "$resource"};
                    
                    mi::Condition cond;
                    cond.variable = "True";
                    cond.expression = "ct-triple $agent type Agent";
                    exampleNorm.conditions.push_back(cond);
                    
                    mi::Triple cons;
                    cons.subject = "$agent";
                    cons.predicate = "permitted";
                    cons.object = "$action";
                    exampleNorm.consequences.push_back(cons);
                    
                    if (createType == "norm") {
                        mi::KnowledgeIO::writeNormsToFile({exampleNorm}, createOutput);
                    }
                }
                
                if (createType == "soa" || createType == "both") {
                    // Create example state of affairs
                    exampleSoa.description = "Example state of affairs template";
                    
                    mi::Triple fact1;
                    fact1.subject = "agent1";
                    fact1.predicate = "type";
                    fact1.object = "Agent";
                    exampleSoa.facts.push_back(fact1);
                    
                    mi::Triple fact2;
                    fact2.subject = "agent1";
                    fact2.predicate = "hasResource";
                    fact2.object = "resource1";
                    exampleSoa.facts.push_back(fact2);
                    
                    if (createType == "soa") {
                        mi::KnowledgeIO::writeStateOfAffairsToFile(exampleSoa, createOutput);
                    } else if (createType == "both") {
                        mi::KnowledgeIO::MettaDocument doc;
                        doc.header = "; Example MeTTa document with norms and state of affairs";
                        doc.norms.push_back(exampleNorm);
                        doc.stateOfAffairs = exampleSoa;
                        mi::KnowledgeIO::writeMettaDocument(doc, createOutput);
                    }
                }
                
                std::cout << Color::GREEN << "✓" << Color::NC 
                         << " Created template file: " << createOutput << "\n";
                         
            } catch (const std::exception& e) {
                std::cerr << Color::RED << "Error: " << e.what() 
                         << Color::NC << "\n";
            }
        });
        
        // Analyze command
        auto* analyze = app.add_subcommand("analyze", 
            "Analyze MeTTa file structure");
        
        std::string analyzeInput;
        bool verbose = false;
        bool analyzeValidate = false;
        
        analyze->add_option("input", analyzeInput, "Input MeTTa file")
               ->required()
               ->check(CLI::ExistingFile);
        analyze->add_flag("-v,--verbose", verbose, "Verbose output");
        analyze->add_flag("--validate", analyzeValidate, "Include validation in analysis");
        
        analyze->callback([&]() {
            try {
                auto doc = mi::KnowledgeIO::readMettaDocument(analyzeInput);
                
                std::cout << Color::BOLD << "File Analysis: " << Color::NC 
                         << analyzeInput << "\n\n";
                
                // Summary
                std::cout << Color::CYAN << "Summary:" << Color::NC << "\n";
                std::cout << "  • Norms: " << doc.norms.size() << "\n";
                std::cout << "  • State of Affairs Facts: " 
                         << doc.stateOfAffairs.facts.size() << "\n";
                
                // Norm statistics
                if (!doc.norms.empty()) {
                    int totalConditions = 0;
                    int totalConsequences = 0;
                    
                    for (const auto& norm : doc.norms) {
                        totalConditions += norm.conditions.size();
                        totalConsequences += norm.consequences.size();
                    }
                    
                    std::cout << "\n" << Color::CYAN << "Norm Statistics:" 
                             << Color::NC << "\n";
                    std::cout << "  • Average conditions per norm: " 
                             << (float)totalConditions / doc.norms.size() << "\n";
                    std::cout << "  • Average consequences per norm: " 
                             << (float)totalConsequences / doc.norms.size() << "\n";
                }
                
                // Fact statistics
                if (!doc.stateOfAffairs.facts.empty()) {
                    std::map<std::string, int> predicateCount;
                    std::set<std::string> subjects;
                    
                    for (const auto& fact : doc.stateOfAffairs.facts) {
                        predicateCount[fact.predicate]++;
                        subjects.insert(fact.subject);
                    }
                    
                    std::cout << "\n" << Color::CYAN 
                             << "State of Affairs Statistics:" << Color::NC << "\n";
                    std::cout << "  • Unique subjects: " << subjects.size() << "\n";
                    std::cout << "  • Unique predicates: " << predicateCount.size() << "\n";
                    
                    if (verbose) {
                        std::cout << "\n" << Color::CYAN << "Predicate frequency:" 
                                 << Color::NC << "\n";
                        for (const auto& [pred, count] : predicateCount) {
                            std::cout << "    " << pred << ": " << count << "\n";
                        }
                    }
                }
                
                if (verbose) {
                    std::cout << "\n" << Color::YELLOW << "Detailed listing:" 
                             << Color::NC << "\n\n";
                    
                    if (!doc.norms.empty()) {
                        std::cout << Color::BOLD << "NORMS:" << Color::NC << "\n";
                        for (const auto& norm : doc.norms) {
                            std::cout << "  • " << norm.name;
                            if (!norm.parameters.empty()) {
                                std::cout << "(";
                                for (size_t i = 0; i < norm.parameters.size(); ++i) {
                                    if (i > 0) std::cout << ", ";
                                    std::cout << norm.parameters[i];
                                }
                                std::cout << ")";
                            }
                            std::cout << "\n";
                        }
                    }
                    
                    if (!doc.stateOfAffairs.facts.empty()) {
                        std::cout << "\n" << Color::BOLD << "STATE OF AFFAIRS FACTS:" 
                                 << Color::NC << "\n";
                        for (const auto& fact : doc.stateOfAffairs.facts) {
                            std::cout << "  • " << fact.subject << " " 
                                     << fact.predicate << " " << fact.object << "\n";
                        }
                    }
                }
                
                // Validate if requested
                if (analyzeValidate) {
                    std::cout << "\n" << Color::CYAN << "Validation Results:" << Color::NC << "\n";
                    
                    if (!doc.stateOfAffairs.eventualities.empty()) {
                        std::vector<std::string> errors;
                        bool valid = doc.stateOfAffairs.validateEventualities(errors);
                        
                        if (valid) {
                            std::cout << "  " << Color::GREEN << "✓ State of Affairs is valid" 
                                     << Color::NC << "\n";
                        } else {
                            std::cout << "  " << Color::RED << "✗ State of Affairs has validation issues:" 
                                     << Color::NC << "\n";
                            for (const auto& error : errors) {
                                std::cout << "    • " << error << "\n";
                            }
                        }
                    } else {
                        std::cout << "  No eventualities to validate\n";
                    }
                }
                
            } catch (const std::exception& e) {
                std::cerr << Color::RED << "Error: " << e.what() 
                         << Color::NC << "\n";
            }
        });
        
        // Convert command  
        auto* convert = app.add_subcommand("convert",
            "Convert between different formats");
        
        std::string convertInput;
        std::string convertOutput;
        std::string convertFormat;
        
        convert->add_option("input", convertInput, "Input file")
               ->required()
               ->check(CLI::ExistingFile);
        convert->add_option("-o,--output", convertOutput, "Output file")
               ->required();
        convert->add_option("-f,--format", convertFormat, "Output format")
               ->default_val("metta")
               ->check(CLI::IsMember({"metta", "json", "yaml"}));
        
        convert->callback([&]() {
            try {
                auto doc = mi::KnowledgeIO::readMettaDocument(convertInput);
                
                if (convertFormat == "metta") {
                    mi::KnowledgeIO::writeMettaDocument(doc, convertOutput);
                } else {
                    std::cerr << Color::YELLOW << "Warning: " << convertFormat 
                             << " format not yet implemented. Using MeTTa format." 
                             << Color::NC << "\n";
                    mi::KnowledgeIO::writeMettaDocument(doc, convertOutput);
                }
                
                std::cout << Color::GREEN << "✓" << Color::NC 
                         << " Converted to: " << convertOutput << "\n";
                         
            } catch (const std::exception& e) {
                std::cerr << Color::RED << "Error: " << e.what() 
                         << Color::NC << "\n";
            }
        });
        
        // Validate command
        auto* validate = app.add_subcommand("validate",
            "Validate state of affairs against knowledge representation rules");
        
        std::string validateInput;
        bool validateVerbose = false;
        bool validateStrict = false;
        
        validate->add_option("input", validateInput, "Input MeTTa file")
                ->required()
                ->check(CLI::ExistingFile);
        validate->add_flag("-v,--verbose", validateVerbose, "Show detailed validation results");
        validate->add_flag("-s,--strict", validateStrict, "Exit with error code on validation failure");
        
        validate->callback([&]() {
            try {
                auto doc = mi::KnowledgeIO::readMettaDocument(validateInput);
                
                std::cout << Color::BOLD << "Validating: " << Color::NC 
                         << validateInput << "\n\n";
                
                // Validate state of affairs eventualities
                std::vector<std::string> errors;
                bool valid = doc.stateOfAffairs.validateEventualities(errors);
                
                if (valid) {
                    std::cout << Color::GREEN << "✓ State of Affairs validation passed" 
                             << Color::NC << "\n";
                    
                    if (validateVerbose && !doc.stateOfAffairs.eventualities.empty()) {
                        std::cout << "\n" << Color::CYAN << "Valid eventualities found:" 
                                 << Color::NC << "\n";
                        for (const auto& [name, eventuality] : doc.stateOfAffairs.eventualities) {
                            std::cout << "  • " << name << " (" << eventuality.type 
                                     << ", agent: " << eventuality.agent << ")\n";
                        }
                    }
                } else {
                    std::cout << Color::RED << "✗ State of Affairs validation failed" 
                             << Color::NC << "\n\n";
                    std::cout << Color::YELLOW << "Validation errors:" << Color::NC << "\n";
                    for (const auto& error : errors) {
                        std::cout << "  • " << error << "\n";
                    }
                    
                    if (validateStrict) {
                        std::exit(1);
                    }
                }
                
                // Show statistics
                std::cout << "\n" << Color::CYAN << "Statistics:" << Color::NC << "\n";
                std::cout << "  • Total facts: " << doc.stateOfAffairs.facts.size() << "\n";
                std::cout << "  • Eventualities: " << doc.stateOfAffairs.eventualities.size() << "\n";
                std::cout << "  • Norms: " << doc.norms.size() << "\n";
                
                // Check for unused facts (facts about entities not declared as eventualities)
                if (validateVerbose) {
                    std::set<std::string> eventualityNames;
                    for (const auto& [name, _] : doc.stateOfAffairs.eventualities) {
                        eventualityNames.insert(name);
                    }
                    
                    std::vector<std::string> unusedFacts;
                    for (const auto& fact : doc.stateOfAffairs.facts) {
                        if (fact.subject.substr(0, 5) == "soa_e" && 
                            eventualityNames.find(fact.subject) == eventualityNames.end()) {
                            unusedFacts.push_back(fact.toString());
                        }
                    }
                    
                    if (!unusedFacts.empty()) {
                        std::cout << "\n" << Color::YELLOW << "Warning: Facts about undeclared eventualities:" 
                                 << Color::NC << "\n";
                        for (const auto& fact : unusedFacts) {
                            std::cout << "  • " << fact << "\n";
                        }
                    }
                }
                
                // Validate against known types
                if (validateVerbose) {
                    std::cout << "\n" << Color::CYAN << "Knowledge base validation:" << Color::NC << "\n";
                    
                    // Check eventuality types
                    bool allTypesValid = true;
                    for (const auto& [name, eventuality] : doc.stateOfAffairs.eventualities) {
                        if (!mi::KnowledgeIO::isValidEventualityType(eventuality.type)) {
                            std::cout << "  • " << Color::RED << "Invalid type: " << Color::NC 
                                     << eventuality.type << " in " << name << "\n";
                            allTypesValid = false;
                        }
                    }
                    
                    if (allTypesValid) {
                        std::cout << "  • " << Color::GREEN << "All eventuality types are valid" 
                                 << Color::NC << "\n";
                    }
                    
                    // Check roles
                    bool allRolesValid = true;
                    for (const auto& [name, eventuality] : doc.stateOfAffairs.eventualities) {
                        for (const auto& [role, _] : eventuality.roles) {
                            if (!mi::KnowledgeIO::isValidRole(role)) {
                                std::cout << "  • " << Color::RED << "Invalid role: " << Color::NC 
                                         << role << " in " << name << "\n";
                                allRolesValid = false;
                            }
                        }
                    }
                    
                    if (allRolesValid) {
                        std::cout << "  • " << Color::GREEN << "All roles are valid" 
                                 << Color::NC << "\n";
                    }
                }
                
            } catch (const std::exception& e) {
                std::cerr << Color::RED << "Error: " << e.what() 
                         << Color::NC << "\n";
                if (validateStrict) {
                    std::exit(1);
                }
            }
        });
        
        // Setup version and help
        app.set_version_flag("--version", "1.0.0");
        app.description("Tool for reading, writing, and manipulating MeTTa knowledge (norms and state of affairs).\n\n"
                       "This tool provides utilities for:\n"
                       "  • Extracting norms and state of affairs from MeTTa files\n"
                       "  • Creating template files for new norms and facts\n"
                       "  • Analyzing MeTTa file structure and statistics\n"
                       "  • Validating state of affairs against knowledge representation rules\n"
                       "  • Converting between different formats");
        
        app.footer("EXAMPLES:\n"
                  "  metta_knowledge_cli extract input.metta -n          # Extract only norms\n"
                  "  metta_knowledge_cli extract input.metta -s -o soa.metta # Extract state of affairs to file\n"
                  "  metta_knowledge_cli create norm -o template.metta   # Create norm template\n"
                  "  metta_knowledge_cli analyze input.metta -v          # Analyze with verbose output\n"
                  "  metta_knowledge_cli validate input.metta -v         # Validate state of affairs\n"
                  "  metta_knowledge_cli convert input.metta -o output.metta # Convert/clean MeTTa file");
        
        CLI11_PARSE(app, argc, argv);
        return 0;
    }
};

int main(int argc, char* argv[]) {
    try {
        MettaKnowledgeCLI cli;
        return cli.run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << Color::RED << "Fatal error: " << e.what() << Color::NC << "\n";
        return 1;
    } catch (...) {
        std::cerr << Color::RED << "Fatal error: Unknown exception" << Color::NC << "\n";
        return 1;
    }
}