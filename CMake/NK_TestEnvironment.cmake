# Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
# Project is under GPL3-license, read LICENSE.md on the root.
include(CheckIncludeFile)

function(__nk_check_header test_list)
    foreach(header_f IN LISTS test_list)
        string(MAKE_C_IDENTIFIER "HAS_${header_f}" result_var)
        check_include_file(${header_f} ${result_var})
        if(NOT ${result_var})
            message(FATAL_ERROR "Missing system header: ${header_f}")
        endif()
    endforeach()
endfunction()

# Set the headers:
set(nk_usual_headers
    "stdint.h"
    "stdio.h"
    "memory.h"
    "string.h"
    "stdlib.h"
)
cmake_language(CALL __nk_check_header "${nk_usual_headers}")

# Set the symbols used by the project:
include(CheckSymbolExists)

function(nk_check_symbols symbol_list)
    list(LENGTH symbol_list sl_length)
    math(EXPR sl_stop "${sl_length} - 1")

    # Step by 2 to grab (Symbol, Header) pairs
    foreach(sl_index RANGE 0 ${sl_stop} 2)
        list(GET symbol_list ${sl_index} func_name)
        math(EXPR next_index "${sl_index} + 1")
        list(GET symbol_list ${next_index} header_name)
        string(MAKE_C_IDENTIFIER "HAS_SYMBOL_${func_name}" result_var)
        check_symbol_exists(${func_name} ${header_name} ${result_var})
        if(NOT ${result_var})
            message(FATAL_ERROR "Missing required symbol: ${func_name} in ${header_name}")
        endif()
    endforeach()
endfunction()

# Define your pairs
set(nk_required_symbols
    # I/O functions:
    "fopen" "stdio.h"
    "fclose" "stdio.h"
    "fseek" "stdio.h"
    "ftell" "stdio.h"
    "fgetc" "stdio.h"
    "sprintf" "stdio.h"
    "printf" "stdio.h"
    
    # Allocating Memories:
    "malloc" "stdlib.h"
    "calloc" "stdlib.h"
    "realloc" "stdlib.h"
    "free" "stdlib.h"

    # Copy/Set functions:
    "memcpy" "string.h"
    "memset" "string.h"

    # Strings:
    "strlen" "string.h"
    "strcpy" "string.h"
    "strcmp" "string.h"
)
nk_check_symbols("${nk_required_symbols}")