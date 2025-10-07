#include "CLI11.hpp"
#include "metta_inference/inference_engine.hpp"
#include "metta_inference/config.hpp"
#include <iostream>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <regex>
#include <memory>

namespace mi = metta_inference;
namespace fs = std::filesystem;

// Forward declare the V2 factory function
namespace metta_inference {
    std::unique_ptr<InferenceEngine> createInferenceEngineV2(const Config& config);
}

namespace Color {
    inline constexpr std::string_view RED = "\033[0;31m";
    inline constexpr std::string_view GREEN = "\033[0;32m";
    inline constexpr std::string_view CYAN = "\033[0;36m";
    inline constexpr std::string_view NC = "\033[0m";
    inline constexpr std::string_view BOLD = "\033[1m";
}

class MettaCLI {
private:
    mi::Config config;

    std::vector<fs::path> parseModulePaths(const std::string& pathsStr) {
        std::vector<fs::path> paths;
        std::istringstream stream(pathsStr);
        std::string path;

        while (std::getline(stream, path, ',')) {
            path.erase(path.begin(), std::find_if(path.begin(), path.end(),
                      [](unsigned char ch) { return !std::isspace(ch); }));
            path.erase(std::find_if(path.rbegin(), path.rend(),
                      [](unsigned char ch) { return !std::isspace(ch); }).base(), path.end());

            if (!path.empty()) {
                paths.emplace_back(path);
            }
        }

        return paths;
    }

    void saveOutput(const std::string& exampleName, const std::string& formattedOutput,
                   const std::string& extension) {
        if (!config.saveOutput) return;

        try {
            fs::create_directories(config.outputDir);

            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            std::ostringstream filename;
            filename << config.outputDir.string() << "/" << exampleName << "_"
                    << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S") << extension;

            std::ofstream file(filename.str());
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open output file: " + filename.str());
            }

            if (config.outputFormat == mi::OutputFormat::Pretty) {
                std::string cleanOutput = formattedOutput;
                std::regex ansiPattern("\033\\[[0-9;]*m");
                cleanOutput = std::regex_replace(cleanOutput, ansiPattern, "");
                file << cleanOutput;
            } else {
                file << formattedOutput;
            }

            file.close();
            std::cout << "\n" << Color::BOLD << "Results saved to:" << Color::NC
                     << " " << filename.str() << "\n";

        } catch (const std::exception& e) {
            std::cerr << Color::RED << "Warning: Failed to save output: "
                     << e.what() << Color::NC << "\n";
        }
    }

