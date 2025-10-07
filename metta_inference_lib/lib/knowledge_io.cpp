#include "metta_inference/knowledge_io.hpp"
#include "metta_inference/sexpr_parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cctype>

namespace metta_inference {

// Triple implementation
std::string Triple::toString() const {
    std::ostringstream oss;
    oss << "(" << tripleType << " " << subject << " " << predicate << " ";
    // Handle nested expressions (like (15000USD)) properly
    if (objectIsExpression || (object.size() > 0 && object[0] == '(')) {
        oss << object;
    } else {
        oss << object;
    }
    oss << ")";
    return oss.str();
}

// Condition implementation
std::string Condition::toString() const {
    std::ostringstream oss;
    oss << "(" << variable << " " << expression << ")";
    return oss.str();
}

// Norm implementation  
std::string Norm::toString() const {
    std::ostringstream oss;
    
    // Add description as comment if present
    if (!description.empty()) {
        oss << "; " << description << "\n";
    }
    
    // Write the norm definition
    oss << "(= (" << name;
    for (const auto& param : parameters) {
        oss << " " << param;
    }
    oss << ")\n";
    
    // Write conditions if any
    if (!conditions.empty()) {
        oss << "  (let* (";
        for (size_t i = 0; i < conditions.size(); ++i) {
            if (i > 0) oss << "\n         ";
            oss << conditions[i].toString();
        }
        oss << ")\n    True))\n";
    } else {
        oss << "  True)\n";
    }
    
    // Write consequences as separate rules
    for (const auto& triple : consequences) {
        oss << "(= (ct-triple-for-add " << triple.subject << " " 
            << triple.predicate << " " << triple.object << ")\n";
        oss << "   (let* ((True (" << name;
        for (const auto& param : parameters) {
            oss << " " << param;
        }
        oss << ")))\n     True))\n";
    }
    
    return oss.str();
}

// Eventuality implementation
bool Eventuality::isValid() const {
    // Must have type, modality (rexist), and agent
    return !name.empty() && !type.empty() && 
           modality == "rexist" && !agent.empty();
}

std::string Eventuality::getExpectedName() const {
    if (type.empty() || agent.empty()) return "";
    
    // Extract first letter of eventuality type (after "soa" prefix if present)
    std::string typeInitial;
    if (type.substr(0, 3) == "soa" && type.length() > 3) {
        typeInitial = std::tolower(type[3]);
    } else if (!type.empty()) {
        typeInitial = std::tolower(type[0]);
    }
    
    // Extract agent initials
    std::string agentInitials;
    std::string agentName = agent;
    
    // Remove "soa_" prefix if present
    if (agentName.substr(0, 4) == "soa_") {
        agentName = agentName.substr(4);
    }
    
    // Extract initials from agent name (handle underscores as word separators)
    bool newWord = true;
    for (char c : agentName) {
        if (c == '_' || c == '-') {
            newWord = true;
        } else if (newWord && std::isalpha(c)) {
            agentInitials += std::tolower(c);
            newWord = false;
        }
    }
    
    return "soa_e" + typeInitial + agentInitials;
}

// LogicalExpression implementation
std::string LogicalExpression::toString() const {
    std::ostringstream oss;
    
    if (type == EQUAL) {
        oss << "(= ";
        if (name.empty()) {
            // Direct equality expression
            oss << "(" << (operands.size() > 0 ? operands[0] : "") << ")";
        } else {
            // Named logical expression
            if (operands.size() == 1 && operands[0].find(" ") != std::string::npos) {
                // Parse the operator type from the operand
                oss << "(" << operands[0] << ")";
            } else {
                oss << "(" << name << ")";
            }
        }
        if (operands.size() > 1) {
            oss << " (" << operands[1];
            for (size_t i = 2; i < operands.size(); ++i) {
                oss << " " << operands[i];
            }
            oss << ")";
        }
        oss << ")";
    } else {
        std::string op = (type == AND) ? "ct-and" : 
                        (type == OR) ? "ct-or" : "ct-not";
        oss << "(= (" << op << " " << name << ")";
        if (!operands.empty()) {
            oss << " (";
            for (size_t i = 0; i < operands.size(); ++i) {
                if (i > 0) oss << " ";
                oss << operands[i];
            }
            oss << ")";
        }
        oss << ")";
    }
    return oss.str();
}

// Entity implementation
std::string Entity::toString() const {
    std::ostringstream oss;
    oss << "(ct-triple " << name << " type " << type << ")";
    for (const auto& [key, value] : properties) {
        oss << "\n(ct-triple " << name << " " << key << " " << value << ")";
    }
    return oss.str();
}

// Negation implementation
std::string Negation::toString() const {
    std::ostringstream oss;
    oss << "(ct-simple-not " << name << " " << negatedEntity << ")";
    if (!name.empty()) {
        oss << "\n(ct-triple " << name << " type rexist)";
    }
    return oss.str();
}

// StateOfAffairs implementation
std::string StateOfAffairs::toString() const {
    std::ostringstream oss;
    
    if (!description.empty()) {
        oss << "; " << description << "\n\n";
    }
    
    // Output logical expressions first
    for (const auto& logExpr : logicalExpressions) {
        oss << logExpr.toString() << "\n";
    }
    
    // Output facts/triples
    for (const auto& fact : facts) {
        oss << fact.toString() << "\n";
    }
    
    // Output negations
    for (const auto& negation : negations) {
        oss << negation.toString() << "\n";
    }
    
    // Output entities (non-eventuality entities)
    for (const auto& [name, entity] : entities) {
        oss << entity.toString() << "\n";
    }
    
    return oss.str();
}

bool StateOfAffairs::validateEventualities(std::vector<std::string>& errors) const {
    bool valid = true;
    
    for (const auto& [name, eventuality] : eventualities) {
        // Check required fields
        if (!eventuality.isValid()) {
            errors.push_back("Eventuality '" + name + "' is missing required fields (type, rexist modality, or agent)");
            valid = false;
        }
        
        // Check naming convention
        std::string expectedName = eventuality.getExpectedName();
        if (name != expectedName) {
            errors.push_back("Eventuality '" + name + "' does not follow naming convention. Expected: '" + expectedName + "'");
            valid = false;
        }
        
        // Validate eventuality type
        if (!KnowledgeIO::isValidEventualityType(eventuality.type)) {
            errors.push_back("Invalid eventuality type '" + eventuality.type + "' for eventuality '" + name + "'");
            valid = false;
        }
        
        // Validate roles
        for (const auto& [role, value] : eventuality.roles) {
            if (!KnowledgeIO::isValidRole(role)) {
                errors.push_back("Invalid role '" + role + "' for eventuality '" + name + "'");
                valid = false;
            }
        }
    }
    
    return valid;
}

bool StateOfAffairs::validateEntities(std::vector<std::string>& errors) const {
    bool valid = true;
    
    for (const auto& [name, entity] : entities) {
        // Check that entity has a type
        if (entity.type.empty()) {
            errors.push_back("Entity '" + name + "' is missing a type");
            valid = false;
        }
        
        // Check that entity name doesn't conflict with eventuality names
        if (eventualities.find(name) != eventualities.end()) {
            errors.push_back("Entity '" + name + "' conflicts with eventuality name");
            valid = false;
        }
    }
    
    return valid;
}

// Helper function implementations
std::string KnowledgeIO::trimWhitespace(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

std::vector<std::string> KnowledgeIO::splitParameters(const std::string& paramStr) {
    std::vector<std::string> params;
    std::istringstream iss(paramStr);
    std::string param;
    
    while (iss >> param) {
        param = trimWhitespace(param);
        if (!param.empty() && param != "(" && param != ")") {
            params.push_back(param);
        }
    }
    
    return params;
}

bool KnowledgeIO::isCommentLine(const std::string& line) {
    std::string trimmed = trimWhitespace(line);
    return trimmed.empty() || trimmed[0] == ';';
}

bool KnowledgeIO::isNormDefinition(const std::string& line) {
    std::string trimmed = trimWhitespace(line);
    return trimmed.find("(=") == 0;
}

bool KnowledgeIO::isTripleDefinition(const std::string& line) {
    std::string trimmed = trimWhitespace(line);
    return trimmed.find("(ct-triple") == 0 || 
           trimmed.find("(meta-triple") == 0;
}

bool KnowledgeIO::isNegationDefinition(const std::string& line) {
    std::string trimmed = trimWhitespace(line);
    return trimmed.find("(ct-simple-not") == 0;
}

bool KnowledgeIO::isLogicalDefinition(const std::string& line) {
    std::string trimmed = trimWhitespace(line);
    return trimmed.find("(= (ct-or") == 0 || 
           trimmed.find("(= (ct-and") == 0 ||
           trimmed.find("(=") == 0;  // General equality expressions
}

// Parse a single triple from MeTTa code
std::optional<Triple> KnowledgeIO::parseTriple(const std::string& mettaCode) {
    try {
        auto expr = SExprParser::parse(mettaCode);
        return parseTripleFromExpr(expr);
    } catch (const std::exception& e) {
        return std::nullopt;
    }
}

// Parse triple from SExpr - handles nested expressions in object position
std::optional<Triple> KnowledgeIO::parseTripleFromExpr(const std::shared_ptr<SExpr>& expr) {
    if (!expr || !expr->isList()) return std::nullopt;
    
    const auto& list = expr->asList();
    if (list.size() != 4) return std::nullopt;
    
    auto tripleType = list[0]->getSymbol();
    if (!tripleType || (*tripleType != "ct-triple" && *tripleType != "meta-triple")) {
        return std::nullopt;
    }
    
    auto subject = list[1]->getSymbol();
    auto predicate = list[2]->getSymbol();
    
    if (!subject || !predicate) return std::nullopt;
    
    Triple triple;
    triple.tripleType = *tripleType;
    triple.subject = *subject;
    triple.predicate = *predicate;
    
    // Handle object - can be either a symbol or a nested expression
    if (list[3]->isAtom()) {
        triple.object = list[3]->asAtom();
        triple.objectIsExpression = false;
    } else if (list[3]->isList()) {
        // It's a nested expression like (15000USD)
        triple.object = list[3]->toString();
        triple.objectIsExpression = true;
    } else {
        return std::nullopt;
    }
    
    return triple;
}

// Parse negation expression
std::optional<Negation> KnowledgeIO::parseNegation(const std::shared_ptr<SExpr>& expr) {
    if (!expr || !expr->isList()) return std::nullopt;
    
    const auto& list = expr->asList();
    if (list.size() != 3) return std::nullopt;
    
    auto op = list[0]->getSymbol();
    if (!op || *op != "ct-simple-not") return std::nullopt;
    
    auto name = list[1]->getSymbol();
    auto negated = list[2]->getSymbol();
    
    if (!name || !negated) return std::nullopt;
    
    Negation negation;
    negation.name = *name;
    negation.negatedEntity = *negated;
    
    return negation;
}

// Parse logical expression (ct-or, ct-and, or general equality)
std::optional<LogicalExpression> KnowledgeIO::parseLogicalExpression(const std::shared_ptr<SExpr>& expr) {
    if (!expr || !expr->isList()) return std::nullopt;
    
    const auto& list = expr->asList();
    if (list.size() < 2) return std::nullopt;
    
    auto first = list[0]->getSymbol();
    if (!first || *first != "=") return std::nullopt;
    
    LogicalExpression logExpr;
    
    // Check if it's (= (ct-or name) (operands...))
    if (list[1]->isList()) {
        const auto& opList = list[1]->asList();
        if (opList.size() >= 2) {
            auto opType = opList[0]->getSymbol();
            auto opName = opList[1]->getSymbol();
            
            if (opType && opName) {
                if (*opType == "ct-or") {
                    logExpr.type = LogicalExpression::OR;
                    logExpr.name = *opName;
                } else if (*opType == "ct-and") {
                    logExpr.type = LogicalExpression::AND;
                    logExpr.name = *opName;
                } else {
                    // General equality expression
                    logExpr.type = LogicalExpression::EQUAL;
                    logExpr.name = *opType;
                    if (opName) {
                        logExpr.operands.push_back(*opName);
                    }
                }
            }
        }
    }
    
    // Parse operands if there's a second part
    if (list.size() >= 3 && list[2]->isList()) {
        const auto& operandList = list[2]->asList();
        for (const auto& operand : operandList) {
            if (auto sym = operand->getSymbol()) {
                logExpr.operands.push_back(*sym);
            }
        }
    }
    
    return logExpr;
}

// Parse entity from a triple
std::optional<Entity> KnowledgeIO::parseEntity(const Triple& triple) {
    // An entity is a triple where the predicate is 'type' and subject is not an eventuality
    if (triple.predicate != "type") return std::nullopt;
    if (triple.subject.substr(0, 5) == "soa_e") return std::nullopt; // It's an eventuality
    
    Entity entity;
    entity.name = triple.subject;
    entity.type = triple.object;
    
    return entity;
}

// Parse a norm from MeTTa code  
std::optional<Norm> KnowledgeIO::parseNorm(const std::string& mettaCode) {
    try {
        // Parse the entire norm structure
        auto expr = SExprParser::parse(mettaCode);
        if (!expr->isList()) return std::nullopt;
        
        const auto& list = expr->asList();
        if (list.size() < 2) return std::nullopt;
        
        // Check if it's an = expression
        auto equals = list[0]->getSymbol();
        if (!equals || *equals != "=") return std::nullopt;
        
        // Parse norm header (name and parameters)
        if (!list[1]->isList()) return std::nullopt;
        const auto& header = list[1]->asList();
        if (header.empty()) return std::nullopt;
        
        auto normName = header[0]->getSymbol();
        if (!normName) return std::nullopt;
        
        Norm norm;
        norm.name = *normName;
        
        // Extract parameters
        for (size_t i = 1; i < header.size(); ++i) {
            if (auto param = header[i]->getSymbol()) {
                norm.parameters.push_back(*param);
            }
        }
        
        // Parse the body - look for let* block
        if (list.size() >= 3 && list[2]->isList()) {
            const auto& body = list[2]->asList();
            if (!body.empty() && body[0]->getSymbol() && *body[0]->getSymbol() == "let*") {
                // Extract conditions from let* block
                if (body.size() >= 2 && body[1]->isList()) {
                    const auto& conditions = body[1]->asList();
                    for (const auto& condExpr : conditions) {
                        if (condExpr->isList()) {
                            const auto& condList = condExpr->asList();
                            if (condList.size() >= 2) {
                                auto variable = condList[0]->getSymbol();
                                if (variable && condList[1]->isList()) {
                                    Condition cond;
                                    cond.variable = *variable;
                                    // Convert expression list back to string
                                    cond.expression = condList[1]->toString();
                                    // Remove outer parentheses if present
                                    if (!cond.expression.empty() && cond.expression[0] == '(') {
                                        cond.expression = cond.expression.substr(1, cond.expression.length() - 2);
                                    }
                                    norm.conditions.push_back(cond);
                                }
                            }
                        }
                    }
                }
            }
        }
        
        return norm;
    } catch (const std::exception& e) {
        return std::nullopt;
    }
}

// Extract norms from MeTTa content
std::vector<Norm> KnowledgeIO::extractNormsFromMetta(const std::string& mettaContent) {
    std::vector<Norm> norms;
    
    // Parse all expressions from the content
    std::vector<std::shared_ptr<SExpr>> expressions;
    try {
        expressions = SExprParser::parseMultiple(mettaContent);
    } catch (const std::exception& e) {
        // If full parsing fails, try parsing line by line
        std::istringstream iss(mettaContent);
        std::string line;
        std::ostringstream currentExpr;
        int parenDepth = 0;
        bool inExpr = false;
        
        while (std::getline(iss, line)) {
            // Skip comments and empty lines
            if (isCommentLine(line)) continue;
            
            // Check if this starts a new expression
            if (line.find("(=") != std::string::npos && !inExpr) {
                inExpr = true;
                parenDepth = 0;
                currentExpr.str("");
                currentExpr.clear();
            }
            
            if (inExpr) {
                currentExpr << line << "\n";
                
                // Count parentheses
                for (char c : line) {
                    if (c == '(') parenDepth++;
                    else if (c == ')') parenDepth--;
                }
                
                // If we've balanced the parentheses, parse the expression
                if (parenDepth <= 0) {
                    try {
                        auto expr = SExprParser::parse(currentExpr.str());
                        expressions.push_back(expr);
                    } catch (...) {
                        // Skip unparseable expressions
                    }
                    inExpr = false;
                }
            }
        }
    }
    
    // Process each expression to find norms
    for (const auto& expr : expressions) {
        if (!expr->isList()) continue;
        
        const auto& list = expr->asList();
        if (list.size() >= 2) {
            auto first = list[0]->getSymbol();
            if (first && *first == "=") {
                // This is a norm definition
                auto norm = parseNorm(expr->toString());
                if (norm) {
                    norms.push_back(*norm);
                }
            }
        }
    }
    
    return norms;
}

// Extract state of affairs from MeTTa content
StateOfAffairs KnowledgeIO::extractStateOfAffairsFromMetta(const std::string& mettaContent) {
    StateOfAffairs soa;
    
    // Parse all expressions from the content
    std::vector<std::shared_ptr<SExpr>> expressions;
    std::vector<std::string> parseErrors;
    
    try {
        expressions = SExprParser::parseMultiple(mettaContent);
    } catch (const std::exception& e) {
        // If full parsing fails, try parsing line by line with better error reporting
        std::istringstream iss(mettaContent);
        std::string line;
        std::string currentExpr;
        int lineNum = 0;
        int parenDepth = 0;
        bool inExpr = false;
        
        while (std::getline(iss, line)) {
            lineNum++;
            
            // Skip comments and empty lines
            if (isCommentLine(line)) {
                // Extract description from special comment markers
                if (line.find("State of Affairs") != std::string::npos) {
                    size_t start = line.find("(");
                    size_t end = line.rfind(")");
                    if (start != std::string::npos && end != std::string::npos) {
                        soa.description = line.substr(start + 1, end - start - 1);
                    }
                }
                continue;
            }
            
            // Check if this starts a new expression
            if ((isTripleDefinition(line) || isNegationDefinition(line) || 
                 isLogicalDefinition(line)) && !inExpr) {
                inExpr = true;
                parenDepth = 0;
                currentExpr = "";
            }
            
            if (inExpr) {
                currentExpr += line + "\n";
                
                // Count parentheses
                for (char c : line) {
                    if (c == '(') parenDepth++;
                    else if (c == ')') parenDepth--;
                }
                
                // If we've balanced the parentheses, parse the expression
                if (parenDepth <= 0) {
                    try {
                        auto expr = SExprParser::parse(currentExpr);
                        expressions.push_back(expr);
                    } catch (const std::exception& e) {
                        parseErrors.push_back("Line " + std::to_string(lineNum) + ": " + e.what());
                    }
                    inExpr = false;
                }
            }
        }
    }
    
    // Process each expression
    for (const auto& expr : expressions) {
        if (!expr->isList()) continue;
        
        const auto& list = expr->asList();
        if (list.empty()) continue;
        
        auto first = list[0]->getSymbol();
        if (!first) continue;
        
        // Handle different expression types
        if (*first == "ct-triple" || *first == "meta-triple") {
            // Parse as triple
            if (auto triple = parseTripleFromExpr(expr)) {
                soa.facts.push_back(*triple);
                
                // Check if this defines an entity
                if (auto entity = parseEntity(*triple)) {
                    soa.entities[entity->name] = *entity;
                }
                
                // Parse eventualities from triples
                if (triple->subject.substr(0, 5) == "soa_e") {
                    std::string eventualityName = triple->subject;
                    
                    // Initialize eventuality if not exists
                    if (soa.eventualities.find(eventualityName) == soa.eventualities.end()) {
                        soa.eventualities[eventualityName] = Eventuality();
                        soa.eventualities[eventualityName].name = eventualityName;
                    }
                    
                    // Parse eventuality properties
                    if (triple->predicate == "type") {
                        // Check if it's an eventuality type or modality
                        if (isValidEventualityType(triple->object)) {
                            soa.eventualities[eventualityName].type = triple->object;
                        } else if (isValidModality(triple->object)) {
                            soa.eventualities[eventualityName].modality = triple->object;
                        }
                    } else if (triple->predicate == "soaHas_agent") {
                        soa.eventualities[eventualityName].agent = triple->object;
                    } else if (triple->predicate.substr(0, 7) == "soaHas_") {
                        // Store other roles
                        soa.eventualities[eventualityName].roles[triple->predicate] = triple->object;
                    }
                }
                
                // Also check for entity properties
                if (triple->subject.find("soa_") != 0 || triple->subject.substr(0, 5) != "soa_e") {
                    // This might be a property of an entity
                    if (soa.entities.find(triple->subject) != soa.entities.end()) {
                        if (triple->predicate != "type") {
                            soa.entities[triple->subject].properties[triple->predicate] = triple->object;
                        }
                    }
                }
            }
        } else if (*first == "ct-simple-not") {
            // Parse as negation
            if (auto negation = parseNegation(expr)) {
                soa.negations.push_back(*negation);
            }
        } else if (*first == "=") {
            // Parse as logical expression
            if (auto logExpr = parseLogicalExpression(expr)) {
                soa.logicalExpressions.push_back(*logExpr);
            }
        }
    }
    
    // Report parse errors if any
    if (!parseErrors.empty()) {
        std::cerr << "Parse errors encountered:\n";
        for (const auto& error : parseErrors) {
            std::cerr << "  " << error << "\n";
        }
    }
    
    return soa;
}

// Read norms from file
std::vector<Norm> KnowledgeIO::readNormsFromFile(const fs::path& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath.string());
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    
    return extractNormsFromMetta(buffer.str());
}

// Read state of affairs from file
StateOfAffairs KnowledgeIO::readStateOfAffairsFromFile(const fs::path& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath.string());
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    
    return extractStateOfAffairsFromMetta(buffer.str());
}

