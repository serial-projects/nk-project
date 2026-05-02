/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKTypesContainersMap_h
#define NKTypesContainersMap_h

#include "NK/Types/Basics.h"
#include "NK/Types/String.h"
#include "NK/Config.h"

/**
 * @brief This is the header of the chain itself.
 */
typedef struct NK_MapChainHeader
{
    /** How far we went on the memory itself. */
    NK_U32 explored;

    /** How far on the capacity we are. */
    NK_U32 capacity;
} NK_MapChainHeader;

/**
 * @brief This is the header of the NODE itself.
 */
typedef struct NK_MapNodeHeader
{
    /** @brief String ID on the pool. */
    NK_SubmergedString key;
} NK_MapNodeHeader;

/**
 * @brief This is a implementation of a hash table where the strings are stored
 * in the pool `NK_SubmergedString`, basically.
 */
typedef struct NK_Map
{
    /** 
     * @brief Contain the chains.
     */
    NK_U8** chains;
    NK_Size count;
    NK_Size element_size;
    NK_Size capacity;
} NK_Map;

/**
 * @brief Creates an new `NK_Map` for you to use forever.
 */
NK_Map*
NK_MapNew();

/**
 * @brief Frees an object created by `NK_MapNew` function.
 */
void
NK_MapFree(
    NK_Map* map
);

/**
 * @brief Construct the internal buffers of `NK_Map`
 */
void
NK_MapConstruct(
    NK_Map* map,
    const NK_Size capacity,
    const NK_Size element_size
);

/**
 * @brief Destruct the internal buffers.
 */
void
NK_MapDestruct(
    NK_Map* map
);

/** InsertOrAssign, Get, At & Remove: */

/**
 * @brief Insert or Assign an element on the map.
 */
NK_MapNodeHeader*
NK_MapInsertOrAssign(
    NK_Map* map,
    const NK_C8* key,
    void* src
);

/**
 * @brief Insert an element on the map.
 * 
 * @warning If the element already exists on the map, we don't insert or
 * replace it, if you want that behavior, you can use `NK_MapInsertOrAssign`
 * function, `NK_MapInsert` is just for inserting.
 * 
 * @warning Trying to insert elements that already exists will trigger panic!
 * 
 * @returns If the element was inserted.
 */
NK_MapNodeHeader*
NK_MapInsert(
    NK_Map* map,
    const NK_C8* key,
    void* src
);

/**
 * @brief This gets the value from the key provided.
 * 
 * @warning This actually gets the value.
 */
void*
NK_MapGet(
    NK_Map* map,
    const NK_C8* key
);

/**
 * @brief This gets the absolute value from the key provided.
 * 
 * @warning the absolute value is a decrement from the get, where
 * `NK_MapNodeHeader` is present.
 */
NK_MapNodeHeader*
NK_MapAt(
    NK_Map* map,
    const NK_C8* key
);

/**
 * @brief If you have an header, this will automatically return the data.
 */
void*
NK_MapGetFromNodeHeader(
    NK_Map* map,
    NK_MapNodeHeader* header
);

/**
 * @brief This removes an key from the map.
 */
NK_Result
NK_MapRemove(
    NK_Map* map,
    const NK_C8* key
);

/**
 * @brief This gets the amount of elements.
 */
const NK_Size
NK_MapSize(
    NK_Map* map
);

/**
 * @brief This is the default iterator for `NK_Map`
 */
typedef
NK_Result
(*NK_MapIteratorFunction)(
    NK_Map* map,
    const NK_C8* key,
    const NK_Size index,
    void* data,
    void* userdata
);

/**
 * @brief Iterate on the map.
 */
void
NK_MapIterate(
    NK_Map* map,
    const NK_MapIteratorFunction iterator,
    void* userdata
);

#endif