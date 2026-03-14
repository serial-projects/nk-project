# Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
# Project is under GPL3-license, read LICENSE.md on the root.

# ANSIC.cmake:                          Set an cmake support for the ANSI C
#                                       compilations as an INTERFACE to make
#                                       it "enable(able)" on only some projects.
add_library(NK_ANSICDemandInterface INTERFACE)
set_target_properties(NK_ANSICDemandInterface PROPERTIES
    C_STANDARD 90
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)