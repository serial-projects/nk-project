# Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
# Project is under GPL3-license, read LICENSE.md on the root.

# TestGenerator.cmake:                      Contains some utils to generate 
#                                           test for the targets.
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