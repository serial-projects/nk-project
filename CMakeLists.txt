# Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
# Project is under GPL3-license, read LICENSE.md on the root.

cmake_minimum_required(VERSION 3.20)
project(
    NK
    VERSION 1.0.000
)

# Set some configurations:
set(NK_VENDOR_NAME "Ruka Raposa")

# Options of NK:
option(NK_ENABLE_ASAN "If ASAN should be used on NK" OFF)
option(NK_ENABLE_TESTS "If tests should be considered" OFF)
option(NK_STATIC "Enable static NK" OFF)
option(NK_SHARED "Enable shared NK" ON)

# Set the `NK_ENABLE_TESTS`, we need NK_Static for the code.
if(NK_ENABLE_TESTS AND NOT NK_STATIC)
    message(WARNING "Enabling NK_STATIC because NK_ENABLE_TESTS requires!")
    set(NK_STATIC ON)
endif()

# Set the path for the NK CMake extensions:
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
include(NK_ANSICDemand)
include(NK_ASanSupport)
include(NK_GenerateTests)
include(NK_TestEnvironment)

# We define an generic target:
add_library(NK_Headers INTERFACE)
target_include_directories(NK_Headers INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(NK_Headers INTERFACE NK_ASanSupportInterface)
target_link_libraries(NK_Headers INTERFACE NK_ANSICDemandInterface)

# Get the source files:
file(GLOB_RECURSE nk_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# Main Targets: NK_Static/NK_Shared
if(NK_STATIC)
    add_library(NK_Static STATIC ${nk_sources})
    target_link_libraries(NK_Static PUBLIC NK_Headers)
    add_library(NK::NK-static ALIAS NK_Static)
endif()

if(NK_SHARED)
    add_library(NK_Shared SHARED ${nk_sources})
    target_link_libraries(NK_Shared PUBLIC NK_Headers)
    add_library(NK::NK-shared ALIAS NK_Shared)
endif()

message(STATUS "[[ NK \"${NK_VENDOR_NAME}\" - Version: ${PROJECT_VERSION} ]]")
message(STATUS "Enable Static:  ${NK_STATIC}")
message(STATUS "Enable Shared:  ${NK_SHARED}")
message(STATUS "Enable Tests:   ${NK_ENABLE_TESTS}")
message(STATUS "Enable ASan:    ${NK_ENABLE_ASAN}")
message(STATUS "[[ NK \"${NK_VENDOR_NAME}\" - Version: ${PROJECT_VERSION} ]]")

# Tests:
if(NK_ENABLE_TESTS)
    # Define an list of tests we want to include:
    enable_testing()

    set(rr_tests
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKInit.c" "NK_InitTest"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKMapInsertion.c" "NK_MapInsertionTest"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKMapIsStringDeleting.c" "NK_MapIsStringDeletingTest"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKSubmergedStrings.c" "NK_SubmergedStringTest"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKTableInsertion.c" "NK_TableInsertionTest"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKValidator.c" "NK_ValidatorTest"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKSet.c" "NK_SetTest"
    )

    # Add an interface for the tests:
    add_library(nk_test_i INTERFACE)
    target_link_libraries(nk_test_i INTERFACE NK::NK-static)
    target_link_libraries(nk_test_i INTERFACE NK_ASanSupportInterface)
    target_link_libraries(nk_test_i INTERFACE NK_ANSICDemandInterface)

    # Set the tests to be compiled.
    cmake_language(CALL NK_GenerateTests "${rr_tests}" nk_test_i)
endif()