/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKCorePanic_h
#define NKCorePanic_h

#include "NK/Types/Basics.h"
#include "NK/Support.h"

/**
 * @brief This function will immediately halt the program execution because the
 * code has entered an inconsistent state. This happens at failures like invalid
 * memory block access or invalid `switch` conditions.
 */
void
NK_Panic(
    const NK_C8* format,
    ...
);

#endif