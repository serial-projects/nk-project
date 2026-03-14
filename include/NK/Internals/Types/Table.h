/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKInternalsTypesTable_h
#define NKInternalsTypesTable_h

#include "NK/Types/Basics.h"

/**
 * @brief On a chain, we have a small header, this says how big the chain is 
 * and how "deep" we are in the chain. The strategy is not to use the whole
 * memory and let the unmapped region (unsolved by the OS) not used.
 */
typedef struct NK_TableChainHeader
{
    /** How many elements there is on the list. */
    NK_U32          explored;

    /** How much the capacity of the chain is. */
    NK_U32          capacity;
} NK_TableChainHeader;

/**
 * @brief After the chain header, we have the node headers.
 */
typedef struct NK_TableChainNodeHeader
{
    /** NOTE: To fast comparasions, we do hashing. */
    NK_U64 hash;

    /** And if the hash matches, we do key. */
    NK_C8* key;
} NK_TableChainNodeHeader;

/**
 * @brief The Table Itself.
 * 
 * @warning Do not use this, prefer the optimized `NK_Map` because there, the
 * strings are stored in a string pool to save memory (and hash power).
 * 
 * NK_Table works on:
 *      [Nh1, Nh2, ... NhC] where C is the capacity, you have N- hash, those
 *      Nh are chains, holding the nodes packed together. Initially, the 
 *      `NK_TableInsertOrAssign` is smart enough to check ONLY the hashes, but
 *      on the flags, if the HASH is already colliding, we do the slow key
 *      check. But on hash, there is only a 2^64 possible keys hashed and we use
 *      FnV (but that can be changed).
 */
typedef struct NK_Table
{
    /**
     * @brief Contains the buffers to the chains.
     */
    NK_U8** data;

    /**
     * @brief The capacity of the chain holder.
     */
    NK_Size capacity;

    /**
     * @brief The counter for how many elements there is.
     */
    NK_Size counter;

    /**
     * @brief The size for element, remember, a node is: `NK_TableHeader` and
     * then the element itself.
     */
    NK_Size element_size;
} NK_Table;

/** New, Free, Construct and Destruct functions: */

/**
 * @brief Returns an new `NK_Table`.
 * 
 * @warning Does not construct the internal buffers, call `NK_TableConstruct`!
 */
NK_Table*
NK_TableNew();

/**
 * @brief Deletes the `NK_Table` returned by `NK_Table`
 * 
 * @warning Does not destruct the internal buffers, call `NK_TableDestruct`!
 */
void
NK_TableFree(
    NK_Table* table
);

/**
 * @brief Construct the internal buffers.
 */
void
NK_TableConstruct(
    NK_Table* table,
    const NK_Size capacity,
    const NK_Size element_size
);

/**
 * @brief Destruct the internal buffers.
 * 
 * @warning You still must use `NK_TableFree` in case you operating on a
 * `NK_Table` created by `NK_TableNew`.
 */
void
NK_TableDestruct(
    NK_Table* table
);

/** Table Operations: */

/**
 * @brief Gets how much elements there are on the table.
 */
NK_Size
NK_TableSize(
    NK_Table* table
);

/**
 * @brief Insert or Assign an Value on the table.
 * 
 * @returns Returns the absolute data (aka. the key/hash). This is done most
 * because on `NK_StringPoolInsert`, we want to insert and then already get
 * the header.
 */
NK_TableChainNodeHeader*
NK_TableInsertOrAssign(
    NK_Table* table,
    const NK_U8* key,
    void* src
);

/**
 * @brief Get an Value.
 * 
 * @warning Returns `NULL` for when the value wasn't found.
 * @note This `Get` returns the relative value (aka. the data).
 */
void*
NK_TableGet(
    NK_Table* table,
    const NK_U8* key
);

/**
 * @brief At the Value.
 * 
 * @warning Retuns `NULL` for when the value wasn't found.
 * @warning This `At` returns the absolute value (aka. the header)
 */
NK_TableChainNodeHeader*
NK_TableAt(
    NK_Table* table,
    const NK_U8* key
);

/**
 * @brief Remove an Value.
 * 
 * @returns If the value was removed.
 */
NK_Result
NK_TableRemove(
    NK_Table* table,
    const NK_U8* key
);

/** Iterate: */

/**
 * @brief This is the function type used on the `NK_TableIterate` function.
 */
typedef
NK_Result
(*NK_TableIteratorFunction)(
    NK_Table* table,
    const NK_C8* key,
    const NK_Size index,
    void* data,
    void* userdata
);

/**
 * @brief Iterate the Table.
 * 
 * @warning return `true` to continue and `false` to break.
 */
void
NK_TableIterate(
    NK_Table* table,
    const NK_TableIteratorFunction iterator,
    void* userdata
);

#endif