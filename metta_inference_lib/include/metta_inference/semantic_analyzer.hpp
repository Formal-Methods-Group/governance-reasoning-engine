#ifndef METTA_INFERENCE_SEMANTIC_ANALYZER_HPP
#define METTA_INFERENCE_SEMANTIC_ANALYZER_HPP

#include "sexpr_parser.hpp"
#include "entity_resolver.hpp"
#include "config.hpp"
#include <vector>
#include <memory>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <functional>

namespace metta_inference {

// Semantic structures representing logical relationships
struct StateOfAffairs {
    std::string entity;
    std::string action;
    std::string agent;
    std::string instrument;
    bool exists = true;
    std::map<std::string, std::string> properties;
    
    std::string toString() const;
};

struct LogicalContradiction {
    StateOfAffairs positive;
    StateOfAffairs negative;
    std::string type;  // "existence", "property", "action"
    
    std::string getDescription(const EntityResolver& resolver,
                              const DescriptionTemplates& templates) const;
};

struct RegulatoryConflict {
    std::string regulation1;
    std::string regulation2;
    std::string conflictingRequirement;
    std::string affectedEntity;
    
    std::string getDescription(const EntityResolver& resolver,
                              const DescriptionTemplates& templates) const;
};

struct NecessaryViolation {
    std::string violatedRule;
    std::string violator;
    std::string reason;
    
    std::string getDescription(const EntityResolver& resolver,
                              const DescriptionTemplates& templates) const;
};

struct ComplianceRelation {
    std::string entity;
    std::string obligation;
    std::string fulfilledBy;
    
    std::string getDescription(const EntityResolver& resolver,
                              const DescriptionTemplates& templates) const;
};

// Main semantic analyzer
class SemanticAnalyzer {
public:
    struct AnalysisResult {
        std::vector<StateOfAffairs> inferredFacts;
        std::vector<LogicalContradiction> contradictions;
        std::vector<RegulatoryConflict> conflicts;
        std::vector<NecessaryViolation> violations;
        std::vector<ComplianceRelation> compliances;
        
        Metrics toMetrics() const;
    };
    
    SemanticAnalyzer();
    explicit SemanticAnalyzer(EntityResolver* resolver, DescriptionTemplates* templates);
    
    // Main analysis method
    AnalysisResult analyze(const std::string& mettaOutput);
    
    // Individual analysis methods
    std::vector<StateOfAffairs> extractStateOfAffairs(
        const std::vector<std::shared_ptr<SExpr>>& expressions);
    
    std::vector<LogicalContradiction> findContradictions(
        const std::vector<std::shared_ptr<SExpr>>& expressions);
    
    std::vector<RegulatoryConflict> findConflicts(
        const std::vector<std::shared_ptr<SExpr>>& expressions);
    
    std::vector<NecessaryViolation> findViolations(
        const std::vector<std::shared_ptr<SExpr>>& expressions);
    
    std::vector<ComplianceRelation> findCompliances(
        const std::vector<std::shared_ptr<SExpr>>& expressions);
    
private:
    EntityResolver* entityResolver;
    DescriptionTemplates* descriptionTemplates;
    
    // Helper methods for parsing specific patterns
    std::optional<StateOfAffairs> parseTripleToSOA(const std::shared_ptr<SExpr>& triple);
    std::optional<LogicalContradiction> parseMetaContradiction(const std::shared_ptr<SExpr>& expr);
    std::optional<RegulatoryConflict> parseConflictExpr(const std::shared_ptr<SExpr>& expr);
    std::optional<NecessaryViolation> parseViolationExpr(const std::shared_ptr<SExpr>& expr);
    std::optional<ComplianceRelation> parseComplianceExpr(const std::shared_ptr<SExpr>& expr);
    
    // Entity relationship analysis
    bool areEntitiesContradictory(const std::string& entity1, const std::string& entity2);
    std::pair<std::string, std::string> extractContradictoryPair(
        const std::string& entity1, const std::string& entity2);
    
    // Action extraction helpers
    std::string extractBaseAction(const std::string& soaEntity);
    
    // Context extraction for better descriptions
    std::map<std::string, std::string> extractContext(const std::shared_ptr<SExpr>& expr);
};

// Knowledge base for semantic understanding
class SemanticKnowledge {
public:
    struct Rule {
        std::string id;
        std::string type;  // "obligation", "prohibition", "permission"
        std::string subject;
        std::string action;
        std::map<std::string, std::string> conditions;
    };
    
    struct Entity {
        std::string id;
        std::string type;
        std::map<std::string, std::string> attributes;
    };
    
    void addRule(const Rule& rule);
    void addEntity(const Entity& entity);
    
    std::optional<Rule> findRule(const std::string& id) const;
    std::optional<Entity> findEntity(const std::string& id) const;
    
    std::vector<Rule> findRulesForEntity(const std::string& entityId) const;
    std::vector<Rule> findConflictingRules(const Rule& rule) const;
    
private:
    std::unordered_map<std::string, Rule> rules;
    std::unordered_map<std::string, Entity> entities;
};

// Inference pattern detector
class InferencePatternDetector {
public:
    enum class PatternType {
        StateOfAffairsAssertion,
        ContradictionDetection,
        ConflictIdentification,
        ViolationNecessity,
        ComplianceFulfillment,
        Unknown
    };
    
    struct Pattern {
        PatternType type;
        std::vector<std::string> matchSequence;
        std::function<bool(const std::shared_ptr<SExpr>&)> validator;
    };
    
    InferencePatternDetector();
    
    PatternType detectPattern(const std::shared_ptr<SExpr>& expr) const;
    std::vector<std::shared_ptr<SExpr>> findPatternsOfType(
        const std::vector<std::shared_ptr<SExpr>>& expressions,
        PatternType type) const;
    
private:
    std::vector<Pattern> patterns;
    
    void initializePatterns();
};

}

#endif