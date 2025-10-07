#include "metta_inference/inference_engine.hpp"

namespace metta_inference {

InferenceEngine::InferenceEngine(const Config& cfg) : config(cfg) {
}

InferenceEngine::Result InferenceEngine::run(const std::filesystem::path&) {
    Result result;
    result.hasLogicalIssues = false;
    return result;
}

}