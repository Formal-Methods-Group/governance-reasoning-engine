#ifndef METTA_INFERENCE_SEXPR_PARSER_HPP
#define METTA_INFERENCE_SEXPR_PARSER_HPP

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <variant>
#include <stdexcept>

namespace metta_inference {

// S-Expression AST structure
class SExpr {
public:
    using Atom = std::string;
    using List = std::vector<std::shared_ptr<SExpr>>;
    using Value = std::variant<Atom, List>;

    explicit SExpr(const std::string& atom) : value(atom) {}
    explicit SExpr(const List& list) : value(list) {}

    bool isAtom() const { return std::holds_alternative<Atom>(value); }
    bool isList() const { return std::holds_alternative<List>(value); }
    
    const Atom& asAtom() const {
        if (auto* atom = std::get_if<Atom>(&value)) {
            return *atom;
        }
        throw std::runtime_error("SExpr is not an atom");
    }
    
    const List& asList() const {
        if (auto* list = std::get_if<List>(&value)) {
            return *list;
        }
        throw std::runtime_error("SExpr is not a list");
    }
    
    std::string toString() const;
    
    // Helper methods for common patterns
    std::optional<std::string> getSymbol() const {
        if (isAtom()) return asAtom();
        return std::nullopt;
    }
    
    std::optional<std::shared_ptr<SExpr>> nth(size_t n) const {
        if (!isList()) return std::nullopt;
        const auto& list = asList();
        if (n >= list.size()) return std::nullopt;
        return list[n];
    }
    
    size_t length() const {
        if (isList()) return asList().size();
        return 1;
    }

private:
    Value value;
};

// S-Expression parser
class SExprParser {
public:
    static std::vector<std::shared_ptr<SExpr>> parseMultiple(const std::string& input);
    static std::shared_ptr<SExpr> parse(const std::string& input);
    
private:
    class Tokenizer {
    public:
        explicit Tokenizer(const std::string& input);
        
        bool hasNext() const;
        std::string next();
        std::string peek() const;
        void consume();
        
    private:
        std::string input;
        size_t position;
        
        void skipWhitespace();
        bool isDelimiter(char c) const;
    };
    
    static std::shared_ptr<SExpr> parseExpression(Tokenizer& tokenizer);
    static std::shared_ptr<SExpr> parseList(Tokenizer& tokenizer);
};

// Triple representation for structured data
struct SExprTriple {
    std::string subject;
    std::string predicate;
    std::string object;
    
    static std::optional<SExprTriple> fromSExpr(const std::shared_ptr<SExpr>& expr);
};

// Meta-expression representation
struct MetaExpr {
    std::string id;
    std::string type;
    std::string property;
    std::string value;
    
    static std::optional<MetaExpr> fromSExpr(const std::shared_ptr<SExpr>& expr);
};

// Pattern matcher for S-expressions
class SExprMatcher {
public:
    // Match against a pattern like (triple ? type rexist)
    static bool matches(const std::shared_ptr<SExpr>& expr, const std::vector<std::string>& pattern);
    
    // Extract values matching wildcards
    static std::vector<std::string> extract(const std::shared_ptr<SExpr>& expr, 
                                           const std::vector<std::string>& pattern);
    
    // Find all expressions matching a pattern in a list
    static std::vector<std::shared_ptr<SExpr>> findAll(
        const std::vector<std::shared_ptr<SExpr>>& exprs,
        const std::vector<std::string>& pattern);
};

}

#endif