/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKTypesContainersSet_h
#define NKTypesContainersSet_t

#include "NK/Types/Basics.h"
#include "NK/Types/String/Submerged.h"

/**
 * @brief This is the top of the chain.
 */
typedef struct NK_SetChainHeader
{
    /**
     * @brief This holds the capacity of the chain.
     */
    NK_U32 explored;

    /**
     * @brief This holds the amount the chain was explored.
     */
    NK_U32 capacity;
} NK_SetChainHeader;

/**
 * @brief This contains the set node.
 */
typedef struct NK_SetNodeHeader
{
    /**
     * @brief This is the key that we need to compare to know if our content is
     * right.
     */
    NK_SubmergedString key;

    /**
     * @brief This is the counter for the amount of elements of this type has
     * on it.
     */
    NK_U64 count;
} NK_SetNodeHeader;

/**
 * @brief This is the set, an set contains objects (keys) and we must know
 * how much of this key is present (or if it is present even).
 */
typedef struct NK_Set
{
    /**
     * @brief Contain the chains.
     */
    NK_U8** chains;

    /**
     * @brief Contain the counter for amount of set objects.
     */
    NK_Size count;

    /**
     * @brief Contain the capacity, the number of chains, the more bigger, the
     * faster the access to the set it will be.
     */
    NK_Size capacity;
} NK_Set;

/**
 * @brief Creates an new `NK_Set` object.
 */
NK_Set*
NK_SetNew();

/**
 * @brief Frees an `NK_Set` created by the `NK_SetNew` function.
 */
void
NK_SetFree(
    NK_Set* set
);

/**
 * @brief Construct an `NK_Set` with the capacity you desire.
 */
void
NK_SetConstruct(
    NK_Set* set,
    const NK_Size capacity
);

/**
 * @brief Destruct the `NK_Set` buffers.
 * @warning Does not imply on `NK_SetFree`
 */
void
NK_SetDestruct(
    NK_Set* set
);

#endif