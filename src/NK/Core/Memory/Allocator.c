/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Internals/Subsystems/GlobalAllocatorStatistics.h"
#include "NK/Core/Memory/Allocator.h"
#include "NK/Core/Panic.h"
#include "NK/Config.h"

static
void*
P_NK_AllocatorImplementationGetAndRequestMerged(
    NK_Size size
)
{
    void* block;

    /** NOTE: We don't allow for 0-sized allocations: */
    if(size <= 0)
    {
        NK_Panic("[At: \"%s\"] Attempt to allocate block with size zero.");
    }
    else
    {
#ifndef NK_CONFIG_CORE_MEMORY_ALLOCATOR_GET_REQUEST_SKIP_MEMORY_ZEROING
        block = 
            NK_RedirectCalloc(
                sizeof(NK_U8),
                (size + sizeof(NK_AllocatorBlockInformation))
            );
#else
        /** NOTE: This is faster than the average speed BUT, less safe. */
        block = 
            NK_RedirectMalloc(
                sizeof(NK_U8) *
                (size + sizeof(NK_AllocatorBlockInformation))
            );
#endif
        NK_GlobalAllocatorStatistics.relative_size += size;
        NK_GlobalAllocatorStatistics.absolute_size +=
            (
                sizeof(NK_AllocatorBlockInformation) + size
            );
        NK_GlobalAllocatorStatistics.absolute_max = 
            (
                (
                    NK_GlobalAllocatorStatistics.absolute_size >
                    NK_GlobalAllocatorStatistics.absolute_max
                )
                ? NK_GlobalAllocatorStatistics.absolute_size
                : NK_GlobalAllocatorStatistics.absolute_max
            );  
        NK_GlobalAllocatorStatistics.online_blocks++;
    }
    return block;
}

void*
NK_AllocatorImplementationGet(
    const NK_C8* from,
    NK_Size size
)
{
    NK_AllocatorBlockInformation* adquired_block;
    NK_U8* payload;

    /** We get the block: */
    adquired_block = 
        (NK_AllocatorBlockInformation*)(
            P_NK_AllocatorImplementationGetAndRequestMerged(size)
        );
    if(adquired_block == NULL)
    {
        NK_Panic(
            "[At: \"%s\"] Failed to allocate: %lu bytes",
            from,
            size
        );
        return NULL;
    }
    else
    {
        adquired_block->size = size;
        /** We skip from the block information... */
        payload =
            ((NK_U8*)(adquired_block)) + sizeof(NK_AllocatorBlockInformation);

        /** Update the runtime information: */
        return (void*)(payload);
    }
}

void*
NK_AllocatorImplementationRequest(
    const NK_C8* from,
    NK_Size size
)
{
    NK_AllocatorBlockInformation* adquired_block;
    NK_U8* payload;

    /** We get the block: */
    adquired_block = 
        (NK_AllocatorBlockInformation*)(
            P_NK_AllocatorImplementationGetAndRequestMerged(size)
        );
    if(adquired_block == NULL)
    {
        /** NOTE: In this case, we're damned, bruh. */
        return NULL;
    }
    else
    {
        /** Write the size and get the payload. */
        adquired_block->size = size;
        payload =
            ((NK_U8*)(adquired_block)) + sizeof(NK_AllocatorBlockInformation);
        return (void*)(payload);
    }
}

void
NK_AllocatorImplementationFree(
    const NK_C8* from,
    void* block
)
{
    NK_AllocatorBlockInformation* block_itself;
    NK_AllocatorBlockInformation block_information;
    
    /** NOTE: We start to copy the information to the block: */
    NK_RedirectMemcpy(
        (void*)(&block_information),
        (NK_U8*)(block) - sizeof(NK_AllocatorBlockInformation),
        sizeof(NK_AllocatorBlockInformation)
    );
    block_itself = 
        (NK_AllocatorBlockInformation*)(
            ((NK_U8*)(block)) - sizeof(NK_AllocatorBlockInformation)
        );
    NK_RedirectFree(block_itself);

    /** Update the runtime information: */
    NK_GlobalAllocatorStatistics.online_blocks--;
    NK_GlobalAllocatorStatistics.relative_size -= block_information.size;
    NK_GlobalAllocatorStatistics.absolute_size -=
        (
            sizeof(NK_AllocatorBlockInformation) + block_information.size
        );
}

