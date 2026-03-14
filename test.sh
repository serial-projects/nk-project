#!/bin/sh

# Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
# Project is under GPL3-license, read LICENSE.md on the root.

NK_TestBuildFolder(){
    if [[ -d './Build' ]]; then
        return
    else
        mkdir ./Build
    fi
}

NK_RunCMAKEToConstructDebugVersion(){
    # This will enter the ./Build directory, make sure it is there:
    cd ./Build
        cmake                                       \
            -G Ninja                                \
            ..                                      \
            -DCMAKE_BUILD_TYPE="Debug"              \
            -DNK_ENABLE_ASAN=true                   \
            -DNK_ENABLE_TESTS=true
            # --debug-find                            \
            # --trace-expand
        [[ $? -eq 0 ]] && cmake --build . -v -j10
        [[ $? -eq 0 ]] && ctest . --extra-verbose --stop-on-failure --progress
    cd ..
}

NK_Main(){
    NK_TestBuildFolder
    NK_RunCMAKEToConstructDebugVersion
}
NK_Main;