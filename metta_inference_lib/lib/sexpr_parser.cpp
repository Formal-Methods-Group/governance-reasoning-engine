#include "metta_inference/sexpr_parser.hpp"
#include <sstream>
#include <cctype>
#include <algorithm>

namespace metta_inference {

// SExpr implementation
std::string SExpr::toString() const {
    if (isAtom()) {
        return asAtom();
    } else {
        std::ostringstream oss;
        oss << "(";
        const auto& list = asList();
        for (size_t i = 0; i < list.size(); ++i) {
            if (i > 0) oss << " ";
            oss << list[i]->toString();
        }
        oss << ")";
        return oss.str();
    }
}

// Tokenizer implementation
SExprParser::Tokenizer::Tokenizer(const std::string& input) 
    : input(input), position(0) {
    skipWhitespace();
}

bool SExprParser::Tokenizer::hasNext() const {
    return position < input.length();
}

std::string SExprParser::Tokenizer::peek() const {
    if (!hasNext()) return "";
    
    if (input[position] == '(' || input[position] == ')' || 
        input[position] == '[' || input[position] == ']') {
        return std::string(1, input[position]);
    }
    
    // Read atom
    size_t start = position;
    size_t end = position;
    
    while (end < input.length() && !isDelimiter(input[end])) {
        end++;
    }
    
    return input.substr(start, end - start);
}

std::string SExprParser::Tokenizer::next() {
    std::string token = peek();
    consume();
    return token;
}

void SExprParser::Tokenizer::consume() {
    if (!hasNext()) return;
    
    if (input[position] == '(' || input[position] == ')' ||
        input[position] == '[' || input[position] == ']') {
        position++;
    } else {
        while (position < input.length() && !isDelimiter(input[position])) {
            position++;
        }
    }
    
    skipWhitespace();
}

void SExprParser::Tokenizer::skipWhitespace() {
    while (position < input.length() && std::isspace(input[position])) {
        position++;
    }
}

bool SExprParser::Tokenizer::isDelimiter(char c) const {
    return c == '(' || c == ')' || c == '[' || c == ']' || std::isspace(c);
}

// Parser implementation
std::shared_ptr<SExpr> SExprParser::parse(const std::string& input) {
    Tokenizer tokenizer(input);
    if (!tokenizer.hasNext()) {
        throw std::runtime_error("Empty input");
    }
    return parseExpression(tokenizer);
}

std::vector<std::shared_ptr<SExpr>> SExprParser::parseMultiple(const std::string& input) {
    std::vector<std::shared_ptr<SExpr>> results;
    Tokenizer tokenizer(input);
    
    while (tokenizer.hasNext()) {
        results.push_back(parseExpression(tokenizer));
    }
    
    return results;
}

std::shared_ptr<SExpr> SExprParser::parseExpression(Tokenizer& tokenizer) {
    if (!tokenizer.hasNext()) {
        throw std::runtime_error("Unexpected end of input");
    }
    
    std::string token = tokenizer.peek();
    
    if (token == "(" || token == "[") {
        return parseList(tokenizer);
    } else if (token == ")" || token == "]") {
        throw std::runtime_error("Unexpected closing bracket");
    } else {
        tokenizer.consume();
        return std::make_shared<SExpr>(token);
    }
}

std::shared_ptr<SExpr> SExprParser::parseList(Tokenizer& tokenizer) {
    std::string open = tokenizer.next();
    if (open != "(" && open != "[") {
        throw std::runtime_error("Expected '(' or '['");
    }
    
    SExpr::List elements;
    
    std::string expectedClose = (open == "(") ? ")" : "]";
    
    while (tokenizer.hasNext() && tokenizer.peek() != expectedClose) {
        elements.push_back(parseExpression(tokenizer));
    }
    
    if (!tokenizer.hasNext() || tokenizer.next() != expectedClose) {
        throw std::runtime_error("Expected '" + expectedClose + "'");
    }
    
    return std::make_shared<SExpr>(elements);
}

// SExprTriple implementation
std::optional<SExprTriple> SExprTriple::fromSExpr(const std::shared_ptr<SExpr>& expr) {
    if (!expr->isList()) return std::nullopt;
    
    const auto& list = expr->asList();
    if (list.size() != 4) return std::nullopt;
    
    auto first = list[0]->getSymbol();
    if (!first || *first != "triple") return std::nullopt;
    
    auto subject = list[1]->getSymbol();
    auto predicate = list[2]->getSymbol();
    auto object = list[3]->getSymbol();
    
    if (!subject || !predicate || !object) return std::nullopt;
    
    return SExprTriple{*subject, *predicate, *object};
}

// MetaExpr implementation
std::optional<MetaExpr> MetaExpr::fromSExpr(const std::shared_ptr<SExpr>& expr) {
    if (!expr->isList()) return std::nullopt;
    
    const auto& list = expr->asList();
    if (list.size() < 2) return std::nullopt;
    
    auto first = list[0]->getSymbol();
    if (!first || *first != "meta-id") return std::nullopt;
    
    MetaExpr result;
    
    if (auto id = list[1]->getSymbol()) {
        result.id = *id;
    } else {
        return std::nullopt;
    }
    
    if (list.size() >= 3 && list[2]->getSymbol()) {
        result.type = *list[2]->getSymbol();
    }
    
    if (list.size() >= 4 && list[3]->getSymbol()) {
        result.property = *list[3]->getSymbol();
    }
    
    if (list.size() >= 5 && list[4]->getSymbol()) {
        result.value = *list[4]->getSymbol();
    }
    
    return result;
}

// SExprMatcher implementation
bool SExprMatcher::matches(const std::shared_ptr<SExpr>& expr, 
                          const std::vector<std::string>& pattern) {
    if (!expr->isList()) {
        if (pattern.size() == 1) {
            auto atom = expr->getSymbol();
            return atom && (*atom == pattern[0] || pattern[0] == "?");
        }
        return false;
    }
    
    const auto& list = expr->asList();
    if (list.size() != pattern.size()) return false;
    
    for (size_t i = 0; i < list.size(); ++i) {
        if (pattern[i] == "?") continue;  // Wildcard
        
        auto symbol = list[i]->getSymbol();
        if (!symbol || *symbol != pattern[i]) {
            return false;
        }
    }
    
    return true;
}

std::vector<std::string> SExprMatcher::extract(const std::shared_ptr<SExpr>& expr,
                                              const std::vector<std::string>& pattern) {
    std::vector<std::string> results;
    
    if (!expr->isList()) return results;
    
    const auto& list = expr->asList();
    if (list.size() != pattern.size()) return results;
    
    for (size_t i = 0; i < list.size(); ++i) {
        if (pattern[i] == "?") {
            if (auto symbol = list[i]->getSymbol()) {
                results.push_back(*symbol);
            }
        }
    }
    
    return results;
}

std::vector<std::shared_ptr<SExpr>> SExprMatcher::findAll(
    const std::vector<std::shared_ptr<SExpr>>& exprs,
    const std::vector<std::string>& pattern) {
    
    std::vector<std::shared_ptr<SExpr>> results;
    
    for (const auto& expr : exprs) {
        if (matches(expr, pattern)) {
            results.push_back(expr);
        }
    }
    
    return results;
}

}