void*
NK_AllocatorImplementationResizeBlock(
    const NK_C8* from,
    void* block,
    const NK_Size new_size
)
{
    NK_AllocatorBlockInformation* block_itself;
    NK_AllocatorBlockInformation* new_block;
    NK_U8* payload;
    NK_U8* clean_zone;
    NK_Size past_size;

    /** Handle realloc(0): */
    if(new_size <= 0)
    {
        NK_Panic(
            "[At: \"%s\"] Attempt to realloc block %p to size 0",
            from,
            block
        );
    }

    /** We get the start of the block itself and save the `past_size`: */
    block_itself =
        (NK_AllocatorBlockInformation*)(
            ((NK_U8*)(block)) - sizeof(NK_AllocatorBlockInformation)
        );
    past_size = block_itself->size;

    /** Handle past_size == new_size: */
    if(new_size == past_size)
    {
        /** NOTE: return itself, duh. */
        return block;
    }

    /** Reallocate: */
    new_block = 
        (NK_AllocatorBlockInformation*)(
            realloc(
                block_itself,
                sizeof(NK_AllocatorBlockInformation) + 
                (sizeof(NK_U8) * new_size)
            )
        );

    /** Reallocate always fail when NULL. */
    if(new_block == NULL)
    {
        NK_Panic(
            "[At: \"%s\"] Failed to reallocate %lu bytes to %lu bytes on %p",
            from,
            past_size,
            new_size,
            block
        );
        return NULL;
    }

    /** We write the new block an value: */
    new_block->size = new_size;
    payload = ((NK_U8*)(new_block) + sizeof(NK_AllocatorBlockInformation));

#ifndef NK_CONFIG_CORE_MEMORY_ALLOCATOR_RESIZE_BLOCK_SKIP_MEMORY_ZEROING
    /**
     * Did we have a shrink or a growth? Because if we had, we have to guarantee
     * our NK_AllocatorImplementationResizeBlock must erase the unclean memory.
     * 
     * NOTE: This literally wastes linux RSS/VSS scheme, becuase we force the
     * reallocated memory to be clean.
     */
    if(new_size > past_size)
    {
        /** NOTE: in this case, we need to guarantee our buffer is clean. */
        clean_zone = payload + past_size;
        NK_RedirectMemset(
            clean_zone,
            0,
            sizeof(NK_U8) * (new_size - past_size)
        );
    }
#endif

    /** Update the statistics: */
    NK_GlobalAllocatorStatistics.relative_size =
        (NK_GlobalAllocatorStatistics.relative_size - past_size) + new_size;
    NK_GlobalAllocatorStatistics.absolute_size = 
        (
            NK_GlobalAllocatorStatistics.absolute_size -
            (sizeof(NK_AllocatorBlockInformation) + past_size)
        ) + (sizeof(NK_AllocatorBlockInformation) + new_size);
    NK_GlobalAllocatorStatistics.absolute_max = 
        (
            NK_GlobalAllocatorStatistics.absolute_size >
            NK_GlobalAllocatorStatistics.absolute_max
        )
        ? NK_GlobalAllocatorStatistics.absolute_size
        : NK_GlobalAllocatorStatistics.absolute_max;
    return payload;
}

/**
 * @brief This gets the current memory information about how much memory the
 * program is using.
 */
NK_AllocatorStatistics
NK_AllocatorImplementationGetCurrentStatistics()
{
    NK_AllocatorStatistics statistics;
    NK_RedirectMemcpy(
        (void*)&statistics,
        (const void*)&NK_GlobalAllocatorStatistics,
        sizeof(NK_AllocatorStatistics)
    );
    return statistics;
}