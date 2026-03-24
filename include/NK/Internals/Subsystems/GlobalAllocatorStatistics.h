/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKInternalsSubsystemsGlobalAllocatorStatistics_h
#define NKInternalsSubsystemsGlobalAllocatorStatistics_h

#include "NK/Memory/AllocatorStatistics.h"
#include "NK/Types/Basics.h"

/** NOTE: This must be initialized on a subsystem: */
extern NK_AllocatorStatistics NK_GlobalAllocatorStatistics;

/**
 * @brief This will construct the subsystem.
 */
void
NK_GlobalAllocatorStatisticsSubsystemConstruct();

/**
 * @brief This will destruct the subsystem.
 */
void
NK_GlobalAllocatorStatisticsSubsystemDestruct();

#endif