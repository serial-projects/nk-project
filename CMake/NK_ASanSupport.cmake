# Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
# Project is under GPL3-license, read LICENSE.md on the root.

# ASanSupport.cmake:                    An interface for the ASan builds during
#                                       development. Pretty useful for `NK`
#                                       or anything that deals with memory 
#                                       allocation.
add_library(NK_ASanSupportInterface INTERFACE)
if(NK_ENABLE_ASAN)
    message(STATUS "NK will include Address Sanitizer!")
    if(MSVC)
        target_compile_options(NK_ASanSupportInterface INTERFACE
            $<$<CONFIG:Debug>:/fsanitize=address /Zi>
        )
        target_link_options(NK_ASanSupportInterface INTERFACE
            $<$<CONFIG:Debug>:/incremental:no>
        )
    else()
        target_compile_options(NK_ASanSupportInterface INTERFACE
            $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer>
        )
        target_link_options(NK_ASanSupportInterface INTERFACE
            $<$<CONFIG:Debug>:-fsanitize=address>
        )
    endif()
endif()