// Write norms to file
void KnowledgeIO::writeNormsToFile(const std::vector<Norm>& norms, const fs::path& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create file: " + filepath.string());
    }
    
    file << "; Norms generated by MeTTa Inference Library\n\n";
    
    for (const auto& norm : norms) {
        file << norm.toString() << "\n";
    }
}

// Write state of affairs to file
void KnowledgeIO::writeStateOfAffairsToFile(const StateOfAffairs& soa, const fs::path& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create file: " + filepath.string());
    }
    
    file << "; State of Affairs generated by MeTTa Inference Library\n\n";
    file << soa.toString();
}

// Read complete MeTTa document
KnowledgeIO::MettaDocument KnowledgeIO::readMettaDocument(const fs::path& filepath) {
    MettaDocument doc;
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath.string());
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    
    // Extract header comments (before first norm or triple)
    size_t firstNorm = content.find("(=");
    size_t firstTriple = content.find("(ct-triple");
    size_t contentStart = std::min(firstNorm, firstTriple);
    
    if (contentStart != std::string::npos && contentStart > 0) {
        doc.header = content.substr(0, contentStart);
    }
    
    // Extract norms and state of affairs
    doc.norms = extractNormsFromMetta(content);
    doc.stateOfAffairs = extractStateOfAffairsFromMetta(content);
    
    return doc;
}

