/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Internals/Types/StringPool.h"
#include "NK/Memory.h"
#include "NK/Config.h"

NK_StringPool*
NK_StringPoolNew()
{
    NK_StringPool* new_pool = 
        (NK_StringPool*)(NK_AllocatorGet(sizeof(NK_StringPool)));
    return new_pool;
}

void
NK_StringPoolFree(
    NK_StringPool* pool
)
{
    NK_AllocatorFree(pool);
}

void
NK_StringPoolConstruct(
    NK_StringPool* pool
)
{
    /** Construct the table with the strings: */
    NK_TableConstruct(
        &pool->references,
        NK_CONFIG_INTERNALS_STRINGPOOL_CONSTRUCT_TABLE_CAPACITY,
        sizeof(NK_Size)
    );

    /** Construct the list with the pointers: */
    NK_VectorConstruct(
        &pool->direct_pointers,
        sizeof(NK_StringPoolEntry)
    );

    NK_VectorConstruct(
        &pool->empty_spots,
        sizeof(NK_U64)
    );
}

void
NK_StringPoolDestruct(
    NK_StringPool* pool
)
{
    NK_VectorDestruct(&pool->direct_pointers);
    NK_VectorDestruct(&pool->empty_spots);
    NK_TableDestruct(&pool->references);
}

static
NK_U64
P_NK_StringPoolInsertGetValidEntrance(
    NK_StringPool* pool
)
{
    NK_StringPoolEntry empty_spot;
    NK_RedirectMemset((void*)&empty_spot, 0, sizeof(NK_StringPoolEntry));
    NK_U64 valid_spot;

    /** Can we get an valid spot? */
    if(NK_VectorSize(&pool->empty_spots) > 0)
    {
        NK_VectorPop(&pool->empty_spots, (void*)(&valid_spot));
        goto found_valid_spot_ending;
    }

    /** In this case, we found nothing, so: */
    NK_VectorPush(&pool->direct_pointers, (void*)&empty_spot);
    valid_spot = NK_VectorSize(&pool->direct_pointers) - 1;
found_valid_spot_ending:
    return valid_spot;
}

NK_U64
NK_StringPoolInsertOrFindMatching(
    NK_StringPool* pool,
    const NK_C8* src
)
{
    NK_U64* maybe_spot;
    NK_StringPoolEntry* entry = NULL;
    NK_StringPoolEntry new_spot;
    NK_U64 adquired_spot;
    NK_TableChainNodeHeader* header;

    /** We get the possible pool reference: */
    maybe_spot = (NK_U64*)(NK_TableGet(&pool->references, src));
    if(maybe_spot != NULL)
    {
        /** We saved memory, we're good to go. */
        entry = 
            (NK_StringPoolEntry*)(
                NK_VectorAt(
                    &pool->direct_pointers,
                    (*maybe_spot)
                )
            );
        entry->connections++;
        return (*maybe_spot) + 1;
    }

    /** NOTE: Is this reserved for us? */
    adquired_spot = P_NK_StringPoolInsertGetValidEntrance(pool);

    /**
     * Then, we register it on the table (and we save the header). To avoid
     * having to duplicate the string, we point to the header string itself,
     * since that is a NK_C8* pointer anyway and it won't change EVEN if the
     * table chain resizes.
     */
    header = 
        NK_TableInsertOrAssign(
            &pool->references,
            src,
            (void*)&adquired_spot
        );

    /** See? We take this pointer: */
    new_spot.referencing = header->key;
    new_spot.hash = header->hash;
    new_spot.connections = 1;

    /** We write on the vector: */
    NK_RedirectMemcpy(
        NK_VectorAt(&pool->direct_pointers, adquired_spot),
        (const void*)(&new_spot),
        sizeof(NK_StringPoolEntry)
    );

    /** 
     * We increment +1 on the string pool, after all, zero indicates an NULL
     * string.
     */
    return adquired_spot + 1;
}

const NK_C8*
NK_StringPoolGet(
    NK_StringPool* pool,
    const NK_U64 id
)
{
    NK_StringPoolEntry* maybe_entry = NULL;
    if(id != NK_STRING_POOL_NO_ID)
    {
        maybe_entry = 
            (NK_StringPoolEntry*)(
                NK_VectorAt(
                    &pool->direct_pointers,
                    /** NOTE: We need to decrease since 0 indicates NULL string */
                    (id - 1)
                )
            );
    }
    return 
        (const NK_C8*)(
            maybe_entry != NULL
            ? maybe_entry->referencing
            : NULL
        );
}

const NK_StringPoolEntry*
NK_StringPoolAt(
    NK_StringPool* pool,
    const NK_U64 id
)
{
    NK_StringPoolEntry* maybe_entry = NULL;
    if(id != NK_STRING_POOL_NO_ID)
    {
        maybe_entry = 
            (NK_StringPoolEntry*)(
                NK_VectorAt(
                    &pool->direct_pointers,
                    (id - 1)
                )
            );
    }
    return (const NK_StringPoolEntry*)(maybe_entry);
}

static
void
P_NK_StringPoolDegradeSweep(
    NK_StringPool* pool,
    const NK_U64 id
)
{
    /** 
     * NOTE: We mark this value as an empty spot, that way, we keep everything
     * O(1), because check the direct_pointers would just be O(n).
     */
    NK_StringPoolEntry* maybe_entry = 
        (NK_StringPoolEntry*)(NK_VectorAt(&pool->direct_pointers, id));
    NK_TableRemove(&pool->references, maybe_entry->referencing);
    
    /** This sets the place to 0. */
    NK_RedirectMemset(maybe_entry, 0, sizeof(NK_StringPoolEntry));
    NK_VectorPush(&pool->empty_spots, (const void*)&id);
}

void
NK_StringPoolDegrade(
    NK_StringPool* pool,
    const NK_U64 id
)
{
    NK_StringPoolEntry* maybe_entry = NULL;
    if(id != NK_STRING_POOL_NO_ID)
    {
        maybe_entry = 
            (NK_StringPoolEntry*)(
                NK_VectorAt(
                    &pool->direct_pointers,
                    (id - 1)
                )
            );
    }

    /** In this case, we are not doing anything. */
    if(maybe_entry == NULL)
    {
        /** NOTE: bad degration? */
        return;
    }

    /** We degrade, and when it is fully degraded, then we delete it. */
    maybe_entry->connections =
        (
            maybe_entry->connections <= 0
            ? 0
            : maybe_entry->connections - 1
        );
    if(maybe_entry->connections <= 0)
    {
        P_NK_StringPoolDegradeSweep(
            pool,
            (id - 1)
        );
    }
}

void
NK_StringPoolConnect(
    NK_StringPool* pool,
    const NK_U64 id
)
{
    NK_StringPoolEntry* maybe_entry = NULL;
    if(id != NK_STRING_POOL_NO_ID)
    {
        maybe_entry = 
            (NK_StringPoolEntry*)(
                NK_VectorAt(
                    &pool->direct_pointers,
                    (id - 1)
                )
            );
    }

    if(maybe_entry == NULL)
    {
        return;
    }

    /** Here we found an connection, then it is good. */
    maybe_entry->connections++;
}

NK_U64
NK_StringPoolTest(
    NK_StringPool* pool,
    const NK_C8* src
)
{
    NK_U64* maybe_string = 
        (NK_U64*)(
            NK_TableGet(
                &pool->references,
                src
            )
        );
    return (
        maybe_string != NULL
        ? (*maybe_string) + 1
        : 0
    );
}
