#include "metta_inference/entity_resolver.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

namespace metta_inference {

// EntityResolver implementation
EntityResolver::EntityResolver() {
    loadDefaultMappings();
}

void EntityResolver::loadDefaultMappings() {
    // Special character mappings
    specialCharMappings["MAERSK"] = "MÆRSK";
    specialCharMappings["AERSK"] = "ÆRSK";
    
    // Common entity mappings
    entityMappings["soa_ALEXANDRA_MAERSK"] = "ALEXANDRA MÆRSK";
    entityMappings["soa_LAURA_MAERSK"] = "LAURA MÆRSK";
    entityMappings["soa_MICT"] = "MICT Smart Port";
    entityMappings["soa_sptMICT"] = "MICT Smart Port Treasury";
    
    // SOA compound entity mappings (entity + action)
    entityMappings["soa_emam"] = "ALEXANDRA MÆRSK";
    entityMappings["soa_enmam"] = "ALEXANDRA MÆRSK";
    entityMappings["soa_eplm"] = "ALEXANDRA MÆRSK";
    entityMappings["soa_enplm"] = "ALEXANDRA MÆRSK";
    entityMappings["soa_elam"] = "ALEXANDRA MÆRSK";
    entityMappings["soa_enlam"] = "ALEXANDRA MÆRSK";
    entityMappings["soa_epam"] = "ALEXANDRA MÆRSK";
    entityMappings["soa_enpam"] = "ALEXANDRA MÆRSK";
    
    // Instrument mappings
    instrumentMappings["soa_USDS"] = "USDS";
    instrumentMappings["soa_INRS"] = "INRS";
    instrumentMappings["soa_USD"] = "USD";
    instrumentMappings["soa_EUR"] = "EUR";
    
    // Action mappings
    actionMappings["soaMoor"] = {"moor", "moor", "moors", "moored"};
    actionMappings["soaPay"] = {"pay", "pay", "pays", "paid"};
    actionMappings["soaLeave"] = {"leave", "leave", "leaves", "left"};
    actionMappings["soaArrive"] = {"arrive", "arrive", "arrives", "arrived"};
    actionMappings["soaDock"] = {"dock", "dock", "docks", "docked"};
    actionMappings["soaDeliver"] = {"deliver", "deliver", "delivers", "delivered"};
    actionMappings["soaLoad"] = {"load", "load", "loads", "loaded"};
    actionMappings["soaUnload"] = {"unload", "unload", "unloads", "unloaded"};
    
    // SOA compound action mappings (based on suffix patterns)
    actionMappings["soa_emam"] = {"moor", "moor", "moors", "moored"};
    actionMappings["soa_enmam"] = {"moor", "moor", "moors", "moored"};
    actionMappings["soa_eplm"] = {"pay", "pay", "pays", "paid"};
    actionMappings["soa_enplm"] = {"pay", "pay", "pays", "paid"};
    actionMappings["soa_elam"] = {"leave", "leave", "leaves", "left"};
    actionMappings["soa_enlam"] = {"leave", "leave", "leaves", "left"};
    actionMappings["soa_epam"] = {"pay", "pay", "pays", "paid"};
    actionMappings["soa_enpam"] = {"pay", "pay", "pays", "paid"};
}

void EntityResolver::loadConfiguration(const std::string& jsonPath) {
    // In a full implementation, this would parse JSON
    // For now, we'll keep the default mappings
    std::cerr << "Loading configuration from: " << jsonPath << std::endl;
}

void EntityResolver::addEntityMapping(const std::string& entity, const std::string& displayName) {
    entityMappings[entity] = displayName;
}

void EntityResolver::addSpecialCharMapping(const std::string& from, const std::string& to) {
    specialCharMappings[from] = to;
}

void EntityResolver::addActionMapping(const std::string& soaAction, const ActionMapping& mapping) {
    actionMappings[soaAction] = mapping;
}

std::string EntityResolver::resolveEntity(const std::string& entity) const {
    // Check if we have a direct mapping
    auto it = entityMappings.find(entity);
    if (it != entityMappings.end()) {
        return it->second;
    }
    
    // Try to resolve dynamically
    return entityToHumanReadable(entity);
}

std::string EntityResolver::resolveAction(const std::string& action, const std::string& tense) const {
    auto it = actionMappings.find(action);
    if (it != actionMappings.end()) {
        const auto& mapping = it->second;
        if (tense == "present") return mapping.presentTense;
        if (tense == "past") return mapping.pastTense;
        return mapping.baseForm;
    }
    
    // Fallback: try to extract from soa prefix
    if (action.substr(0, 3) == "soa") {
        std::string baseAction = action.substr(3);
        if (!baseAction.empty()) {
            baseAction[0] = std::tolower(baseAction[0]);
            if (tense == "present") {
                return baseAction + "s";
            }
            return baseAction;
        }
    }
    
    return action;
}

std::string EntityResolver::resolveInstrument(const std::string& instrument) const {
    auto it = instrumentMappings.find(instrument);
    if (it != instrumentMappings.end()) {
        return it->second;
    }
    
    // Try to extract from soa_ prefix
    if (instrument.substr(0, 4) == "soa_") {
        return instrument.substr(4);
    }
    
    return instrument;
}

std::string EntityResolver::resolvePort(const std::string& port) const {
    auto it = entityMappings.find(port);
    if (it != entityMappings.end()) {
        return it->second;
    }
    
    if (port.substr(0, 4) == "soa_") {
        std::string portName = port.substr(4);
        if (portName.substr(0, 3) == "spt") {
            portName = portName.substr(3) + " Smart Port";
        }
        return portName;
    }
    
    return port;
}

std::string EntityResolver::entityToHumanReadable(const std::string& soaEntity) const {
    if (soaEntity.substr(0, 4) != "soa_") {
        return soaEntity;
    }
    
    std::string entity = soaEntity.substr(4);
    
    // Replace underscores with spaces
    std::replace(entity.begin(), entity.end(), '_', ' ');
    
    // Apply special character mappings
    entity = applySpecialChars(entity);
    
    return entity;
}

bool EntityResolver::isNegatedEntity(const std::string& entity) const {
    // Check for patterns like soa_enXXX (negated after soa_e)
    return entity.size() > 6 && entity.substr(0, 6) == "soa_en";
}

std::string EntityResolver::getNegatedForm(const std::string& entity) const {
    if (isNegatedEntity(entity)) {
        return entity;
    }
    
    if (entity.substr(0, 5) == "soa_e") {
        return "soa_en" + entity.substr(5);
    }
    
    return "not_" + entity;
}

std::string EntityResolver::getBaseForm(const std::string& entity) const {
    if (!isNegatedEntity(entity)) {
        return entity;
    }
    
    return "soa_e" + entity.substr(6);
}

std::string EntityResolver::applySpecialChars(const std::string& text) const {
    std::string result = text;
    
    for (const auto& [from, to] : specialCharMappings) {
        size_t pos = 0;
        while ((pos = result.find(from, pos)) != std::string::npos) {
            result.replace(pos, from.length(), to);
            pos += to.length();
        }
    }
    
    return result;
}

// DescriptionTemplates implementation
DescriptionTemplates::DescriptionTemplates() {
    loadDefaultTemplates();
}

void DescriptionTemplates::loadDefaultTemplates() {
    // Contradiction templates
    templates["contradiction_existence"] = {
        "contradiction_existence",
        "Contradiction: {entity} cannot both {action1} and {action2}",
        {}
    };
    
    templates["contradiction_payment"] = {
        "contradiction_payment",
        "Contradiction: Payment declared in {instrument1} but {instrument2} is required",
        {}
    };
    
    templates["contradiction_action"] = {
        "contradiction_action",
        "Contradiction: {entity} cannot both {action} and not {action} at the same time",
        {}
    };
    
    // Conflict templates
    templates["conflict_regulation"] = {
        "conflict_regulation",
        "Regulatory conflict: {regulation1} prohibits {action} while {regulation2} requires it",
        {}
    };
    
    templates["conflict_payment"] = {
        "conflict_payment",
        "{entity} faces a conflict: {reason}",
        {}
    };
    
    // Violation templates
    templates["violation_necessary"] = {
        "violation_necessary",
        "The {rule} must be violated due to {reason}",
        {}
    };
    
    templates["violation_constraint"] = {
        "violation_constraint",
        "{entity} violates {rule} because of {constraint}",
        {}
    };
    
    // Compliance templates
    templates["compliance_fulfilled"] = {
        "compliance_fulfilled",
        "{entity} successfully fulfills {obligation} by {action}",
        {}
    };
    
    templates["compliance_met"] = {
        "compliance_met",
        "Requirement {requirement} is met by {entity}",
        {}
    };
}

void DescriptionTemplates::loadTemplates(const std::string& jsonPath) {
    // In a full implementation, this would parse JSON
    std::cerr << "Loading templates from: " << jsonPath << std::endl;
}

void DescriptionTemplates::addTemplate(const std::string& id, const std::string& pattern) {
    templates[id] = {id, pattern, {}};
}

std::string DescriptionTemplates::generateContradictionDescription(
    const std::string& entity1,
    const std::string& entity2,
    const std::string& context) const {
    
    std::map<std::string, std::string> vars;
    vars["entity1"] = entity1;
    vars["entity2"] = entity2;
    vars["context"] = context;
    
    // Determine which template to use based on context
    std::string templateId = "contradiction_existence";
    
    if (context == "payment_method" || 
        context.find("USDS") != std::string::npos ||
        context.find("INRS") != std::string::npos) {
        templateId = "contradiction_payment";
        
        // Extract instruments from context if possible
        if (context.find("USDS") != std::string::npos) {
            vars["instrument1"] = "USDS";
            vars["instrument2"] = "INRS";
        } else if (context.find("INRS") != std::string::npos) {
            vars["instrument1"] = "INRS";
            vars["instrument2"] = "USDS";
        }
    } else if (context == "action") {
        templateId = "contradiction_action";
        
        // Extract entity and action from entity1 and entity2
        // entity1 should be like "ALEXANDRA MÆRSK moor"
        // entity2 should be like "ALEXANDRA MÆRSK does not moor"
        size_t spacePos = entity1.rfind(' ');
        if (spacePos != std::string::npos) {
            vars["entity"] = entity1.substr(0, spacePos);
            vars["action"] = entity1.substr(spacePos + 1);
        } else {
            vars["entity"] = entity1;
            vars["action"] = "act";
        }
    } else {
        // For other cases, extract action information for action1/action2 placeholders
        vars["entity"] = entity1;
        
        // Try to extract actions from entity descriptions
        size_t space1 = entity1.rfind(' ');
        size_t space2 = entity2.rfind(' ');
        
        if (space1 != std::string::npos && space2 != std::string::npos) {
            vars["action1"] = entity1.substr(space1 + 1);
            vars["action2"] = entity2.substr(space2 + 1);
            vars["entity"] = entity1.substr(0, space1);
        } else {
            vars["action1"] = entity1;
            vars["action2"] = entity2;
        }
    }
    
    auto it = templates.find(templateId);
    if (it != templates.end()) {
        return substitute(it->second.pattern, vars);
    }
    
    return "Contradiction between " + entity1 + " and " + entity2;
}

std::string DescriptionTemplates::generateConflictDescription(
    const std::string& entity1,
    const std::string& entity2,
    const std::string& reason) const {
    
    std::map<std::string, std::string> vars;
    vars["entity"] = entity1;
    vars["entity1"] = entity1;
    vars["entity2"] = entity2;
    vars["reason"] = reason;
    
    std::string templateId = "conflict_regulation";
    if (reason.find("payment") != std::string::npos) {
        templateId = "conflict_payment";
    }
    
    auto it = templates.find(templateId);
    if (it != templates.end()) {
        return substitute(it->second.pattern, vars);
    }
    
    return "Conflict between " + entity1 + " and " + entity2 + ": " + reason;
}

std::string DescriptionTemplates::generateViolationDescription(
    const std::string& violator,
    const std::string& violatedRule,
    const std::string& context) const {
    
    std::map<std::string, std::string> vars;
    vars["entity"] = violator;
    vars["violator"] = violator;
    vars["rule"] = violatedRule;
    vars["reason"] = context;
    vars["constraint"] = context;
    
    std::string templateId = "violation_necessary";
    
    auto it = templates.find(templateId);
    if (it != templates.end()) {
        return substitute(it->second.pattern, vars);
    }
    
    return violatedRule + " violated by " + violator;
}

std::string DescriptionTemplates::generateComplianceDescription(
    const std::string& entity,
    const std::string& rule,
    const std::string& action) const {
    
    std::map<std::string, std::string> vars;
    vars["entity"] = entity;
    vars["obligation"] = rule;
    vars["requirement"] = rule;
    vars["action"] = action;
    
    std::string templateId = "compliance_fulfilled";
    
    auto it = templates.find(templateId);
    if (it != templates.end()) {
        return substitute(it->second.pattern, vars);
    }
    
    return entity + " complies with " + rule;
}

std::string DescriptionTemplates::substitute(const std::string& templateStr,
                                            const std::map<std::string, std::string>& variables) const {
    std::string result = templateStr;
    
    for (const auto& [key, value] : variables) {
        std::string placeholder = "{" + key + "}";
        size_t pos = 0;
        while ((pos = result.find(placeholder, pos)) != std::string::npos) {
            result.replace(pos, placeholder.length(), value);
            pos += value.length();
        }
    }
    
    return result;
}

std::string DescriptionTemplates::findBestTemplate(const std::string& category,
                                                  const std::map<std::string, std::string>& /* context */) const {
    // Logic to find the best matching template based on context
    // For now, return the first template in the category
    for (const auto& [id, tmpl] : templates) {
        if (id.find(category) == 0) {
            return id;
        }
    }
    return "";
}

// InferenceConfiguration implementation
InferenceConfiguration& InferenceConfiguration::getInstance() {
    static InferenceConfiguration instance;
    return instance;
}

InferenceConfiguration::InferenceConfiguration() {
    // Initialize with defaults
    applyConfiguration();
}

void InferenceConfiguration::loadFromFile(const std::string& path) {
    // In a full implementation, this would parse JSON from file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open configuration file: " << path << std::endl;
        return;
    }
    
