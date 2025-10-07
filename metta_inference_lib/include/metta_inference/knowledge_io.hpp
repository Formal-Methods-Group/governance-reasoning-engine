#ifndef METTA_INFERENCE_KNOWLEDGE_IO_HPP
#define METTA_INFERENCE_KNOWLEDGE_IO_HPP

#include <filesystem>
#include <vector>
#include <string>
#include <variant>
#include <optional>
#include <set>
#include <map>
#include <memory>

namespace metta_inference {

namespace fs = std::filesystem;

// Forward declarations
class SExpr;

// Structure to represent a triple (subject, predicate, object)
struct Triple {
    std::string subject;
    std::string predicate; 
    std::string object;
    
    // Optional type specification (e.g., "ct-triple" or "meta-triple")
    std::string tripleType = "ct-triple";
    
    // Flag to indicate if object is a nested expression
    bool objectIsExpression = false;
    
    std::string toString() const;
};

// Structure to represent a condition in a norm
struct Condition {
    std::string variable;
    std::string expression;
    
    std::string toString() const;
};

// Structure to represent a norm rule
struct Norm {
    std::string name;                      // Name of the norm (e.g., "pay-obligatory")
    std::vector<std::string> parameters;   // Parameters (e.g., ["$v", "$spt"])
    std::vector<Condition> conditions;     // Conditions in let* block
    std::vector<Triple> consequences;      // Resulting triples
    std::string description;                // Optional human-readable description
    
    std::string toString() const;
};

// Structure to represent an eventuality in state of affairs
struct Eventuality {
    std::string name;                      // e.g., "soa_emam"
    std::string type;                      // e.g., "soaMoor"
    std::string modality;                  // e.g., "rexist" (required for state of affairs)
    std::string agent;                      // e.g., "soa_ALEXANDRA_MAERSK" (required)
    std::map<std::string, std::string> roles; // Optional role assignments
    
    bool isValid() const;
    std::string getExpectedName() const;      // Generate expected name based on type and agent
};

// Structure to represent logical expressions (AND, OR, NOT)
struct LogicalExpression {
    enum Type { AND, OR, NOT, EQUAL };
    Type type;
    std::string name;                       // e.g., "soa_eo" for (ct-or soa_eo)
    std::vector<std::string> operands;      // e.g., ["soa_elam", "soa_ea"] 
    
    std::string toString() const;
};

// Structure to represent an entity
struct Entity {
    std::string name;                       // e.g., "ALEXANDRA_MAERSK"
    std::string type;                       // e.g., "soaContainerVessel"
    std::map<std::string, std::string> properties; // Additional properties
    
    std::string toString() const;
};

// Structure to represent a negation
struct Negation {
    std::string name;                       // e.g., "soa_enmam"
    std::string negatedEntity;              // e.g., "soa_emam"
    
    std::string toString() const;
};

// Structure to represent state of affairs
struct StateOfAffairs {
    std::vector<Triple> facts;             // Collection of fact triples
    std::map<std::string, Eventuality> eventualities; // Parsed eventualities
    std::map<std::string, Entity> entities; // Entity definitions
    std::vector<LogicalExpression> logicalExpressions; // Logical constraints
    std::vector<Negation> negations;       // Negation expressions
    std::string description;                // Optional description
    
    std::string toString() const;
    bool validateEventualities(std::vector<std::string>& errors) const;
    bool validateEntities(std::vector<std::string>& errors) const;
};

// Main I/O class for knowledge (norms and state of affairs)
class KnowledgeIO {
public:
    // Reading functions
    static std::vector<Norm> readNormsFromFile(const fs::path& filepath);
    static StateOfAffairs readStateOfAffairsFromFile(const fs::path& filepath);
    
    // Writing functions  
    static void writeNormsToFile(const std::vector<Norm>& norms, const fs::path& filepath);
    static void writeStateOfAffairsToFile(const StateOfAffairs& soa, const fs::path& filepath);
    
    // Combined reading (reads both norms and state of affairs from one file)
    struct MettaDocument {
        std::vector<Norm> norms;
        StateOfAffairs stateOfAffairs;
        std::string header;  // Optional header comments
    };
    static MettaDocument readMettaDocument(const fs::path& filepath);
    static void writeMettaDocument(const MettaDocument& doc, const fs::path& filepath);
    
    // Parsing utilities
    static std::optional<Norm> parseNorm(const std::string& mettaCode);
    static std::optional<Triple> parseTriple(const std::string& mettaCode);
    static std::optional<Triple> parseTripleFromExpr(const std::shared_ptr<SExpr>& expr);
    static std::optional<Negation> parseNegation(const std::shared_ptr<SExpr>& expr);
    static std::optional<LogicalExpression> parseLogicalExpression(const std::shared_ptr<SExpr>& expr);
    static std::optional<Entity> parseEntity(const Triple& triple);
    
    // Extraction utilities (extract norms/soa from larger metta files)
    static std::vector<Norm> extractNormsFromMetta(const std::string& mettaContent);
    static StateOfAffairs extractStateOfAffairsFromMetta(const std::string& mettaContent);
    
    // Validation utilities
    static bool validateEventuality(const Eventuality& eventuality, std::string& error);
    static bool validatePredicate(const std::string& predicate, std::string& error);
    static bool isValidEventualityType(const std::string& type);
    static bool isValidRole(const std::string& role);
    static bool isValidModality(const std::string& modality);
    
    // Known types from knowledge base
    static const std::set<std::string>& getValidEventualityTypes();
    static const std::set<std::string>& getValidRoles();
    static const std::set<std::string>& getValidModalities();
    
private:
    // Helper functions for parsing
    static std::string trimWhitespace(const std::string& str);
    static std::vector<std::string> splitParameters(const std::string& paramStr);
    static bool isCommentLine(const std::string& line);
    static bool isNormDefinition(const std::string& line);
    static bool isTripleDefinition(const std::string& line);
    static bool isNegationDefinition(const std::string& line);
    static bool isLogicalDefinition(const std::string& line);
};

// Abstract visitor for processing different expression types
class ExpressionVisitor {
public:
    virtual ~ExpressionVisitor() = default;
    virtual void visitTriple(const Triple& triple) = 0;
    virtual void visitNegation(const Negation& negation) = 0;
    virtual void visitLogicalExpression(const LogicalExpression& expr) = 0;
    virtual void visitEntity(const Entity& entity) = 0;
};

// Expression processor using visitor pattern
class ExpressionProcessor {
public:
    static void process(const std::shared_ptr<SExpr>& expr, ExpressionVisitor& visitor);
    static std::string extractExpressionType(const std::shared_ptr<SExpr>& expr);
};

} // namespace metta_inference

#endif // METTA_INFERENCE_KNOWLEDGE_IO_HPP