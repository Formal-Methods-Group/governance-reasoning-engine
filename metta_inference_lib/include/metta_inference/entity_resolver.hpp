#ifndef METTA_INFERENCE_ENTITY_RESOLVER_HPP
#define METTA_INFERENCE_ENTITY_RESOLVER_HPP

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <optional>

namespace metta_inference {

// Configuration-driven entity resolver
class EntityResolver {
public:
    struct NameMapping {
        std::string pattern;
        std::string replacement;
        bool useRegex = false;
    };
    
    struct ActionMapping {
        std::string pattern;
        std::string baseForm;
        std::string presentTense;
        std::string pastTense;
    };
    
    EntityResolver();
    
    // Configure from JSON or programmatically
    void loadConfiguration(const std::string& jsonPath);
    void addEntityMapping(const std::string& entity, const std::string& displayName);
    void addSpecialCharMapping(const std::string& from, const std::string& to);
    void addActionMapping(const std::string& soaAction, const ActionMapping& mapping);
    
    // Resolution methods
    std::string resolveEntity(const std::string& entity) const;
    std::string resolveAction(const std::string& action, const std::string& tense = "present") const;
    std::string resolveInstrument(const std::string& instrument) const;
    std::string resolvePort(const std::string& port) const;
    
    // Convert soa_ENTITY_NAME to human-readable form
    std::string entityToHumanReadable(const std::string& soaEntity) const;
    
    // Check if entity is negated (e.g., soa_enXXX)
    bool isNegatedEntity(const std::string& entity) const;
    std::string getNegatedForm(const std::string& entity) const;
    std::string getBaseForm(const std::string& entity) const;
    
private:
    std::unordered_map<std::string, std::string> entityMappings;
    std::unordered_map<std::string, std::string> specialCharMappings;
    std::unordered_map<std::string, ActionMapping> actionMappings;
    std::unordered_map<std::string, std::string> instrumentMappings;
    std::unordered_map<std::string, std::string> portMappings;
    
    // Helper for applying special character replacements
    std::string applySpecialChars(const std::string& text) const;
    
    // Default mappings
    void loadDefaultMappings();
};

// Template-driven description generator
class DescriptionTemplates {
public:
    struct Template {
        std::string id;
        std::string pattern;
        std::map<std::string, std::string> variables;
    };
    
    DescriptionTemplates();
    
    // Load templates from configuration
    void loadTemplates(const std::string& jsonPath);
    void addTemplate(const std::string& id, const std::string& pattern);
    
    // Generate descriptions from templates
    std::string generateContradictionDescription(
        const std::string& entity1,
        const std::string& entity2,
        const std::string& context = "") const;
    
    std::string generateConflictDescription(
        const std::string& entity1,
        const std::string& entity2,
        const std::string& reason = "") const;
    
    std::string generateViolationDescription(
        const std::string& violator,
        const std::string& violatedRule,
        const std::string& context = "") const;
    
    std::string generateComplianceDescription(
        const std::string& entity,
        const std::string& rule,
        const std::string& action = "") const;
    
    // Template variable substitution
    std::string substitute(const std::string& templateStr,
                          const std::map<std::string, std::string>& variables) const;
    
private:
    std::map<std::string, Template> templates;
    
    void loadDefaultTemplates();
    std::string findBestTemplate(const std::string& category,
                                const std::map<std::string, std::string>& context) const;
};

// Inference configuration manager
class InferenceConfiguration {
public:
    struct Config {
        // Entity resolution settings
        std::map<std::string, std::string> entityMappings;
        std::map<std::string, std::string> specialCharacters;
        
        // Action mappings
        std::map<std::string, EntityResolver::ActionMapping> actionMappings;
        
        // Description templates
        std::map<std::string, std::string> contradictionTemplates;
        std::map<std::string, std::string> conflictTemplates;
        std::map<std::string, std::string> violationTemplates;
        std::map<std::string, std::string> complianceTemplates;
        
        // Pattern recognition settings
        bool useStrictMatching = false;
        bool enableFuzzyMatching = false;
        double fuzzyThreshold = 0.8;
    };
    
    static InferenceConfiguration& getInstance();
    
    void loadFromFile(const std::string& path);
    void loadFromString(const std::string& json);
    
    const Config& getConfig() const { return config; }
    EntityResolver& getEntityResolver() { return entityResolver; }
    DescriptionTemplates& getTemplates() { return descriptionTemplates; }
    
private:
    InferenceConfiguration();
    
    Config config;
    EntityResolver entityResolver;
    DescriptionTemplates descriptionTemplates;
    
    void applyConfiguration();
};

}

#endif