    // For now, we'll use the default configuration
    applyConfiguration();
}

void InferenceConfiguration::loadFromString(const std::string& /* json */) {
    // In a full implementation, this would parse JSON string
    // For now, we'll use the default configuration
    applyConfiguration();
}

void InferenceConfiguration::applyConfiguration() {
    // Apply entity mappings
    for (const auto& [entity, displayName] : config.entityMappings) {
        entityResolver.addEntityMapping(entity, displayName);
    }
    
    // Apply special characters
    for (const auto& [from, to] : config.specialCharacters) {
        entityResolver.addSpecialCharMapping(from, to);
    }
    
    // Apply action mappings
    for (const auto& [action, mapping] : config.actionMappings) {
        entityResolver.addActionMapping(action, mapping);
    }
    
    // Apply templates
    for (const auto& [id, pattern] : config.contradictionTemplates) {
        descriptionTemplates.addTemplate("contradiction_" + id, pattern);
    }
    
    for (const auto& [id, pattern] : config.conflictTemplates) {
        descriptionTemplates.addTemplate("conflict_" + id, pattern);
    }
    
    for (const auto& [id, pattern] : config.violationTemplates) {
        descriptionTemplates.addTemplate("violation_" + id, pattern);
    }
    
    for (const auto& [id, pattern] : config.complianceTemplates) {
        descriptionTemplates.addTemplate("compliance_" + id, pattern);
    }
}

}