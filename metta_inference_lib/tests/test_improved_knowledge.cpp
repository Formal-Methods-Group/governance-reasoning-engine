#include "metta_inference/knowledge_io.hpp"
#include "metta_inference/sexpr_parser.hpp"
#include <iostream>
#include <string>

using namespace metta_inference;

// Test visitor implementation
class TestVisitor : public ExpressionVisitor {
public:
    int tripleCount = 0;
    int negationCount = 0;
    int logicalCount = 0;
    int entityCount = 0;
    
    void visitTriple(const Triple& triple) override {
        tripleCount++;
        std::cout << "  Triple: " << triple.toString() << "\n";
        if (triple.objectIsExpression) {
            std::cout << "    (Object is expression: " << triple.object << ")\n";
        }
    }
    
    void visitNegation(const Negation& negation) override {
        negationCount++;
        std::cout << "  Negation: " << negation.toString() << "\n";
    }
    
    void visitLogicalExpression(const LogicalExpression& expr) override {
        logicalCount++;
        std::cout << "  Logical: " << expr.toString() << "\n";
    }
    
    void visitEntity(const Entity& entity) override {
        entityCount++;
        std::cout << "  Entity: " << entity.name << " type " << entity.type << "\n";
    }
};

int main() {
    std::cout << "Testing improved knowledge_io with complex state of affairs examples\n\n";
    
    // Test 1: Parse triple with nested expression
    std::cout << "Test 1: Triple with nested expression (amount)\n";
    std::string test1 = "(ct-triple soa_ep15kiam soaHas_amount (15000USD))";
    auto triple1 = KnowledgeIO::parseTriple(test1);
    if (triple1) {
        std::cout << "  Success: " << triple1->toString() << "\n";
        std::cout << "  Object is expression: " << (triple1->objectIsExpression ? "yes" : "no") << "\n";
    } else {
        std::cout << "  Failed to parse\n";
    }
    
    // Test 2: Parse negation
    std::cout << "\nTest 2: Negation expression\n";
    std::string test2 = "(ct-simple-not soa_enmam soa_emam)";
    auto expr2 = SExprParser::parse(test2);
    auto negation2 = KnowledgeIO::parseNegation(expr2);
    if (negation2) {
        std::cout << "  Success: " << negation2->toString() << "\n";
    } else {
        std::cout << "  Failed to parse\n";
    }
    
    // Test 3: Parse logical OR expression
    std::cout << "\nTest 3: Logical OR expression\n";
    std::string test3 = "(= (ct-or soa_eo) (soa_elam soa_ea))";
    auto expr3 = SExprParser::parse(test3);
    auto logical3 = KnowledgeIO::parseLogicalExpression(expr3);
    if (logical3) {
        std::cout << "  Success: " << logical3->toString() << "\n";
        std::cout << "  Type: " << (logical3->type == LogicalExpression::OR ? "OR" : "OTHER") << "\n";
        std::cout << "  Name: " << logical3->name << "\n";
        std::cout << "  Operands: ";
        for (const auto& op : logical3->operands) {
            std::cout << op << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "  Failed to parse\n";
    }
    
    // Test 4: Parse logical AND expression
    std::cout << "\nTest 4: Logical AND expression\n";
    std::string test4 = "(= (ct-and soa_ea) (soa_emam soa_epam))";
    auto expr4 = SExprParser::parse(test4);
    auto logical4 = KnowledgeIO::parseLogicalExpression(expr4);
    if (logical4) {
        std::cout << "  Success: " << logical4->toString() << "\n";
        std::cout << "  Type: " << (logical4->type == LogicalExpression::AND ? "AND" : "OTHER") << "\n";
    } else {
        std::cout << "  Failed to parse\n";
    }
    
    // Test 5: Parse entity
    std::cout << "\nTest 5: Entity definition\n";
    std::string test5 = "(ct-triple ALEXANDRA_MAERSK type soaContainerVessel)";
    auto triple5 = KnowledgeIO::parseTriple(test5);
    if (triple5) {
        auto entity5 = KnowledgeIO::parseEntity(*triple5);
        if (entity5) {
            std::cout << "  Success: " << entity5->toString() << "\n";
        } else {
            std::cout << "  Not recognized as entity\n";
        }
    }
    
    // Test 6: Full state of affairs parsing
    std::cout << "\nTest 6: Full state of affairs document\n";
    std::string fullDoc = R"(
; State of Affairs (3) ALEXANDRA MÆRSK pays equivalent of 15000USD in INRS

(ct-triple soa_ep15kiam type soaPay)
(ct-triple soa_ep15kiam type rexist)
(ct-triple soa_ep15kiam soaHas_agent soa_ALEXANDRA_MAERSK)
(ct-triple soa_ep15kiam soaHas_amount (15000USD))
(ct-triple soa_ep15kiam soaHas_instrument soaINRS)

; Negation example
(ct-simple-not soa_enmam soa_emam)
(ct-triple soa_enmam type rexist)

; Logical expressions
(= (ct-or soa_eo) (soa_elam soa_ea))
(= (ct-and soa_ea) (soa_emam soa_epam))

; Entity definition
(ct-triple ALEXANDRA_MAERSK type soaContainerVessel)
(ct-triple soa_berthMICT type soa_mooringBerth)
(ct-triple soa_sptMICT soa_associated-with soa_berthMICT)
)";
    
    auto soa = KnowledgeIO::extractStateOfAffairsFromMetta(fullDoc);
    
    std::cout << "  Facts parsed: " << soa.facts.size() << "\n";
    std::cout << "  Eventualities: " << soa.eventualities.size() << "\n";
    std::cout << "  Entities: " << soa.entities.size() << "\n";
    std::cout << "  Negations: " << soa.negations.size() << "\n";
    std::cout << "  Logical expressions: " << soa.logicalExpressions.size() << "\n";
    
    // Test 7: Visitor pattern
    std::cout << "\nTest 7: Visitor pattern processing\n";
    TestVisitor visitor;
    auto expressions = SExprParser::parseMultiple(fullDoc);
    
    for (const auto& expr : expressions) {
        ExpressionProcessor::process(expr, visitor);
    }
    
    std::cout << "  Visitor counts:\n";
    std::cout << "    Triples: " << visitor.tripleCount << "\n";
    std::cout << "    Negations: " << visitor.negationCount << "\n";
    std::cout << "    Logical: " << visitor.logicalCount << "\n";
    std::cout << "    Entities: " << visitor.entityCount << "\n";
    
    // Test 8: Validation
    std::cout << "\nTest 8: Validation\n";
    std::vector<std::string> errors;
    bool valid = soa.validateEventualities(errors);
    std::cout << "  Eventualities valid: " << (valid ? "yes" : "no") << "\n";
    if (!errors.empty()) {
        for (const auto& error : errors) {
            std::cout << "    Error: " << error << "\n";
        }
    }
    
    valid = soa.validateEntities(errors);
    std::cout << "  Entities valid: " << (valid ? "yes" : "no") << "\n";
    if (!errors.empty()) {
        for (const auto& error : errors) {
            std::cout << "    Error: " << error << "\n";
        }
    }
    
    // Test 9: ToString output
    std::cout << "\nTest 9: StateOfAffairs toString output:\n";
    std::cout << soa.toString() << "\n";
    
    std::cout << "\nAll tests completed!\n";
    
    return 0;
}