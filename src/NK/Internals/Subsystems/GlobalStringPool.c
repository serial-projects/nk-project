/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Internals/Subsystems/GlobalStringPool.h"

/** NOTE: The global is defined here: */
NK_StringPool NK_GlobalStringPool;

void
NK_GlobalStringPoolSubsystemConstruct()
{
    NK_StringPoolConstruct(&NK_GlobalStringPool);
}

void
NK_GlobalStringPoolSubsystemDestruct()
{
    NK_StringPoolDestruct(&NK_GlobalStringPool);
}