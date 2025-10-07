#include "metta_inference/semantic_analyzer.hpp"
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cctype>
#include <map>

namespace metta_inference {

// StateOfAffairs implementation
std::string StateOfAffairs::toString() const {
    std::ostringstream oss;
    if (!agent.empty()) {
        oss << agent << " ";
    }
    oss << action;
    if (!instrument.empty()) {
        oss << " using " << instrument;
    }
    if (!exists) {
        oss << " (negated)";
    }
    return oss.str();
}

// LogicalContradiction implementation
std::string LogicalContradiction::getDescription(const EntityResolver& resolver,
                                                const DescriptionTemplates& templates) const {
    std::string entity1Desc = positive.toString();
    std::string entity2Desc = negative.toString();
    
    if (type == "action") {
        std::string agent = resolver.resolveEntity(positive.agent);
        std::string action = resolver.resolveAction(positive.action);
        return templates.generateContradictionDescription(
            agent + " " + action,
            agent + " does not " + action,
            "action"
        );
    } else if (type == "payment") {
        return templates.generateContradictionDescription(
            entity1Desc,
            entity2Desc,
            "payment"
        );
    }
    
    return templates.generateContradictionDescription(entity1Desc, entity2Desc, type);
}

// RegulatoryConflict implementation
std::string RegulatoryConflict::getDescription(const EntityResolver& resolver,
                                              const DescriptionTemplates& templates) const {
    std::string reg1 = resolver.resolveEntity(regulation1);
    std::string reg2 = resolver.resolveEntity(regulation2);
    std::string entity = resolver.resolveEntity(affectedEntity);
    
    std::string reason = reg1 + " conflicts with " + reg2 + " regarding " + conflictingRequirement;
    
    return templates.generateConflictDescription(reg1, reg2, reason);
}

// NecessaryViolation implementation
std::string NecessaryViolation::getDescription(const EntityResolver& resolver,
                                              const DescriptionTemplates& templates) const {
    std::string rule = resolver.resolveEntity(violatedRule);
    std::string viol = resolver.resolveEntity(violator);
    
    return templates.generateViolationDescription(viol, rule, reason);
}

// ComplianceRelation implementation
std::string ComplianceRelation::getDescription(const EntityResolver& resolver,
                                              const DescriptionTemplates& templates) const {
    std::string ent = resolver.resolveEntity(entity);
    std::string obl = resolver.resolveEntity(obligation);
    std::string action = resolver.resolveEntity(fulfilledBy);
    
    return templates.generateComplianceDescription(ent, obl, action);
}

// AnalysisResult implementation
Metrics SemanticAnalyzer::AnalysisResult::toMetrics() const {
    Metrics metrics;
    
    // Convert inferred facts
    metrics.inferredFacts = static_cast<int>(inferredFacts.size());
    for (const auto& fact : inferredFacts) {
        metrics.inferredStateOfAffairs.push_back(fact.toString());
    }
    
    // Convert contradictions
    metrics.contradictions = static_cast<int>(contradictions.size());
    metrics.contradictionPairs = static_cast<int>(contradictions.size());
    
    auto& config = InferenceConfiguration::getInstance();
    for (const auto& contradiction : contradictions) {
        ContradictionDetail detail;
        detail.entity1 = contradiction.positive.toString();
        detail.entity2 = contradiction.negative.toString();
        detail.description = contradiction.getDescription(
            config.getEntityResolver(),
            config.getTemplates()
        );
        metrics.contradictionDetails.push_back(detail);
    }
    
    // Convert conflicts
    metrics.conflicts = static_cast<int>(conflicts.size());
    for (const auto& conflict : conflicts) {
        ConflictDetail detail;
        detail.entity1 = conflict.regulation1;
        detail.entity2 = conflict.regulation2;
        detail.description = conflict.getDescription(
            config.getEntityResolver(),
            config.getTemplates()
        );
        metrics.conflictDetails.push_back(detail);
    }
    
    // Convert violations
    metrics.violations = static_cast<int>(violations.size());
    for (const auto& violation : violations) {
        ViolationDetail detail;
        detail.violator = violation.violator;
        detail.violated_rule = violation.violatedRule;
        detail.description = violation.getDescription(
            config.getEntityResolver(),
            config.getTemplates()
        );
        metrics.violationDetails.push_back(detail);
    }
    
    // Convert compliances
    metrics.compliances = static_cast<int>(compliances.size());
    
    return metrics;
}

// SemanticAnalyzer implementation
SemanticAnalyzer::SemanticAnalyzer() {
    auto& config = InferenceConfiguration::getInstance();
    entityResolver = &config.getEntityResolver();
    descriptionTemplates = &config.getTemplates();
}

SemanticAnalyzer::SemanticAnalyzer(EntityResolver* resolver, DescriptionTemplates* templates)
    : entityResolver(resolver), descriptionTemplates(templates) {
    if (!entityResolver || !descriptionTemplates) {
        auto& config = InferenceConfiguration::getInstance();
        if (!entityResolver) entityResolver = &config.getEntityResolver();
        if (!descriptionTemplates) descriptionTemplates = &config.getTemplates();
    }
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze(const std::string& mettaOutput) {
    AnalysisResult result;
    
    // Parse the output into S-expressions
    std::vector<std::shared_ptr<SExpr>> expressions;
    try {
        expressions = SExprParser::parseMultiple(mettaOutput);
    } catch (const std::exception& e) {
        // If parsing fails, fall back to line-by-line parsing
        std::istringstream iss(mettaOutput);
        std::string line;
        while (std::getline(iss, line)) {
            if (line.empty() || (line[0] != '(' && line[0] != '[')) continue;
            try {
                expressions.push_back(SExprParser::parse(line));
            } catch (...) {
                // Skip unparseable lines
            }
        }
    }
    
    // Extract different types of semantic information
    result.inferredFacts = extractStateOfAffairs(expressions);
    result.contradictions = findContradictions(expressions);
    result.conflicts = findConflicts(expressions);
    result.violations = findViolations(expressions);
    result.compliances = findCompliances(expressions);
    
    return result;
}

std::vector<StateOfAffairs> SemanticAnalyzer::extractStateOfAffairs(
    const std::vector<std::shared_ptr<SExpr>>& expressions) {
    
    std::vector<StateOfAffairs> results;
    std::unordered_set<std::string> processed;
    
    // Group all triples by entity
    std::map<std::string, std::vector<std::shared_ptr<SExpr>>> entityTriples;
    
    // Collect all triples
    for (const auto& expr : expressions) {
        if (!expr->isList()) continue;
        
        const auto& list = expr->asList();
        // Handle nested lists (like the output from metta-repl)
        for (const auto& elem : list) {
            if (elem->isList()) {
                const auto& subList = elem->asList();
                if (subList.size() >= 4 && subList[0]->getSymbol() && 
                    *subList[0]->getSymbol() == "triple") {
                    auto subject = subList[1]->getSymbol();
                    if (subject && subject->find("soa_") == 0) {
                        entityTriples[*subject].push_back(elem);
                    }
                }
            }
        }
    }
    
    // Process each entity's triples to build StateOfAffairs
    for (const auto& [entity, triples] : entityTriples) {
        // Skip already processed entities and special entities
        if (processed.count(entity) > 0) continue;
        if (entity == "soa_eo" || entity == "soa_ea") continue;
        if (entity.find("disjunction") != std::string::npos) continue;
        if (entity.find("id_not_not_false") != std::string::npos) continue;
        
        StateOfAffairs soa;
        soa.entity = entity;
        bool hasAction = false;
        bool exists = false;
        
        // Extract information from all triples for this entity
        for (const auto& triple : triples) {
            const auto& tripleList = triple->asList();
            if (tripleList.size() >= 4) {
                auto predicate = tripleList[2]->getSymbol();
                auto object = tripleList[3]->getSymbol();
                
                if (predicate && object) {
                    if (*predicate == "type") {
                        if (*object == "rexist") {
                            exists = true;
                            soa.exists = true;
                        } else if (object->find("soa") == 0) {
                            // This is an action type (soaLeave, soaMoor, soaPay, etc.)
                            hasAction = true;
                            std::string actionType = *object;
                            
                            // Convert soaLeave to "leave", soaMoor to "moor", etc.
                            if (actionType.size() > 3 && actionType.substr(0, 3) == "soa") {
                                actionType = actionType.substr(3);
                                // Convert first letter to lowercase
                                if (!actionType.empty()) {
                                    actionType[0] = std::tolower(actionType[0]);
                                }
                            }
                            soa.action = actionType;
                        }
                    } else if (*predicate == "soaHas_agent") {
                        // Extract agent
                        soa.agent = entityResolver->resolveEntity(*object);
                    } else if (*predicate == "soaHas_instrument") {
                        // Extract instrument
                        soa.instrument = entityResolver->resolveEntity(*object);
                    }
                }
            }
        }
        
        // Only add if we found a meaningful action or existence
        if (hasAction && exists) {
            processed.insert(entity);
            results.push_back(soa);
        }
    }
    
    return results;
}

std::vector<LogicalContradiction> SemanticAnalyzer::findContradictions(
    const std::vector<std::shared_ptr<SExpr>>& expressions) {
    
    std::vector<LogicalContradiction> results;
    std::map<std::string, std::vector<std::string>> contradictoryEntitiesByAction;
    std::vector<std::string> idNotNotFalseEntities;
    
    // Look for meta-id expressions indicating contradictions
    for (const auto& expr : expressions) {
        if (!expr->isList()) continue;
        
        const auto& list = expr->asList();
        
        // Handle nested lists (like the output from metta-repl)
        for (const auto& elem : list) {
            // Skip comma tokens
            if (elem->isAtom() && elem->asAtom() == ",") continue;
            
            if (elem->isList()) {
                const auto& subList = elem->asList();
                // Check if this sub-list contains a meta-id expression
                if (subList.size() >= 2) {
                    auto first = subList[0];
                    if (first->isList()) {
                        auto firstList = first->asList();
                        if (!firstList.empty() && firstList[0]->getSymbol() == std::optional<std::string>("meta-id") && 
                            firstList.size() >= 5 && firstList[4]->getSymbol() && *firstList[4]->getSymbol() == "true") {
                            // Check for id_not_not_false pattern
                            if (subList.size() > 1 && subList[1]->isList()) {
                                auto secondList = subList[1]->asList();
                                if (!secondList.empty() && secondList[0]->getSymbol() && 
                                    *secondList[0]->getSymbol() == "id_not_not_false") {
                                    // This is a contradictory entity
                                    auto entitySymbol = firstList[1]->getSymbol();
                                    if (entitySymbol) {
                                        std::string entity = *entitySymbol;
                                        idNotNotFalseEntities.push_back(entity);
                                        // Extract base action from entity
                                        std::string baseAction = extractBaseAction(entity);
                                        if (!baseAction.empty()) {
                                            contradictoryEntitiesByAction[baseAction].push_back(entity);
                                        }
                                    }
                                }
                            }
                        } else {
                            // Handle other contradiction patterns (like type rexist false)
                            auto contradiction = parseMetaContradiction(elem);
                            if (contradiction) {
                                results.push_back(*contradiction);
                            }
                        }
                    }
                }
            }
        }
        
        // Also check the top-level expression itself
        if (list.size() >= 2) {
            auto first = list[0];
            if (first->isList()) {
                auto firstList = first->asList();
                if (!firstList.empty() && firstList[0]->getSymbol() == std::optional<std::string>("meta-id")) {
                    auto contradiction = parseMetaContradiction(expr);
                    if (contradiction) {
                        results.push_back(*contradiction);
                    }
                }
            }
        }
    }
    
    // Create contradictions for entities with id_not_not_false pattern
    // Each entity with this pattern represents a contradiction:
    // The entity exists (rexist true) but is also marked as contradicted (id_not_not_false)
    if (!idNotNotFalseEntities.empty()) {
        // Check if they are payment-related entities
        bool arePaymentEntities = true;
        for (const auto& entity : idNotNotFalseEntities) {
            if (entity.find("INRS") == std::string::npos && 
                entity.find("USDS") == std::string::npos) {
                arePaymentEntities = false;
                break;
            }
        }
        
        if (arePaymentEntities) {
            // For payment entities, we have domain knowledge that:
            // - If paying in INRS, then NOT paying in USDS
            // - If paying in USDS, then NOT paying in INRS
            // 
            // So when we see both soa_epamINRS and soa_epamUSDS with id_not_not_false,
            // it means:
            // 1. soa_epamINRS exists but is contradicted (by inference from USDS that says NOT INRS)
            // 2. soa_epamUSDS exists but is contradicted (by inference from INRS that says NOT USDS)
            
            for (const auto& entity : idNotNotFalseEntities) {
                LogicalContradiction contradiction;
                
                // Determine which instrument this entity uses
                std::string instrument = (entity.find("INRS") != std::string::npos) ? "INRS" : "USDS";
                std::string oppositeInstrument = (instrument == "INRS") ? "USDS" : "INRS";
                
                // The positive side: the entity exists and pays with this instrument
                contradiction.positive.entity = entityResolver->resolveEntity(entity);
                contradiction.positive.action = "pays in " + instrument;
                contradiction.positive.instrument = instrument;
                contradiction.positive.exists = true;
                
                // The negative side: inferred from the opposite payment that this shouldn't exist
                contradiction.negative.entity = entityResolver->resolveEntity(entity);
                contradiction.negative.action = "does not pay in " + instrument;
                contradiction.negative.instrument = instrument;
                contradiction.negative.exists = false;
                
                contradiction.type = "payment_method";
                results.push_back(contradiction);
            }
        }
    }
    
    // Group contradictory entities by action and create proper contradiction pairs
    for (const auto& [baseAction, entities] : contradictoryEntitiesByAction) {
        if (entities.size() >= 2) {
            // Find positive and negative versions
            std::string positiveEntity, negativeEntity;
            for (const auto& entity : entities) {
                if (entityResolver->isNegatedEntity(entity)) {
                    negativeEntity = entity;
                } else {
                    positiveEntity = entity;
                }
            }
            
            if (!positiveEntity.empty() && !negativeEntity.empty()) {
                LogicalContradiction contradiction;
                contradiction.positive.entity = entityResolver->resolveEntity(positiveEntity);
                contradiction.positive.action = entityResolver->resolveAction(positiveEntity);
                contradiction.negative.entity = entityResolver->resolveEntity(negativeEntity);
                contradiction.negative.action = "not " + entityResolver->resolveAction(negativeEntity);
                contradiction.type = "action";
                results.push_back(contradiction);
            }
        }
    }
    
    // Also look for id_not_not_false patterns
    auto notNotFalse = SExprMatcher::findAll(expressions, {"id_not_not_false", "?"});
    std::unordered_set<std::string> contradictoryEntities;
    
    for (const auto& expr : notNotFalse) {
        auto values = SExprMatcher::extract(expr, {"id_not_not_false", "?"});
        if (!values.empty()) {
            contradictoryEntities.insert(values[0]);
        }
    }
    
    // Group contradictory entities into pairs
    for (const auto& entity : contradictoryEntities) {
        if (entityResolver->isNegatedEntity(entity)) {
            std::string base = entityResolver->getBaseForm(entity);
            if (contradictoryEntities.count(base) > 0) {
                LogicalContradiction contradiction;
                contradiction.positive.entity = base;
                contradiction.negative.entity = entity;
                contradiction.type = "existence";
                results.push_back(contradiction);
            }
        }
    }
    
    return results;
}

std::vector<RegulatoryConflict> SemanticAnalyzer::findConflicts(
    const std::vector<std::shared_ptr<SExpr>>& expressions) {
    
    std::vector<RegulatoryConflict> results;
    
    // Look for conflict expressions
    for (const auto& expr : expressions) {
        if (!expr->isList()) continue;
        
        const auto& list = expr->asList();
        
        // Handle lists containing multiple conflict expressions like:
        // [(conflict not_opt soa_elam), (conflict (mod-not-id soa_elam permitted) soa_elam)]
        bool foundConflictInList = false;
        for (const auto& elem : list) {
            // Skip comma tokens if present
            if (elem->isAtom() && elem->asAtom() == ",") continue;
            
            if (elem->isList()) {
                auto conflict = parseConflictExpr(elem);
                if (conflict) {
                    results.push_back(*conflict);
                    foundConflictInList = true;
                }
            }
        }
        
        // If we found conflicts in the list, continue to next expression
        if (foundConflictInList) {
            continue;
        }
        
        // Handle single wrapped expressions like [(conflict ...)]
        if (list.size() == 1 && list[0]->isList()) {
            // Check if the inner expression is a conflict
            auto conflict = parseConflictExpr(list[0]);
            if (conflict) {
                results.push_back(*conflict);
                continue;
            }
        }
        
        // Try direct parsing
        auto conflict = parseConflictExpr(expr);
        if (conflict) {
            results.push_back(*conflict);
        }
    }
    
    return results;
}

std::vector<NecessaryViolation> SemanticAnalyzer::findViolations(
    const std::vector<std::shared_ptr<SExpr>>& expressions) {
    
    std::vector<NecessaryViolation> results;
    
    // Look for quote expressions indicating violations
    for (const auto& expr : expressions) {
        if (!expr->isList()) continue;
        
        // Handle wrapped expressions like [(quote ...)]
        const auto& list = expr->asList();
        if (list.size() == 1 && list[0]->isList()) {
            // Check if the inner expression is a violation
            auto violation = parseViolationExpr(list[0]);
            if (violation) {
                results.push_back(*violation);
                continue;
            }
        }
        
        // Try direct parsing
        auto violation = parseViolationExpr(expr);
        if (violation) {
            results.push_back(*violation);
        }
    }
    
    return results;
}

std::vector<ComplianceRelation> SemanticAnalyzer::findCompliances(
    const std::vector<std::shared_ptr<SExpr>>& expressions) {
    
    std::vector<ComplianceRelation> results;
    
    // Look for compliance expressions in two formats:
    // 1. Explicit format: (is_complied_with_by obligation entity)
    auto compliances = SExprMatcher::findAll(expressions, {"is_complied_with_by", "?", "?"});
    
    for (const auto& expr : compliances) {
        auto compliance = parseComplianceExpr(expr);
        if (compliance) {
            results.push_back(*compliance);
        }
    }
    
    // 2. Tuple format from inference: [(obligation entity)]
    // This appears as a list containing a 2-element list
    for (const auto& expr : expressions) {
        if (!expr->isList()) continue;
        
        const auto& outerList = expr->asList();
        
        // Check if this is a single-element list containing a pair
        if (outerList.size() == 1 && outerList[0]->isList()) {
            const auto& innerList = outerList[0]->asList();
            
            // Check if it's a 2-element list with SOA entities
            if (innerList.size() == 2) {
                auto first = innerList[0]->getSymbol();
                auto second = innerList[1]->getSymbol();
                
                if (first && second) {
                    // Check if these look like SOA entities (start with "soa_")
                    if (first->substr(0, 4) == "soa_" && second->substr(0, 4) == "soa_") {
                        // Determine which is obligation and which is the complying action
                        // Typically, obligations have patterns like soa_enpam (not-pay prohibited)
                        // and actions like soa_epam15k (actual payment)
                        ComplianceRelation compliance;
                        
                        // If first entity contains 'en' (negation), it's likely the obligation
                        if (first->find("en") != std::string::npos && 
                            second->find("en") == std::string::npos) {
                            compliance.obligation = entityResolver->resolveEntity(*first);
                            compliance.entity = entityResolver->resolveEntity(*second);
                            compliance.fulfilledBy = *second;
                        } else if (second->find("en") != std::string::npos && 
                                   first->find("en") == std::string::npos) {
                            compliance.obligation = entityResolver->resolveEntity(*second);
                            compliance.entity = entityResolver->resolveEntity(*first);
                            compliance.fulfilledBy = *first;
                        } else {
                            // Default: assume first is obligation, second is action
                            compliance.obligation = entityResolver->resolveEntity(*first);
                            compliance.entity = entityResolver->resolveEntity(*second);
                            compliance.fulfilledBy = *second;
                        }
                        
                        results.push_back(compliance);
                    }
                }
            }
        }
        
        // Also check for direct 2-element lists at top level
        if (outerList.size() == 2) {
            auto first = outerList[0]->getSymbol();
            auto second = outerList[1]->getSymbol();
            
            if (first && second && 
                first->substr(0, 4) == "soa_" && second->substr(0, 4) == "soa_") {
                ComplianceRelation compliance;
                
                // Similar logic for determining obligation vs action
                if (first->find("en") != std::string::npos && 
                    second->find("en") == std::string::npos) {
                    compliance.obligation = entityResolver->resolveEntity(*first);
                    compliance.entity = entityResolver->resolveEntity(*second);
                    compliance.fulfilledBy = *second;
                } else {
                    compliance.obligation = entityResolver->resolveEntity(*first);
                    compliance.entity = entityResolver->resolveEntity(*second);
                    compliance.fulfilledBy = *second;
                }
                
                results.push_back(compliance);
            }
        }
    }
    
    return results;
}

std::optional<StateOfAffairs> SemanticAnalyzer::parseTripleToSOA(const std::shared_ptr<SExpr>& triple) {
    auto tripleOpt = SExprTriple::fromSExpr(triple);
    if (!tripleOpt) return std::nullopt;
    
    StateOfAffairs soa;
    soa.entity = tripleOpt->subject;
    
    // Extract action type from another triple
    // This would need to look for (triple entity type soaAction)
    // For now, simplified implementation
    soa.action = entityResolver->resolveAction(tripleOpt->subject);
    
    return soa;
}

std::optional<LogicalContradiction> SemanticAnalyzer::parseMetaContradiction(const std::shared_ptr<SExpr>& expr) {
    if (!expr->isList()) return std::nullopt;
    
    const auto& list = expr->asList();
    if (list.size() < 2) return std::nullopt;
    
    LogicalContradiction contradiction;
    
    // Parse the meta-id structure
    auto first = list[0];
    if (first->isList()) {
        const auto& metaList = first->asList();
        
        // Check for (meta-id entity type rexist false) pattern
        if (metaList.size() >= 5) {
            auto firstSymbol = metaList[0]->getSymbol();
            if (firstSymbol && *firstSymbol == "meta-id") {
                auto entitySymbol = metaList[1]->getSymbol();
                auto typeSymbol = metaList[2]->getSymbol();
                auto rexistSymbol = metaList[3]->getSymbol();
                auto valueSymbol = metaList[4]->getSymbol();
                
                if (entitySymbol && typeSymbol && *typeSymbol == "type" &&
                    rexistSymbol && *rexistSymbol == "rexist") {
                    
                    std::string entityName = *entitySymbol;
                    
                    if (valueSymbol && *valueSymbol == "false") {
                        // Pattern: (meta-id entity type rexist false) - entity doesn't exist
                        contradiction.positive.entity = entityName;
                        contradiction.negative.entity = "not_" + entityName;
                        contradiction.type = "existence";
                        
                        // Add description based on the second part if available
                        if (list.size() > 1 && list[1]->isList()) {
                            const auto& detailList = list[1]->asList();
                            if (!detailList.empty()) {
                                auto detailType = detailList[0]->getSymbol();
                                if (detailType && detailType->find("inrs-not-usds") != std::string::npos) {
                                    contradiction.positive.action = "uses INRS";
                                    contradiction.negative.action = "uses USDS";
                                    contradiction.type = "payment_method";
                                }
                            }
                        }
                        
                        return contradiction;
                    } else if (valueSymbol && *valueSymbol == "true") {
                        // Pattern: (meta-id entity type rexist true) + (id_not_not_false entity)
                        // Check if second part has id_not_not_false
                        if (list.size() > 1 && list[1]->isList()) {
                            const auto& secondList = list[1]->asList();
                            if (!secondList.empty()) {
                                auto secondSymbol = secondList[0]->getSymbol();
                                if (secondSymbol && *secondSymbol == "id_not_not_false") {
                                    // This indicates a contradiction
                                    std::string resolvedEntity = entityResolver->resolveEntity(entityName);
                                    std::string action = entityResolver->resolveAction(entityName);
                                    
                                    // Determine if this is a negated entity
                                    bool isNegated = false;
                                    std::string baseEntity = entityName;
                                    if (entityName.length() > 6 && entityName.substr(0, 6) == "soa_en") {
                                        isNegated = true;
                                        baseEntity = "soa_e" + entityName.substr(6); // Remove "n" from "soa_en"
                                    }
                                    
                                    if (isNegated) {
                                        contradiction.negative.entity = resolvedEntity;
                                        contradiction.negative.action = action;
                                        contradiction.positive.entity = entityResolver->resolveEntity(baseEntity);
                                        contradiction.positive.action = entityResolver->resolveAction(baseEntity);
                                    } else {
                                        contradiction.positive.entity = resolvedEntity;
                                        contradiction.positive.action = action;
                                        // Find the corresponding negated entity
                                        std::string negatedEntity = entityName;
                                        if (entityName.length() > 4 && entityName.substr(0, 4) == "soa_e") {
                                            negatedEntity = "soa_en" + entityName.substr(4); // Add "n" after "soa_e"
                                        }
                                        contradiction.negative.entity = entityResolver->resolveEntity(negatedEntity);
                                        contradiction.negative.action = "not " + action;
                                    }
                                    
                                    contradiction.type = "action";
                                    return contradiction;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // Fallback to original logic
        auto metaExpr = MetaExpr::fromSExpr(first);
        if (metaExpr) {
            contradiction.positive.entity = metaExpr->id;
            contradiction.type = "property";
            
            // The second part typically contains the contradicting information
            if (list.size() > 1 && list[1]->isList()) {
                // Extract contradiction details
                contradiction.negative.entity = "not_" + metaExpr->id;
            }
        }
    }
    
    return contradiction;
}

std::optional<RegulatoryConflict> SemanticAnalyzer::parseConflictExpr(const std::shared_ptr<SExpr>& expr) {
    if (!expr->isList()) return std::nullopt;
    
    const auto& list = expr->asList();
    if (list.size() < 3) return std::nullopt;
    
    auto first = list[0]->getSymbol();
    if (!first || *first != "conflict") return std::nullopt;
    
    RegulatoryConflict conflict;
    
    // Parse first regulation - can be either a list (complex regulation) or atom (simple entity)
    if (list[1]->isList()) {
        const auto& reg1 = list[1]->asList();
        if (!reg1.empty() && reg1[0]->getSymbol()) {
            std::string regulation = *reg1[0]->getSymbol();
            
            // Extract entity if present
            if (reg1.size() > 1 && reg1[1]->getSymbol()) {
                conflict.affectedEntity = *reg1[1]->getSymbol();
            }
            
            if (regulation == "inrs-prohibited-id") {
                conflict.regulation1 = "EU MiCA regulation (INRS prohibition)";
            } else if (regulation == "mod-not-id") {
                // Handle modality expressions like (mod-not-id soa_elam permitted)
                std::string entity = (reg1.size() > 1 && reg1[1]->getSymbol()) ? *reg1[1]->getSymbol() : "";
                std::string modality = (reg1.size() > 2 && reg1[2]->getSymbol()) ? *reg1[2]->getSymbol() : "";
                conflict.regulation1 = entityResolver->resolveEntity(entity) + " is not " + modality;
                conflict.affectedEntity = entity;
            } else {
                conflict.regulation1 = regulation;
            }
        }
    } else if (list[1]->getSymbol()) {
        // Handle simple atoms like "not_opt"
        std::string regulation = *list[1]->getSymbol();
        if (regulation == "not_opt") {
            conflict.regulation1 = "Not optional (prohibited)";
        } else {
            conflict.regulation1 = entityResolver->resolveEntity(regulation);
        }
    }
    
    // Parse second regulation - can be either a list (complex regulation) or atom (simple entity)
    if (list[2]->isList()) {
        const auto& reg2 = list[2]->asList();
        if (!reg2.empty() && reg2[0]->getSymbol()) {
            std::string regulation = *reg2[0]->getSymbol();
            
            if (regulation == "pay-obligatory-id" && reg2.size() > 2) {
                auto port = reg2[2]->getSymbol();
                if (port && *port == "soa_sptMICT") {
                    conflict.regulation2 = "MICT Smart Port payment obligation";
                } else {
                    conflict.regulation2 = "Payment obligation";
                }
            } else if (regulation == "inrs-only-id") {
                conflict.regulation2 = "INRS-only requirement";
            } else {
                conflict.regulation2 = regulation;
            }
        }
    } else if (list[2]->getSymbol()) {
        // Handle simple atoms like "soa_elam"
        std::string entity = *list[2]->getSymbol();
        conflict.regulation2 = entityResolver->resolveEntity(entity);
        if (conflict.affectedEntity.empty()) {
            conflict.affectedEntity = entity;
        }
    }
    
    // Extract affected entity name
    if (!conflict.affectedEntity.empty()) {
        conflict.affectedEntity = entityResolver->resolveEntity(conflict.affectedEntity);
    }
    
    // Set more appropriate conflict description based on the entities involved
    if (conflict.regulation1.find("Not optional") != std::string::npos && 
        conflict.regulation2.find("ALEXANDRA") != std::string::npos) {
        conflict.conflictingRequirement = "permission vs prohibition to leave";
    } else {
        conflict.conflictingRequirement = "regulatory requirements";
    }
    
    return conflict;
}

std::optional<NecessaryViolation> SemanticAnalyzer::parseViolationExpr(const std::shared_ptr<SExpr>& expr) {
    if (!expr->isList()) return std::nullopt;
    
    const auto& list = expr->asList();
    if (list.size() < 2) return std::nullopt;
    
    auto first = list[0]->getSymbol();
    if (!first || *first != "quote") return std::nullopt;
    
    NecessaryViolation violation;
    
    // Extract violation details from nested structure
    // Structure: (quote ((violator) (violated-rule)))
    if (list[1]->isList()) {
        const auto& violationPair = list[1]->asList();
        
        // Parse violator (first element)
        if (violationPair.size() > 0 && violationPair[0]->isList()) {
            const auto& violatorExpr = violationPair[0]->asList();
            if (!violatorExpr.empty() && violatorExpr[0]->getSymbol()) {
                std::string violatorId = *violatorExpr[0]->getSymbol();
                
                if (violatorId == "inrs-prohibited-id") {
                    violation.violator = "EU MiCA regulation (INRS prohibition)";
                    
                    // Extract entity
                    if (violatorExpr.size() > 1 && violatorExpr[1]->getSymbol()) {
                        std::string entity = entityResolver->resolveEntity(*violatorExpr[1]->getSymbol());
                        violation.violator = "EU MiCA regulation prohibiting " + entity + " from using INRS";
                    }
                } else {
                    violation.violator = violatorId;
                }
            }
        }
        
        // Parse violated rule (second element)
        if (violationPair.size() > 1 && violationPair[1]->isList()) {
            const auto& ruleExpr = violationPair[1]->asList();
            if (!ruleExpr.empty() && ruleExpr[0]->getSymbol()) {
                std::string ruleId = *ruleExpr[0]->getSymbol();
                
                if (ruleId == "inrs-only-id") {
                    violation.violatedRule = "MICT port INRS-only payment requirement";
                    
                    // Check if there's nested payment info
                    if (ruleExpr.size() > 1 && ruleExpr[1]->isList()) {
                        const auto& paymentExpr = ruleExpr[1]->asList();
                        if (!paymentExpr.empty() && paymentExpr[0]->getSymbol() && 
                            *paymentExpr[0]->getSymbol() == "pay-obligatory-id") {
                            violation.violatedRule = "MICT port INRS-only payment requirement";
                        }
                    }
                } else if (ruleId == "pay-obligatory-id") {
                    violation.violatedRule = "Port payment obligation";
                } else {
                    violation.violatedRule = ruleId;
                }
            }
        }
    }
    
    violation.reason = "conflicting regulatory requirements";
    
    return violation;
}

std::optional<ComplianceRelation> SemanticAnalyzer::parseComplianceExpr(const std::shared_ptr<SExpr>& expr) {
    if (!SExprMatcher::matches(expr, {"is_complied_with_by", "?", "?"})) {
        return std::nullopt;
    }
    
    auto values = SExprMatcher::extract(expr, {"is_complied_with_by", "?", "?"});
    if (values.size() < 2) return std::nullopt;
    
    ComplianceRelation compliance;
    compliance.obligation = values[0];
    compliance.entity = values[1];
    compliance.fulfilledBy = "action";
    
    return compliance;
}

bool SemanticAnalyzer::areEntitiesContradictory(const std::string& entity1, const std::string& entity2) {
    // Check if one is the negation of the other
    return (entityResolver->isNegatedEntity(entity1) && 
            entityResolver->getBaseForm(entity1) == entity2) ||
           (entityResolver->isNegatedEntity(entity2) && 
            entityResolver->getBaseForm(entity2) == entity1);
}

std::pair<std::string, std::string> SemanticAnalyzer::extractContradictoryPair(
    const std::string& entity1, const std::string& entity2) {
    
    if (entityResolver->isNegatedEntity(entity1)) {
        return {entityResolver->getBaseForm(entity1), entity1};
    } else if (entityResolver->isNegatedEntity(entity2)) {
        return {entity2, entityResolver->getBaseForm(entity2)};
    }
    
    return {entity1, entity2};
}

std::string SemanticAnalyzer::extractBaseAction(const std::string& soaEntity) {
    // Extract base action from SOA entities like soa_emam, soa_enmam, soa_eplm, soa_enplm
    if (soaEntity.length() < 7 || soaEntity.substr(0, 4) != "soa_") {
        return "";
    }
    
    std::string suffix = soaEntity.substr(4); // Remove "soa_"
    
    // Remove negation prefix if present (en)
    if (suffix.length() > 2 && suffix.substr(0, 2) == "en") {
        suffix = suffix.substr(2); // Remove "en"
    }
    
    // Remove entity prefix (e)
    if (suffix.length() > 1 && suffix[0] == 'e') {
        suffix = suffix.substr(1); // Remove "e"
    }
    
    return suffix; // Return action suffix like "mam", "plm"
}

std::map<std::string, std::string> SemanticAnalyzer::extractContext(const std::shared_ptr<SExpr>& expr) {
    std::map<std::string, std::string> context;
    
    // Extract relevant context from the expression
    if (expr->isList()) {
        const auto& list = expr->asList();
        for (const auto& elem : list) {
            if (elem->isAtom()) {
                std::string atom = elem->asAtom();
                if (atom.find("soa_") == 0) {
                    context["entity"] = atom;
                } else if (atom.find("INRS") != std::string::npos) {
                    context["instrument"] = "INRS";
                } else if (atom.find("USDS") != std::string::npos) {
                    context["instrument"] = "USDS";
                }
            }
        }
    }
    
    return context;
}

// InferencePatternDetector implementation
InferencePatternDetector::InferencePatternDetector() {
    initializePatterns();
}

void InferencePatternDetector::initializePatterns() {
    // State of Affairs pattern
    Pattern p1;
    p1.type = PatternType::StateOfAffairsAssertion;
    p1.matchSequence = {"triple", "?", "type", "rexist"};
    p1.validator = [](const std::shared_ptr<SExpr>& expr) {
        return SExprMatcher::matches(expr, {"triple", "?", "type", "rexist"});
    };
    patterns.push_back(p1);
    
    // Contradiction pattern
    Pattern p2;
    p2.type = PatternType::ContradictionDetection;
    p2.matchSequence = {"id_not_not_false", "?"};
    p2.validator = [](const std::shared_ptr<SExpr>& expr) {
        return SExprMatcher::matches(expr, {"id_not_not_false", "?"});
    };
    patterns.push_back(p2);
    
    // Conflict pattern
    Pattern p3;
    p3.type = PatternType::ConflictIdentification;
    p3.matchSequence = {"conflict", "?", "?"};
    p3.validator = [](const std::shared_ptr<SExpr>& expr) {
        return SExprMatcher::matches(expr, {"conflict", "?", "?"});
    };
    patterns.push_back(p3);
    
    // Violation pattern
    Pattern p4;
    p4.type = PatternType::ViolationNecessity;
    p4.matchSequence = {"quote", "?"};
    p4.validator = [](const std::shared_ptr<SExpr>& expr) {
        return SExprMatcher::matches(expr, {"quote", "?"});
    };
    patterns.push_back(p4);
    
    // Compliance pattern
    Pattern p5;
    p5.type = PatternType::ComplianceFulfillment;
    p5.matchSequence = {"is_complied_with_by", "?", "?"};
    p5.validator = [](const std::shared_ptr<SExpr>& expr) {
        return SExprMatcher::matches(expr, {"is_complied_with_by", "?", "?"});
    };
    patterns.push_back(p5);
}

InferencePatternDetector::PatternType InferencePatternDetector::detectPattern(
    const std::shared_ptr<SExpr>& expr) const {
    
    for (const auto& pattern : patterns) {
        if (pattern.validator(expr)) {
            return pattern.type;
        }
    }
    
    return PatternType::Unknown;
}

std::vector<std::shared_ptr<SExpr>> InferencePatternDetector::findPatternsOfType(
    const std::vector<std::shared_ptr<SExpr>>& expressions,
    PatternType type) const {
    
    std::vector<std::shared_ptr<SExpr>> results;
    
    for (const auto& expr : expressions) {
        if (detectPattern(expr) == type) {
            results.push_back(expr);
        }
    }
    
    return results;
}

}