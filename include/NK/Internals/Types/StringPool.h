/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKInternalsTypesStringPool_h
#define NKInternalsTypesStringPool_h

#include "NK/Internals/Types/Table.h"
#include "NK/Types/Containers/Vector.h"
#include "NK/Types/Basics.h"

#define NK_STRING_POOL_NO_ID    0

/**
 * @brief Contains the carrier for the direct_pointers, it contains an
 * connection counter that says how much `NK_SubmergedStrings` are connected to
 * the string.
 */
typedef struct NK_StringPoolEntry
{
    /** @brief The actual reference to the NK_TableHeader->key. */
    NK_C8*  referencing;

    /** @brief The counter of connections. */
    NK_U64  connections;

    /** @brief The hash value of the string. */
    NK_U64  hash;
} NK_StringPoolEntry;

/**
 * @brief An string pool contains (you guessed it), strings. Most specifically
 * strings that can be reused and to avoid either: hashing again/ creating many
 * buffers. NK provides an string pool.
 * 
 * @warning It is better for you to not change the strings in the pool.
 * 
 * String Pool uses the internal NK_Table to keep an reference to a ID inside
 * the string pool list.
 */
typedef struct NK_StringPool
{
    /** Contain the strings: */
    NK_Table references;
    NK_Vector empty_spots;
    NK_Vector direct_pointers;
} NK_StringPool;

/** New, Free, Construct and Destruct: */

/**
 * @brief Creates an `NK_StringPool*`
 */
NK_StringPool*
NK_StringPoolNew();

/**
 * @brief Frees an `NK_StringPool*`
 * 
 * @warning Does not run `NK_StringPoolDestruct`!
 */
void
NK_StringPoolFree(
    NK_StringPool* pool
);

/**
 * @brief Construct the internal buffers.
 * 
 * @warning Always run `NK_StringPoolDestruct` or you will memory leak!
 */
void
NK_StringPoolConstruct(
    NK_StringPool* pool
);

/**
 * @brief Destruct the internal buffers.
 */
void
NK_StringPoolDestruct(
    NK_StringPool* pool
);

/**
 * @brief Insert an string in the pool or we find for one matching the string.
 * 
 * @returns The ID of the string for fast lookups.
 */
NK_U64
NK_StringPoolInsertOrFindMatching(
    NK_StringPool* pool,
    const NK_C8* src
);

/**
 * @brief Get an string using the ID.
 * 
 * @note `Get` is used here because the STRING returned is a constant and not
 * modifiable.
 */
const NK_C8*
NK_StringPoolGet(
    NK_StringPool* pool,
    const NK_U64 id
);

/**
 * @brief Get the `NK_StringPoolEntry` using the ID.
 * 
 * @note `At` is used here because this is the absolute value of any string.
 * 
 * @warning This is returned an constant point because you are not supposed to
 * temper the value.
 */
const NK_StringPoolEntry*
NK_StringPoolAt(
    NK_StringPool* pool,
    const NK_U64 id
);

/**
 * @brief Says that an string is not going to be used anymore.
 * 
 * @warning You must call this to indicate an possible GC on the strings that
 * won't be used anymore.
 */
void
NK_StringPoolDegrade(
    NK_StringPool* pool,
    const NK_U64 id
);

/**
 * @brief Says that an string is going to be used again.
 * 
 * @warning This says that the string will be used again.
 */
void
NK_StringPoolConnect(
    NK_StringPool* pool,
    const NK_U64 id
);

/**
 * @brief Test if the string is on the pool, this doesn't do any reference count
 * or anything, we just check if the string is present on the hash table.
 * 
 * @warning When not, we return `NK_STRING_POOL_NO_ID` (aka. 0).
 */
NK_U64
NK_StringPoolTest(
    NK_StringPool* pool,
    const NK_C8* src
);

#endif