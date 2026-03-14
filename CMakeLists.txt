# Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
# Project is under GPL3-license, read LICENSE.md on the root.

cmake_minimum_required(VERSION 3.20)
project(
    NK
    VERSION 1.0
)

# Options of NK:
option(NK_ENABLE_ASAN "If ASAN should be used on NK" OFF)
option(NK_ENABLE_TESTS "If tests should be considered" OFF)

# Set the path for the NK CMake extensions:
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
include(NK_ANSICDemand)
include(NK_ASanSupport)
include(NK_GenerateTests)

# Defines the `NK` target here:
file(GLOB_RECURSE nk_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_library(NK STATIC ${nk_sources})
target_include_directories(NK PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Include support libraries:
target_link_libraries(NK PRIVATE NK_ASanSupportInterface)
target_link_libraries(NK PRIVATE NK_ANSICDemandInterface)

# Generate some tests:
if(NK_ENABLE_TESTS)
    # Define an list of tests we want to include:
    message(STATUS "Enabling NK tests...")

    set(rr_tests
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/NKInit.c" "NK_InitTest"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/NKMapInsertion.c" "NK_MapInsertionTest"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/NKMapIsStringDeleting.c" "NK_MapIsStringDeletingTest"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/NKSubmergedStrings.c" "NKSubmergedStringTest"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/NKTableInsertion.c" "NKTableInsertionTest"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/NKValidator.c" "NK_ValidatorTest"
    )

    # Add an interface for the tests:
    add_library(nk_test_i INTERFACE)
    target_link_libraries(nk_test_i INTERFACE NK)
    target_link_libraries(nk_test_i INTERFACE NK_ASanSupportInterface)
    target_link_libraries(nk_test_i INTERFACE NK_ANSICDemandInterface)

    # Set the tests to be compiled.
    cmake_language(CALL NK_GenerateTests "${rr_tests}" nk_test_i)
endif()