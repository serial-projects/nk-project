/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Internals/Subsystems/GlobalAllocatorStatistics.h"
#include "NK/Redirects.h"

NK_AllocatorStatistics NK_GlobalAllocatorStatistics;

void
NK_GlobalAllocatorStatisticsSubsystemConstruct()
{
    NK_RedirectMemset(
        &NK_GlobalAllocatorStatistics,
        0,
        sizeof(NK_GlobalAllocatorStatistics)
    );
}

void
NK_GlobalAllocatorStatisticsSubsystemDestruct()
{
    /** NOTE: We don't need anything to fix this either. */
    return;
}