// Write complete MeTTa document
void KnowledgeIO::writeMettaDocument(const MettaDocument& doc, const fs::path& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create file: " + filepath.string());
    }
    
    // Write header if present
    if (!doc.header.empty()) {
        file << doc.header << "\n\n";
    }
    
    // Write norms section
    if (!doc.norms.empty()) {
        file << "; ========== NORMS ==========\n\n";
        for (const auto& norm : doc.norms) {
            file << norm.toString() << "\n";
        }
    }
    
    // Write state of affairs section
    if (!doc.stateOfAffairs.facts.empty()) {
        file << "\n; ========== STATE OF AFFAIRS ==========\n\n";
        file << doc.stateOfAffairs.toString();
    }
}

// Validation implementations
bool KnowledgeIO::validateEventuality(const Eventuality& eventuality, std::string& error) {
    // More flexible validation - only check for critical fields
    if (eventuality.name.empty()) {
        error = "Eventuality missing name";
        return false;
    }
    
    if (eventuality.type.empty() && eventuality.modality.empty()) {
        error = "Eventuality '" + eventuality.name + "' missing both type and modality";
        return false;
    }
    
    // Check eventuality type if present (but allow unknown types with warning)
    if (!eventuality.type.empty() && !isValidEventualityType(eventuality.type)) {
        // Just warn, don't fail
        std::cerr << "Warning: Unknown eventuality type '" << eventuality.type 
                  << "' for eventuality '" << eventuality.name << "'\n";
    }
    
    // Check modality if present
    if (!eventuality.modality.empty() && !isValidModality(eventuality.modality)) {
        error = "Invalid modality '" + eventuality.modality + "' for eventuality '" + eventuality.name + "'";
        return false;
    }
    
    // Optional: Check naming convention (but just warn, don't fail)
    if (!eventuality.type.empty() && !eventuality.agent.empty()) {
        std::string expectedName = eventuality.getExpectedName();
        if (eventuality.name != expectedName) {
            std::cerr << "Warning: Eventuality name '" << eventuality.name 
                      << "' does not follow convention. Expected: '" << expectedName << "'\n";
        }
    }
    
    return true;
}

