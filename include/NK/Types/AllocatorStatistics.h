/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#ifndef NKTypesAllocatorStatistics_h
#define NKTypesAllocatorStatistics_h

#include "NK/Types/Basics.h"

/**
 * @brief This is a information to ALL allocated pointers. It contains some
 * information you can get to debug/profile code.
 */
typedef struct NK_AllocatorStatistics
{
    /**
     * @brief Contains the relative size of the allocations (doesn't count the
     * payload on the beginning).
     */
    NK_Size relative_size;

    /**
     * @brief Contains the absolute size of the allocations.
     */
    NK_Size absolute_size;

    /**
     * @brief Contains the highest memory ever usage.
     */
    NK_Size absolute_max;

    /**
     * @brief Current allocations that are not fred.
     */
    NK_U64  online_blocks;
} NK_AllocatorStatistics;

#endif