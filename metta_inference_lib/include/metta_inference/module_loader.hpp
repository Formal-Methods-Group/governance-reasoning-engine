#ifndef METTA_INFERENCE_MODULE_LOADER_HPP
#define METTA_INFERENCE_MODULE_LOADER_HPP

#include <filesystem>
#include <vector>
#include <string>

namespace metta_inference {

namespace fs = std::filesystem;

class ModuleLoader {
public:
    struct ModuleInfo {
        fs::path path;
        std::vector<fs::path> files;
        size_t totalSize = 0;
    };

    static std::vector<fs::path> scanMettaFiles(const fs::path& directory);
    static ModuleInfo analyzeModule(const fs::path& directory);
    static fs::path createCombinedFile(
        const std::vector<fs::path>& modulePaths,
        const fs::path& exampleFile,
        bool verbose = false
    );
    static std::vector<ModuleInfo> validateModules(const std::vector<fs::path>& modulePaths);
};

}

#endif