bool KnowledgeIO::validatePredicate(const std::string& predicate, std::string& error) {
    // More flexible validation - allow more predicates
    if (predicate == "type" || 
        predicate.substr(0, 7) == "soaHas_" || 
        predicate.substr(0, 4) == "soa_" ||
        predicate == "associated-with" ||
        predicate == "soa_associated-with" ||
        isValidRole(predicate)) {
        return true;
    }
    // Just warn for unknown predicates, don't fail
    std::cerr << "Warning: Unknown predicate '" << predicate << "'\n";
    return true; // Allow unknown predicates for flexibility
}

bool KnowledgeIO::isValidEventualityType(const std::string& type) {
    return getValidEventualityTypes().count(type) > 0;
}

bool KnowledgeIO::isValidRole(const std::string& role) {
    return getValidRoles().count(role) > 0;
}

bool KnowledgeIO::isValidModality(const std::string& modality) {
    return getValidModalities().count(modality) > 0;
}

const std::set<std::string>& KnowledgeIO::getValidEventualityTypes() {
    static const std::set<std::string> types = {
        // Smart Port example Eventuality types
        "soaMoor", "soaPay", "soaLeave", "soaContainerVessel",
        // Entity types that might appear
        "soaContainerVessel", "soa_mooringBerth", "smartport",
        // Demand Side Stablecoin Primitive Eventuality types
        "soaIdentify", "soaDevelop", "soaOnboard", "soaActivate",
        "soaInvoke", "soaCreate", "soaUpdate", "soaReview",
        "soaReward", "soaUnreward", "soaCalculate", "soaIssue",
        "soaSettle", "soaReimburse", "soaVerify",
        // Additional types that might be used
        "soaDeclare", "soaRegister", "soaTransfer", "soaValidate",
        // Allow abbreviated forms
        "Pay", "Moor", "Leave"
    };
    return types;
}

