/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKInternalsSubsystemsGlobalStringPool_h
#define NKInternalsSubsystemsGlobalStringPool_h

#include "NK/Internals/Types/StringPool.h"
extern NK_StringPool NK_GlobalStringPool;

/**
 * @brief This will construct the subsystem.
 */
void
NK_GlobalStringPoolSubsystemConstruct();

/**
 * @brief This will destruct the subsystem.
 */
void
NK_GlobalStringPoolSubsystemDestruct();

#endif