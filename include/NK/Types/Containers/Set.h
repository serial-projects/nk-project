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

/**
 * @brief Inserts an element in the `NK_Set` and starts counting on many times
 * it is present inside the list. You don't hold any information on `NK_Set`
 * on the "standard way", what you store in the `NK_Set` is counting, for
 * example: Value `A` _exists_ 10 times, Value `B` _exists_ 2 times, etc. This
 * is hashed for speed during access.
 * 
 * @warning This may be a multiset also, to avoid many `NK_<>Set`
 * implementations, `NK_Set` behaves as the two at the same time.
 * 
 * @warning `NK_Set` holds `NK_U32` reference counts, more implies on cappping
 * the value, so you shouldn't be holding 2^32 references to elements.
 */
const NK_U32
NK_SetInsertOrAssign(
    NK_Set* set,
    const NK_C8* key
);

/**
 * @brief This gets the `NK_SetNodeHeader` attributed to the `key`
 * 
 * @note This is a `At` function since it points to the absolute value of the
 * `key` you want, which is the node itself.
 */
NK_SetNodeHeader*
NK_SetAt(
    NK_Set* set,
    const NK_C8* key
);

/**
 * @brief This function will remove the element from the set, even if it repeats
 * multiple times.
 */
const NK_Result
NK_SetRemove(
    NK_Set* set,
    const NK_C8* key
);

/**
 * @brief This function will `drop` an element from the set, if the reference
 * counter reaches `0`, then the element will be `0`.
 */
const NK_U32
NK_SetDrop(
    NK_Set* set,
    const NK_C8* key
);

/**
 * @brief Return how many times the object is present on the set, on the
 * `NK_Set` we count references.
 * 
 * @warning Returns `0` when the `key` was not found, this means the set
 * contains nothing of the `key` inside it.
 * 
 * @note This is a `Get` function that returns the amount of times the object
 * is being referenced, note the `NK_SetNodeHeader`, for that, you should use
 * `At` function, which returns the absolute value.
 */
const NK_U32
NK_SetGet(
    NK_Set* set,
    const NK_C8* key
);

/**
 * @brief Unlike the other objects, on `NK_Set`, we have `NK_SetHas` which
 * returns an `NK_Result` saying if the `NK_Set` does contain the `key` you so
 * provided.
 */
const NK_Result
NK_SetContains(
    NK_Set* set,
    const NK_C8* key
);

#endif