/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Internals/Subsystems/GlobalStringPool.h"
#include "NK/Internals/Subsystems/GlobalAllocatorStatistics.h"
#include "NK/Internals/Subsystems.h"
#include "NK/Support.h"

void
NK_SubsystemsConstruct()
{
    NK_GlobalAllocatorStatisticsSubsystemConstruct();
    NK_GlobalStringPoolSubsystemConstruct();
}

void
NK_SubsystemsDestruct()
{
    NK_GlobalStringPoolSubsystemDestruct();
    NK_GlobalAllocatorStatisticsSubsystemDestruct();
}