public:
    int run(int argc, char* argv[]) {
        CLI::App app{"MeTTa CT Inference Runner - Modular C++ Version"};

        // Add command line options
        app.add_flag("-v,--verbose", config.verbose, "Show detailed processing information");

        std::string formatStr = "pretty";
        app.add_option("-f,--format", formatStr, "Output format")
            ->check(CLI::IsMember({"pretty", "json", "csv", "markdown"}))
            ->default_val("pretty");

        app.add_flag("-s,--save", config.saveOutput, "Save results to file");

        app.add_option("-o,--output-dir", config.outputDir, "Output directory")
            ->default_val("./inference_results");

        app.add_flag("-r,--raw", config.showRaw, "Also show raw MeTTa output");

        std::string configFile;
        app.add_option("-c,--config", configFile, "Path to JSON configuration file (for entity mappings, templates, etc.)");

        std::string modulePaths = "/app/base,/app/knowledge,/app/reason";
        app.add_option("-m,--modules", modulePaths,
            "Module directories (comma-separated)")
            ->default_val(modulePaths);

        app.add_option("-e,--engine", config.mettaReplPath,
            "Path to metta-repl executable")
            ->default_val("/usr/local/bin/metta-repl");

        app.add_option("example", config.exampleFile, "Example MeTTa file to process")
            ->required()
            ->check(CLI::ExistingFile);

        // Set up version and help info
        app.set_version_flag("--version", "2.0.0 (S-Expression Parser)");

        // Custom help formatter
        app.description("A modular inference runner for MeTTa reasoning.\n\n"
                       "MODULE LOADING:\n"
                       "  Modules are loaded in the order specified. Each directory is scanned\n"
                       "  for .metta files which are combined alphabetically before the example.\n"
                       "  Default order: base → knowledge → reason → example");

        // Examples in footer
        app.footer("EXAMPLES:\n"
                  "  metta_cli example1.metta                     # Basic usage with default modules\n"
                  "  metta_cli -v -s example1.metta              # Verbose with saved output\n"
                  "  metta_cli -f json -s example1.metta         # Save as JSON\n"
                  "  metta_cli -m ./base,./knowledge example1.metta  # Custom module paths\n"
                  "  metta_cli -e /path/to/metta-repl example1.metta  # Custom engine path");

        // Parse arguments
        CLI11_PARSE(app, argc, argv);

        // Convert format string to enum
        if (formatStr == "pretty") config.outputFormat = mi::OutputFormat::Pretty;
        else if (formatStr == "json") config.outputFormat = mi::OutputFormat::JSON;
        else if (formatStr == "csv") config.outputFormat = mi::OutputFormat::CSV;
        else if (formatStr == "markdown") config.outputFormat = mi::OutputFormat::Markdown;

        // Parse module paths
        config.modulePaths = parseModulePaths(modulePaths);

        // Load configuration file if specified
        if (!configFile.empty()) {
            fs::path configPath(configFile);
            if (fs::exists(configPath)) {
                if (config.verbose) {
                    std::cout << Color::CYAN << "Loading configuration from: "
                             << configPath << Color::NC << "\n";
                }
                // Configuration will be loaded by the V2 engine
            } else {
                std::cerr << Color::RED << "Warning: Config file not found: "
                         << configPath << Color::NC << "\n";
            }
        }

        // Validate module paths exist
        for (const auto& modulePath : config.modulePaths) {
            if (!fs::exists(modulePath)) {
                std::cerr << Color::RED << "Error: Module directory not found: "
                         << modulePath << Color::NC << "\n";
                return 1;
            }
            if (!fs::is_directory(modulePath)) {
                std::cerr << Color::RED << "Error: Module path is not a directory: "
                         << modulePath << Color::NC << "\n";
                return 1;
            }
        }

        // Validate metta-repl executable
        if (!fs::exists(config.mettaReplPath)) {
            std::cerr << Color::RED << "Error: MeTTa REPL executable not found: "
                     << config.mettaReplPath << Color::NC << "\n"
                     << "Hint: Set METTA_REPL_PATH environment variable or use -e flag\n";
            return 1;
        }

        if (!fs::is_regular_file(config.mettaReplPath)) {
            std::cerr << Color::RED << "Error: MeTTa REPL path is not a file: "
                     << config.mettaReplPath << Color::NC << "\n";
            return 1;
        }

        // Check if file is executable
        std::error_code ec;
        auto perms = fs::status(config.mettaReplPath, ec).permissions();
        if (ec) {
            std::cerr << Color::RED << "Error: Cannot check MeTTa REPL permissions: "
                     << ec.message() << Color::NC << "\n";
            return 1;
        }

        if ((perms & fs::perms::owner_exec) == fs::perms::none &&
            (perms & fs::perms::group_exec) == fs::perms::none &&
            (perms & fs::perms::others_exec) == fs::perms::none) {
            std::cerr << Color::RED << "Error: MeTTa REPL is not executable: "
                     << config.mettaReplPath << Color::NC << "\n";
            return 1;
        }

        // Run the inference
        try {
            auto startTime = std::chrono::steady_clock::now();

            if (config.verbose) {
                std::cout << Color::CYAN << "=== MeTTa CT Modular Inference Runner V2 ===" << Color::NC << "\n";
                std::cout << Color::BOLD << "Engine:" << Color::NC << " " << config.mettaReplPath << "\n";
                std::cout << Color::BOLD << "Example file:" << Color::NC << " " << config.exampleFile << "\n";
                std::cout << Color::BOLD << "Module paths:" << Color::NC << "\n";
                for (size_t i = 0; i < config.modulePaths.size(); ++i) {
                    std::cout << "  " << (i + 1) << ". " << config.modulePaths[i] << "\n";
                }
                auto now = std::chrono::system_clock::now();
                auto time_t = std::chrono::system_clock::to_time_t(now);
                std::cout << Color::BOLD << "Started:" << Color::NC << " "
                         << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "\n\n";
            }

            // Use V2 engine with improved S-Expression parser
            auto engine = mi::createInferenceEngineV2(config);
            auto result = engine->run(config.exampleFile);

            std::cout << result.formattedOutput << "\n";

            // Get extension based on format
            std::string extension;
            switch (config.outputFormat) {
                case mi::OutputFormat::Pretty: extension = ".txt"; break;
                case mi::OutputFormat::JSON: extension = ".json"; break;
                case mi::OutputFormat::CSV: extension = ".csv"; break;
                case mi::OutputFormat::Markdown: extension = ".md"; break;
            }

            saveOutput(config.exampleFile.stem().string(), result.formattedOutput, extension);

            if (config.showRaw) {
                std::cout << "\n" << Color::CYAN << "=== RAW METTA OUTPUT ==="
                         << Color::NC << "\n";
                std::cout << result.rawOutput << "\n";
            }

            if (config.verbose) {
                auto endTime = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

                std::cout << "\n" << Color::BOLD << "Performance:" << Color::NC << "\n";
                std::cout << "  • Processing time: " << duration << "s\n";
                auto now = std::chrono::system_clock::now();
                auto time_t = std::chrono::system_clock::to_time_t(now);
                std::cout << "  • Completed: " << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "\n";
            }

            return result.hasLogicalIssues ? 2 : 0;

        } catch (const std::exception& e) {
            if (config.verbose) {
                std::cout << Color::RED << "✗" << Color::NC << "\n";
            }
            std::cerr << Color::RED << "Error: " << e.what() << Color::NC << "\n";
            return 1;
        }
    }
};

int main(int argc, char* argv[]) {
    try {
        MettaCLI cli;
        return cli.run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << Color::RED << "Fatal error: " << e.what() << Color::NC << "\n";
        return 1;
    } catch (...) {
        std::cerr << Color::RED << "Fatal error: Unknown exception" << Color::NC << "\n";
        return 1;
    }
}
