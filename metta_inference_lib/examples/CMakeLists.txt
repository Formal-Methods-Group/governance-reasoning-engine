add_executable(example_basic example_basic.cpp)
target_link_libraries(example_basic PRIVATE metta_inference_core)

add_executable(test_knowledge_io test_knowledge_io.cpp)
target_link_libraries(test_knowledge_io PRIVATE metta_inference_core)

add_executable(test_soa_validation test_soa_validation.cpp)
target_link_libraries(test_soa_validation PRIVATE metta_inference_core)

add_executable(test_v2_improvements test_v2_improvements.cpp)
target_link_libraries(test_v2_improvements PRIVATE metta_inference_core)

if(BUILD_API)
    add_executable(example_api example_api.cpp)
    target_link_libraries(example_api PRIVATE metta_inference_api)
endif()