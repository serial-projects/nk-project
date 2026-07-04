# Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
# Project is under GPL3-license, read LICENSE.md on the root.
# 

cmake_minimum_required(VERSION 3.20)

project(NK VERSION 1.0.000)
set(NK_VENDOR_NAME "Ruka Raposa")
option(NK_ENABLE_ASAN "If ASAN should be used on NK" OFF)
option(NK_ENABLE_TESTS "If tests should be considered" OFF)
option(NK_STATIC "Enable static NK" ON)
option(NK_SHARED "Enable shared NK" OFF)

# NOTE: `NK_ENABLE_TESTS` need (depends on `NK_STATIC`) so, force it to enable.
if(NK_ENABLE_TESTS AND NOT NK_STATIC)
    message(WARNING "Enabling NK_STATIC because NK_ENABLE_TESTS requires!")
    set(NK_STATIC ON)
endif()

add_library(NK_Headers INTERFACE)
target_include_directories(NK_Headers INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

#
# NK_Setup:
#   + Contains `ansi` property;
#   + Contains `fsanitizer` property.
#
add_library(NK_Setup INTERFACE)
# set_target_properties(NK_Setup PROPERTIES C_STANDARD 90)
# set_target_properties(NK_Setup PROPERTIES C_STANDARD_REQUIRED ON)
# set_target_properties(NK_Setup PROPERTIES C_EXTENSIONS OFF)

if(MSVC)
    target_compile_options(NK_Setup INTERFACE /Za)
else()
    # Forces C90, turns off GNU extensions, and turns on pedantic warnings
    target_compile_options(NK_Setup INTERFACE -std=c90 -Wpedantic -pedantic-errors)
endif()

if(NK_ENABLE_ASAN)
    if(MSVC)
        target_compile_options(NK_Setup INTERFACE
            $<$<CONFIG:Debug>:/fsanitize=address /Zi>
        )
        target_link_options(NK_Setup INTERFACE
            $<$<CONFIG:Debug>:/incremental:no>
        )
    else()
        target_compile_options(NK_Setup INTERFACE
            $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer>
        )
        target_link_options(NK_Setup INTERFACE
            $<$<CONFIG:Debug>:-fsanitize=address>
        )
    endif()
endif()


# NOTE: By `NK` and `RR` Standards, it is guaranteed that `*.c` in src/ will
# be always source files.
file(GLOB_RECURSE nk_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

if(NK_STATIC)
    add_library(NK_Static STATIC ${nk_sources})
    target_link_libraries(NK_Static PUBLIC NK_Headers)
    target_link_libraries(NK_Static PRIVATE NK_Setup)

    # Set this to the namespace:
    add_library(NK::NK-static ALIAS NK_Static)
endif()

if(NK_SHARED)
    add_library(NK_Shared SHARED ${nk_sources})
    target_link_libraries(NK_Shared PUBLIC NK_Headers)
    target_link_libraries(NK_Shared PRIVATE NK_Setup)

    # Set this to the namespace:
    add_library(NK::NK-shared ALIAS NK_Shared)
endif()

# Debug Some Messages:
message(STATUS "[[ NK \"${NK_VENDOR_NAME}\" - Version: ${PROJECT_VERSION} ]]")
message(STATUS "Enable Static:  ${NK_STATIC}")
message(STATUS "Enable Shared:  ${NK_SHARED}")
message(STATUS "Enable Tests:   ${NK_ENABLE_TESTS}")
message(STATUS "Enable ASan:    ${NK_ENABLE_ASAN}")
message(STATUS "[[ NK \"${NK_VENDOR_NAME}\" - Version: ${PROJECT_VERSION} ]]")

function(NK_GenerateTests tests link_with)
    list(LENGTH tests t_length)
    math(EXPR t_stop "${t_length} - 1")

    foreach(t_index RANGE 0 ${t_stop} 2)
        # We expect everything to be a pair: [test_path, test_name]
        list(GET tests ${t_index} t_path)
        math(EXPR t_index_next "${t_index} + 1")
        list(GET tests ${t_index_next} t_name)

        # We define the executable here:
        add_executable(${t_name} ${t_path})
        target_link_libraries(${t_name} PRIVATE ${link_with})

        # Add the test:
        add_test(NAME ${t_name} COMMAND ${t_name})
    endforeach()
endfunction()

# Build the tests:
if(NK_ENABLE_TESTS)
    # Define an list of tests we want to include:
    enable_testing()

    set(rr_tests
        # NK_InitTest:
        #   + Test if the NK_Init() & NK_Quit() functions work.
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKInit.c"
        "NK_InitTest"
        
        # NK_DynamicStringsTest:
        #   + Test if Dynamic Strings functions are functional.
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKDynamicStrings.c"
        "NK_DynamicStringsTest"

        # NK_MapInsertionTest: 
        #   + Map Insertion Test (to test memory load + sanity).
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKMapInsertion.c"
        "NK_MapInsertionTest"

        # NK_MapIsStringDeletingTest:
        #   + This is to test if the `StringPool` global is deleting strings.
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKMapIsStringDeleting.c"
        "NK_MapIsStringDeletingTest"

        # NK_SubmergedStringTest:
        #   + Test if the Submerged String system is functional.
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKSubmergedStrings.c"
        "NK_SubmergedStringTest"

        # NK_TableInsertionTest:
        #   + Test for the Internal resource `NK_Table` is functional.
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKTableInsertion.c"
        "NK_TableInsertionTest"

        # NK_ValidatorTest: 
        #   + Test if the validator is functional;
        #   + Test for level system;
        #   + Test for callback;
        #   + Do memory stress test.
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKValidator.c" "NK_ValidatorTest"

        # NK_SetTest:
        #   + Test `NK_Set` sanity.
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKSet.c" "NK_SetTest"

        # NK_ArgumentParser:
        #   + Test for 'NK_ArgumentParser` is sane.
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Base/NKArgumentParser.c" "NK_ArgumentParser"
    )

    # Add an interface for the tests:
    add_library(nk_test_i INTERFACE)
    target_link_libraries(nk_test_i INTERFACE NK::NK-static)

    # Set the tests to be compiled.
    cmake_language(CALL NK_GenerateTests "${rr_tests}" nk_test_i)
endif()