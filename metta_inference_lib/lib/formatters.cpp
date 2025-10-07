#include "metta_inference/formatters.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>

namespace metta_inference {

namespace Color {
    inline constexpr std::string_view RED = "\033[0;31m";
    inline constexpr std::string_view GREEN = "\033[0;32m";
    inline constexpr std::string_view YELLOW = "\033[1;33m";
    inline constexpr std::string_view BLUE = "\033[0;34m";
    inline constexpr std::string_view PURPLE = "\033[0;35m";
    inline constexpr std::string_view CYAN = "\033[0;36m";
    inline constexpr std::string_view NC = "\033[0m";
    inline constexpr std::string_view BOLD = "\033[1m";
}

std::string PrettyFormatter::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string PrettyFormatter::formatProcessingStatus() const {
    std::ostringstream result;
    result << "  " << Color::BOLD << "Processing Status:" << Color::NC << "\n";
    result << "    File parsing: " << Color::GREEN << "✓" << Color::NC << "\n";
    result << "    Inference engine: " << Color::GREEN << "✓" << Color::NC << "\n";
    result << "    Results extraction: " << Color::GREEN << "✓" << Color::NC << "\n\n";
    return result.str();
}

std::string PrettyFormatter::formatResultsSummary(const Metrics& metrics) const {
    std::ostringstream result;
    result << "  " << Color::BOLD << "Results Summary:" << Color::NC << "\n";

    // Show inferred state of affairs if present
    if (metrics.inferredFacts > 0) {
        result << "    • Inferred facts:       " << Color::GREEN << metrics.inferredFacts
               << " state of affairs" << Color::NC << "\n";
    }

    // Show both raw contradictions and unique pairs
    if (metrics.contradictions > 0) {
        result << "    • Contradictions:       " << Color::RED << metrics.contradictions
               << " inferred" << Color::NC;
        result << " (" << Color::BOLD << metrics.contradictionPairs
               << " unique pairs" << Color::NC << ")";
        result << "\n";
    } else {
        result << "    • Contradictions:       " << Color::GREEN << "0" << Color::NC << "\n";
    }

    result << "    • Compliance relations: " << Color::GREEN << metrics.compliances << Color::NC << "\n";
    result << "    • Conflicts:           " << Color::YELLOW << metrics.conflicts << Color::NC << "\n";
    result << "    • Necessary violations: " << Color::PURPLE << metrics.violations << Color::NC << "\n";
    result << "    • " << Color::BOLD << "Total relationships:  " << metrics.total() << Color::NC << "\n\n";
    return result.str();
}

std::string PrettyFormatter::formatDetailedFindings(const std::string&, const Metrics& metrics) const {
    std::ostringstream result;

    if (metrics.inferredFacts > 0 || metrics.total() > 0) {
        result << "  " << Color::BOLD << "Detailed Findings:" << Color::NC << "\n";
        result << "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

        // Display inferred state of affairs
        if (!metrics.inferredStateOfAffairs.empty()) {
            result << "  " << Color::BOLD << "Inferred State of Affairs:" << Color::NC << "\n";
            for (const auto& fact : metrics.inferredStateOfAffairs) {
                result << "    ✓ " << fact << "\n";
            }
            result << "\n";
        }

        // Display contradiction details
        if (!metrics.contradictionDetails.empty()) {
            result << "  " << Color::RED << "Contradictions Found:" << Color::NC << "\n";
            for (const auto& contradiction : metrics.contradictionDetails) {
                result << "    ❌ " << Color::BOLD << "Contradiction between:" << Color::NC << "\n";
                result << "       • " << contradiction.entity1 << "\n";
                result << "       • " << contradiction.entity2 << "\n";
                result << "       " << Color::CYAN << "→ " << contradiction.description << Color::NC << "\n\n";
            }
        }

        // Display conflict details
        if (!metrics.conflictDetails.empty()) {
            result << "  " << Color::YELLOW << "Conflicts Found:" << Color::NC << "\n";
            for (const auto& conflict : metrics.conflictDetails) {
                result << "    ⚠️  " << Color::BOLD << "Conflict between:" << Color::NC << "\n";
                result << "       • " << conflict.entity1 << "\n";
                result << "       • " << conflict.entity2 << "\n";
                result << "       " << Color::CYAN << "→ " << conflict.description << Color::NC << "\n\n";
            }
        }

        // Display violation details
        if (!metrics.violationDetails.empty()) {
            result << "  " << Color::PURPLE << "Necessary Violations:" << Color::NC << "\n";
            for (const auto& violation : metrics.violationDetails) {
                result << "    ❗ " << Color::BOLD << "Violation:" << Color::NC << "\n";
                result << "       • " << Color::BOLD << "Rule violated:" << Color::NC << " " << violation.violated_rule << "\n";
                result << "       • " << Color::BOLD << "Due to:" << Color::NC << " " << violation.violator << "\n";
                result << "       " << Color::CYAN << "→ " << violation.description << Color::NC << "\n\n";
            }
        }
    }

    return result.str();
}

std::string PrettyFormatter::formatOverallAssessment(const Metrics& metrics) const {
    std::ostringstream result;
    result << "  " << Color::BOLD << "Overall Assessment:" << Color::NC << "\n";
    result << "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

    // Primary assessment based on findings
    if (metrics.inferredFacts > 0 && metrics.contradictions == 0 &&
        metrics.conflicts == 0 && metrics.violations == 0) {
        result << "  " << Color::GREEN << "✅ State of affairs successfully inferred!" << Color::NC << "\n";
        result << "     • " << metrics.inferredFacts << " fact(s) inferred by the engine\n";
        result << "     • No contradictions or conflicts detected\n";
        result << "     • System reasoning is logically sound\n";
    } else if (metrics.contradictions > 0) {
        result << "  " << Color::RED << "❌ Contradictions present!" << Color::NC << "\n";
        result << "     • " << metrics.contradictions << " contradiction inferences found\n";
        if (metrics.contradictionPairs > 0) {
            result << "     • Forming " << metrics.contradictionPairs << " unique contradiction pair(s)\n";
        }
        result << "     • System contains mutually exclusive statements\n";
    } else if (metrics.conflicts > 0 || metrics.violations > 0) {
        result << "  " << Color::YELLOW << "⚠️  Logical issues detected!" << Color::NC << "\n";
        result << "     • Found " << metrics.conflicts << " conflict(s) and "
              << metrics.violations << " necessary violation(s)\n";
        result << "     • Review constraints to resolve inconsistencies\n";
        if (metrics.violations > 0) {
            result << "     • Some rules must be violated - system is over-constrained\n";
        }
    } else if (metrics.compliances > 0) {
        result << "  " << Color::GREEN << "✅ Positive compliance relationships found." << Color::NC << "\n";
        result << "     • Some obligations are being properly fulfilled\n";
        result << "     • System shows partial correctness\n";
    } else {
        result << "  " << Color::BLUE << "ℹ️  No inference relationships found." << Color::NC << "\n";
        result << "     • The example appears to be logically consistent\n";
        result << "     • No rule conflicts or violations detected\n";
    }

    return result.str();
}

std::string PrettyFormatter::format(const Config& config, const Metrics& metrics,
                                    const std::string& output, const std::string& exampleName) const {
    std::ostringstream result;

    result << "  " << Color::CYAN << "╔════════════════════════════════════════════╗" << Color::NC << "\n";
    result << "  " << Color::CYAN << "║          SKY Governance Inference          ║" << Color::NC << "\n";
    result << "  " << Color::CYAN << "╚════════════════════════════════════════════╝" << Color::NC << "\n\n";

    result << "  " << Color::BOLD << "Example:" << Color::NC << " " << exampleName << "\n";
    result << "  " << Color::BOLD << "Timestamp:" << Color::NC << " " << getCurrentTimestamp() << "\n\n";

    if (config.verbose) {
        result << formatProcessingStatus();
    }

    result << formatResultsSummary(metrics);

    if (metrics.total() > 0) {
        result << formatDetailedFindings(output, metrics);
    }

    result << formatOverallAssessment(metrics);

    return result.str();
}

std::string JSONFormatter::format(const Config&, const Metrics& metrics,
                                  const std::string&, const std::string& exampleName) const {
    std::ostringstream json;
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    json << "{\n";
    json << "  \"example\": \"" << exampleName << "\",\n";
    json << "  \"timestamp\": \"" << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%SZ") << "\",\n";
    json << "  \"results\": {\n";
    json << "    \"contradictions\": " << metrics.contradictions << ",\n";
    json << "    \"compliances\": " << metrics.compliances << ",\n";
    json << "    \"conflicts\": " << metrics.conflicts << ",\n";
    json << "    \"necessary_violations\": " << metrics.violations << ",\n";
    json << "    \"total\": " << metrics.total() << "\n";
    json << "  },\n";
    json << "  \"interpretation\": {\n";
    json << "    \"has_logical_issues\": " << (metrics.conflicts > 0 || metrics.violations > 0 ? "true" : "false") << ",\n";
    json << "    \"is_consistent\": " << (metrics.contradictions == 0 ? "true" : "false") << ",\n";
    json << "    \"has_fulfilled_obligations\": " << (metrics.compliances > 0 ? "true" : "false") << "\n";
    json << "  }\n";
    json << "}";

    return json.str();
}

std::string CSVFormatter::format(const Config&, const Metrics& metrics,
                                 const std::string&, const std::string& exampleName) const {
    std::ostringstream csv;
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    csv << "Example,Timestamp,Contradictions,Compliances,Conflicts,Violations,Total\n";
    csv << exampleName << "," << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%SZ") << ",";
    csv << metrics.contradictions << "," << metrics.compliances << ",";
    csv << metrics.conflicts << "," << metrics.violations << "," << metrics.total();

    return csv.str();
}


std::string MarkdownFormatter::formatDetailedResults(const Metrics& metrics) const {
    std::ostringstream md;
    md << "## Detailed Results\n\n";
    
    // Display inferred state of affairs
    if (!metrics.inferredStateOfAffairs.empty()) {
        md << "### Inferred State of Affairs\n";
        for (const auto& fact : metrics.inferredStateOfAffairs) {
            md << "- " << fact << "\n";
        }
        md << "\n";
    }
    
    // Display contradiction details
    if (!metrics.contradictionDetails.empty()) {
        md << "### Contradictions Found\n";
        for (const auto& contradiction : metrics.contradictionDetails) {
            md << "- **Between:** " << contradiction.entity1 << " and " << contradiction.entity2 << "\n";
            md << "  - " << contradiction.description << "\n";
        }
        md << "\n";
    }
    
    // Display conflict details
    if (!metrics.conflictDetails.empty()) {
        md << "### Conflicts Found\n";
        for (const auto& conflict : metrics.conflictDetails) {
            md << "- **Between:** " << conflict.entity1 << " and " << conflict.entity2 << "\n";
            md << "  - " << conflict.description << "\n";
        }
        md << "\n";
    }
    
    // Display violation details
    if (!metrics.violationDetails.empty()) {
        md << "### Necessary Violations\n";
        for (const auto& violation : metrics.violationDetails) {
            md << "- **Rule violated:** " << violation.violated_rule << "\n";
            md << "  - **By:** " << violation.violator << "\n";
            md << "  - " << violation.description << "\n";
        }
        md << "\n";
    }
    
    return md.str();
}

std::string MarkdownFormatter::formatInterpretation(const Metrics& metrics) const {
    std::ostringstream md;
    md << "## Interpretation\n\n";

    if (metrics.conflicts > 0 || metrics.violations > 0) {
        md << "⚠️ **Logical issues detected!** Found " << metrics.conflicts
           << " conflict(s) and " << metrics.violations << " necessary violation(s).\n\n";
    } else if (metrics.compliances > 0) {
        md << "✅ **Positive compliance** - Some obligations are being properly fulfilled.\n\n";
    } else {
        md << "ℹ️ **No significant relationships found** - The system appears consistent.\n\n";
    }

    if (metrics.contradictions > 0) {
        md << "❌ **Contradictions present!** Direct logical inconsistencies found in the system.\n\n";
    }

    return md.str();
}

std::string MarkdownFormatter::format(const Config&, const Metrics& metrics,
                                      const std::string&, const std::string& exampleName) const {
    std::ostringstream md;
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    md << "# MeTTa Inference Results: " << exampleName << "\n\n";
    md << "**Generated:** " << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "\n\n";
    md << "## Summary\n\n";
    md << "| Metric | Count | Status |\n";
    md << "|--------|-------|---------|\n";
    md << "| Contradictions | " << metrics.contradictions << " | "
       << (metrics.contradictions == 0 ? "✅" : "❌") << " |\n";
    md << "| Compliance Relations | " << metrics.compliances << " | "
       << (metrics.compliances > 0 ? "✅" : "⚪") << " |\n";
    md << "| Conflicts | " << metrics.conflicts << " | "
       << (metrics.conflicts == 0 ? "✅" : "⚠️") << " |\n";
    md << "| Necessary Violations | " << metrics.violations << " | "
       << (metrics.violations == 0 ? "✅" : "⚠️") << " |\n";
    md << "| **Total** | **" << metrics.total() << "** | - |\n\n";

    md << formatDetailedResults(metrics);
    md << formatInterpretation(metrics);

    return md.str();
}

std::unique_ptr<ResultFormatter> FormatterFactory::create(OutputFormat format) {
    switch (format) {
        case OutputFormat::Pretty:
            return std::make_unique<PrettyFormatter>();
        case OutputFormat::JSON:
            return std::make_unique<JSONFormatter>();
        case OutputFormat::CSV:
            return std::make_unique<CSVFormatter>();
        case OutputFormat::Markdown:
            return std::make_unique<MarkdownFormatter>();
        default:
            return std::make_unique<PrettyFormatter>();
    }
}

}
