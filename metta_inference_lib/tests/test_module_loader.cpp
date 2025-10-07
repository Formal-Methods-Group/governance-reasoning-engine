#include "metta_inference/module_loader.hpp"
#include <iostream>
#include <cassert>
#include <fstream>
#include <filesystem>

namespace mi = metta_inference;
namespace fs = std::filesystem;

void testScanMettaFiles() {
    // Create temporary test directory
    fs::path testDir = fs::temp_directory_path() / "metta_test_scan";
    fs::create_directories(testDir);
    
    // Create test files
    std::ofstream(testDir / "test1.metta") << "test content 1";
    std::ofstream(testDir / "test2.metta") << "test content 2";
    std::ofstream(testDir / "not_metta.txt") << "should not be found";
    
    auto files = mi::ModuleLoader::scanMettaFiles(testDir);
    
    assert(files.size() == 2);
    assert(files[0].filename() == "test1.metta");
    assert(files[1].filename() == "test2.metta");
    
    // Cleanup
    fs::remove_all(testDir);
    
    std::cout << "✓ Scan MeTTa files test passed\n";
}

void testAnalyzeModule() {
    // Create temporary test directory
    fs::path testDir = fs::temp_directory_path() / "metta_test_analyze";
    fs::create_directories(testDir);
    
    // Create test files with known content
    std::ofstream file1(testDir / "test1.metta");
    file1 << "test content";
    file1.close();
    
    std::ofstream file2(testDir / "test2.metta");
    file2 << "more test content";
    file2.close();
    
    auto info = mi::ModuleLoader::analyzeModule(testDir);
    
    assert(info.path == testDir);
    assert(info.files.size() == 2);
    assert(info.totalSize > 0);
    
    // Cleanup
    fs::remove_all(testDir);
    
    std::cout << "✓ Analyze module test passed\n";
}

void testNonExistentDirectory() {
    fs::path nonExistent = "/this/does/not/exist/at/all";
    
    auto files = mi::ModuleLoader::scanMettaFiles(nonExistent);
    assert(files.empty());
    
    auto info = mi::ModuleLoader::analyzeModule(nonExistent);
    assert(info.files.empty());
    assert(info.totalSize == 0);
    
    std::cout << "✓ Non-existent directory test passed\n";
}

void testEmptyDirectory() {
    fs::path emptyDir = fs::temp_directory_path() / "metta_test_empty";
    fs::create_directories(emptyDir);
    
    auto files = mi::ModuleLoader::scanMettaFiles(emptyDir);
    assert(files.empty());
    
    auto info = mi::ModuleLoader::analyzeModule(emptyDir);
    assert(info.files.empty());
    assert(info.totalSize == 0);
    
    // Cleanup
    fs::remove_all(emptyDir);
    
    std::cout << "✓ Empty directory test passed\n";
}

int main() {
    try {
        std::cout << "Running ModuleLoader tests...\n";
        
        testScanMettaFiles();
        testAnalyzeModule();
        testNonExistentDirectory();
        testEmptyDirectory();
        
        std::cout << "\nAll tests passed! ✅\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}