const std::set<std::string>& KnowledgeIO::getValidRoles() {
    static const std::set<std::string> roles = {
        "soaHas_agent", "soaHas_beneficiary", "soaHas_cause", "soaHas_goal",
        "soaHas_instrument", "soaHas_partner", "soaHas_patient", "soaHas_pivot",
        "soaHas_purpose", "soaHas_reason", "soaHas_result", "soaHas_setting",
        "soaHas_source", "soaHas_theme", "soaHas_time", "soaHas_manner",
        "soaHas_medium", "soaHas_means", "soaHas_location", "soaHas_initial-location",
        "soaHas_final-location", "soaHas_distance", "soaHas_duration", "soaHas_initial-time",
        "soaHas_final-time", "soaHas_path", "soaHas_amount", "soaHas_attribute"
    };
    return roles;
}

const std::set<std::string>& KnowledgeIO::getValidModalities() {
    static const std::set<std::string> modalities = {
        "rexist",        // Really exists (required for state of affairs)
        "obligatory",    // Deontic modalities
        "permitted",
        "optional"
    };
    return modalities;
}

// ExpressionProcessor implementation
void ExpressionProcessor::process(const std::shared_ptr<SExpr>& expr, ExpressionVisitor& visitor) {
    if (!expr || !expr->isList()) return;
    
    const auto& list = expr->asList();
    if (list.empty()) return;
    
    auto first = list[0]->getSymbol();
    if (!first) return;
    
    // Determine expression type and call appropriate visitor method
    if (*first == "ct-triple" || *first == "meta-triple") {
        if (auto triple = KnowledgeIO::parseTripleFromExpr(expr)) {
            visitor.visitTriple(*triple);
            
            // Also check if it defines an entity
            if (auto entity = KnowledgeIO::parseEntity(*triple)) {
                visitor.visitEntity(*entity);
            }
        }
    } else if (*first == "ct-simple-not") {
        if (auto negation = KnowledgeIO::parseNegation(expr)) {
            visitor.visitNegation(*negation);
        }
    } else if (*first == "=") {
        if (auto logExpr = KnowledgeIO::parseLogicalExpression(expr)) {
            visitor.visitLogicalExpression(*logExpr);
        }
    }
}

std::string ExpressionProcessor::extractExpressionType(const std::shared_ptr<SExpr>& expr) {
    if (!expr || !expr->isList()) return "unknown";
    
    const auto& list = expr->asList();
    if (list.empty()) return "unknown";
    
    auto first = list[0]->getSymbol();
    if (!first) return "unknown";
    
    if (*first == "ct-triple" || *first == "meta-triple") {
        return "triple";
    } else if (*first == "ct-simple-not") {
        return "negation";
    } else if (*first == "=") {
        // Look deeper to determine type
        if (list.size() >= 2 && list[1]->isList()) {
            const auto& opList = list[1]->asList();
            if (!opList.empty()) {
                auto op = opList[0]->getSymbol();
                if (op) {
                    if (*op == "ct-or") return "logical-or";
                    if (*op == "ct-and") return "logical-and";
                }
            }
        }
        return "logical-equal";
    }
    
    return "unknown";
}

} // namespace metta_inference