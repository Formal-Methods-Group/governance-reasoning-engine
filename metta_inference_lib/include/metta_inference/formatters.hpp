#ifndef METTA_INFERENCE_FORMATTERS_HPP
#define METTA_INFERENCE_FORMATTERS_HPP

#include "config.hpp"
#include <string>
#include <memory>

namespace metta_inference {

class ResultFormatter {
public:
    virtual ~ResultFormatter() = default;
    virtual std::string format(const Config& config, const Metrics& metrics, 
                               const std::string& output, const std::string& exampleName) const = 0;
    virtual std::string getExtension() const = 0;
};

class PrettyFormatter : public ResultFormatter {
public:
    std::string format(const Config& config, const Metrics& metrics, 
                      const std::string& output, const std::string& exampleName) const override;
    std::string getExtension() const override { return ".txt"; }
    
private:
    std::string getCurrentTimestamp() const;
    std::string formatProcessingStatus() const;
    std::string formatResultsSummary(const Metrics& metrics) const;
    std::string formatDetailedFindings(const std::string& output, const Metrics& metrics) const;
    std::string formatOverallAssessment(const Metrics& metrics) const;
};

class JSONFormatter : public ResultFormatter {
public:
    std::string format(const Config& config, const Metrics& metrics, 
                      const std::string& output, const std::string& exampleName) const override;
    std::string getExtension() const override { return ".json"; }
};

class CSVFormatter : public ResultFormatter {
public:
    std::string format(const Config& config, const Metrics& metrics, 
                      const std::string& output, const std::string& exampleName) const override;
    std::string getExtension() const override { return ".csv"; }
};

class MarkdownFormatter : public ResultFormatter {
public:
    std::string format(const Config& config, const Metrics& metrics, 
                      const std::string& output, const std::string& exampleName) const override;
    std::string getExtension() const override { return ".md"; }
    
private:
    std::string formatDetailedResults(const Metrics& metrics) const;
    std::string formatInterpretation(const Metrics& metrics) const;
};

class FormatterFactory {
public:
    static std::unique_ptr<ResultFormatter> create(OutputFormat format);
};

}

#endif