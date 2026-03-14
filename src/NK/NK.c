/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/NK.h"
#include "NK/Internals/Subsystems.h"

/**
 * @brief Set the initialized the NK.
 */
NK_Boolean NK_Initialized = false;

void
NK_Init()
{
    NK_SubsystemsConstruct();
    NK_Initialized = true;
}

void
NK_Quit()
{
    NK_SubsystemsDestruct();
    NK_Initialized = false;
}

const NK_Result
NK_IsInitialized()
{
    return NK_Initialized;
}