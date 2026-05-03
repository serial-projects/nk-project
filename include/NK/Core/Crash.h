/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKCoreCrash_h
#define NKCoreCrash_h

#include "NK/Core/Panic.h"

/**
 * @brief This will panic the program because of an unimplemented function.
 */
#define NK_CrashBecauseNotImplementedFunction() \
    NK_Panic("%s: Not implemented function!", NK_CURRENT_WHERE)

#endif