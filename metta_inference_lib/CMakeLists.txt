cmake_minimum_required(VERSION 3.14)
project(metta_inference VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_CLI "Build CLI executable" ON)
option(BUILD_API "Build API library" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(ENABLE_ASAN "Enable Address Sanitizer in Debug builds" OFF)
option(ENABLE_LTO "Enable Link Time Optimization" OFF)
option(ENABLE_NATIVE_ARCH "Enable native architecture optimizations" OFF)

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -pedantic)
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
        if(ENABLE_ASAN)
            add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
            add_link_options(-fsanitize=address)
        endif()
    else()
        add_compile_options(-O3)
        
        # Enable native architecture optimizations if requested
        if(ENABLE_NATIVE_ARCH)
            add_compile_options(-march=native)
        endif()
        
        # Enable Link Time Optimization if requested
        if(ENABLE_LTO)
            add_compile_options(-flto)
            add_link_options(-flto)
        endif()
    endif()
elseif(MSVC)
    add_compile_options(/W4)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/O2)
        if(ENABLE_LTO)
            add_compile_options(/GL)
            add_link_options(/LTCG)
        endif()
    endif()
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Core library sources
set(LIB_SOURCES
    lib/process_executor.cpp
    lib/module_loader.cpp
    lib/formatters.cpp
    lib/knowledge_io.cpp
    lib/sexpr_parser.cpp
    lib/entity_resolver.cpp
    lib/semantic_analyzer.cpp
    lib/inference_engine_base.cpp
    lib/inference_engine_v2.cpp
)

# Create core library
add_library(metta_inference_core ${LIB_SOURCES})
target_include_directories(metta_inference_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(metta_inference_core PUBLIC pthread)

# API library
if(BUILD_API)
    add_library(metta_inference_api api/metta_api.cpp)
    target_link_libraries(metta_inference_api PUBLIC metta_inference_core)
    target_include_directories(metta_inference_api PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>
        $<INSTALL_INTERFACE:include/api>
    )
endif()

# CLI executable
if(BUILD_CLI)
    # Download CLI11 if not present
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cli/CLI11.hpp")
        message(STATUS "Downloading CLI11...")
        file(DOWNLOAD 
            "https://raw.githubusercontent.com/CLIUtils/CLI11/v2.3.2/include/CLI/CLI11.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/cli/CLI11.hpp"
            SHOW_PROGRESS
        )
    endif()
    
    add_executable(metta_cli cli/metta_cli.cpp)
    target_include_directories(metta_cli PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cli)
    target_link_libraries(metta_cli PRIVATE metta_inference_core)
    
    add_executable(metta_knowledge_cli cli/metta_knowledge_cli.cpp)
    target_include_directories(metta_knowledge_cli PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cli)
    target_link_libraries(metta_knowledge_cli PRIVATE metta_inference_core)
endif()

# Examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation
install(TARGETS metta_inference_core
    EXPORT metta_inference_targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/metta_inference
    DESTINATION include
)

if(BUILD_API)
    install(TARGETS metta_inference_api
        EXPORT metta_inference_targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
    install(FILES api/metta_api.hpp
        DESTINATION include/metta_inference
    )
endif()

if(BUILD_CLI)
    install(TARGETS metta_cli metta_knowledge_cli
        RUNTIME DESTINATION bin
    )
endif()

# Export package
install(EXPORT metta_inference_targets
    FILE metta_inference-config.cmake
    NAMESPACE metta_inference::
    DESTINATION lib/cmake/metta_inference
)

# Create package config version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/metta_inference-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/metta_inference-config-version.cmake"
    DESTINATION lib/cmake/metta_inference
)

# Print configuration summary
message(STATUS "====================================")
message(STATUS "MeTTa Inference Library Configuration:")
message(STATUS "  Version:           ${PROJECT_VERSION}")
message(STATUS "  Build type:        ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Compiler:      ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Build CLI:         ${BUILD_CLI}")
message(STATUS "  Build API:         ${BUILD_API}")
message(STATUS "  Build tests:       ${BUILD_TESTS}")
message(STATUS "  Build examples:    ${BUILD_EXAMPLES}")
message(STATUS "  Enable ASAN:       ${ENABLE_ASAN}")
message(STATUS "  Enable LTO:        ${ENABLE_LTO}")
message(STATUS "  Native arch opts:  ${ENABLE_NATIVE_ARCH}")
message(STATUS "  Install prefix:    ${CMAKE_INSTALL_PREFIX}")
message(STATUS "=====================================")