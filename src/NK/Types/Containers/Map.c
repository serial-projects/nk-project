/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Types/Containers/Map.h"
#include "NK/Memory.h"
#include "NK/Config.h"

NK_Map*
NK_MapNew()
{
    NK_Map* new_map = 
        (NK_Map*)(
            NK_AllocatorGet(sizeof(NK_Map))
        );
    return new_map;
}
void
NK_MapFree(
    NK_Map* map
)
{
    NK_AllocatorFree(map);
}

void
NK_MapConstruct(
    NK_Map* map,
    const NK_Size capacity,
    const NK_Size element_size
)
{
    map->capacity = capacity;
    map->count = 0;
    map->element_size = element_size;
    map->chains = 
        (NK_U8**)(
            NK_AllocatorGet(
                sizeof(NK_MapChainHeader) +
                (map->capacity * sizeof(NK_MapNodeHeader))
            )
        );
    NK_RedirectMemset(
        map->chains,
        0,
        sizeof(NK_C8*) * map->capacity
    );
}

static
void
P_NK_MapDestructChain(
    NK_Map* map,
    NK_U8* chain
)
{
    /** CC = Current Chain. */
    NK_MapChainHeader* cc_header = (NK_MapChainHeader*)(chain);
    NK_MapNodeHeader* cc_node_header;
    NK_U32 index = 0;
    for(index; index < cc_header->explored; index++)
    {
        cc_node_header = 
            (NK_MapNodeHeader*)(
                ((NK_U8*)(chain) + sizeof(NK_MapChainHeader)) + 
                (
                    index *
                    (sizeof(NK_MapNodeHeader) + map->element_size)
                )
            );
        /** 
         * NOTE: We don't need to be worried for thumbstones, once the
         * `NK_StringPool` doesn't care for 0 ID strings. But we do check
         * anyway.
         */
        if(NK_SubmergedStringIsForgotten(&cc_node_header->key))
        {
            continue;
        }
        NK_SubmergedStringDestruct(&cc_node_header->key);
    }
}

void
NK_MapDestruct(
    NK_Map* map
)
{
    NK_Size index = 0;
    NK_U8* chain;
    for(index; index < map->capacity; index++)
    {
        chain = map->chains[index];
        if(chain == NULL)
        {
            continue;
        }
        P_NK_MapDestructChain(map, chain);
        NK_AllocatorFree(chain);
    }
    NK_AllocatorFree(map->chains);
}

/** InsertOrAssign, Get & Remove: */

NK_MapNodeHeader*
NK_MapInsertOrAssign(
    NK_Map* map,
    const NK_C8* key,
    void* src
)
{
    /** Hash the string: */
    NK_SubmergedString s_key;
    NK_SubmergedStringConstruct(&s_key, key);

    /** Get the chain we will be looking for: */
    NK_U64 look_chain = NK_SubmergedStringGetHash(&s_key) % map->capacity;
    NK_U8* chain = map->chains[look_chain];

    /** For new chain and resizing: */
    NK_U8* new_chain;
    NK_U32 new_capacity;

    NK_MapChainHeader new_header;
    new_header.capacity = NK_CONFIG_CONTAINERS_MAP_CHAIN_DEFAULT_SIZE;
    new_header.explored = 0;

    /** Set the chain: */
    NK_MapChainHeader* cc_header;
    NK_MapNodeHeader* cc_node_header;
    NK_MapNodeHeader* empty_spot = NULL;
    NK_U8* data;
    NK_U32 index = 0;

    /** Do we have a new chain? */
    if(chain == NULL)
    {
        new_chain = 
            (NK_U8*)(
                NK_AllocatorGet(
                    sizeof(NK_MapChainHeader) + 
                    (
                        new_header.capacity *
                        (sizeof(NK_MapNodeHeader) + map->element_size)
                    )
                )
            );
        map->chains[look_chain] = new_chain;
        chain = new_chain;
        cc_header = (NK_MapChainHeader*)(chain);
        *cc_header = new_header;
    }
    else
    {
        cc_header = (NK_MapChainHeader*)(chain);
    }

    index = 0;
    for(index; index < cc_header->explored; index++)
    {
        cc_node_header = 
            (NK_MapNodeHeader*)(
                ((NK_U8*)(chain) + sizeof(NK_MapChainHeader)) + 
                (
                    index *
                    (sizeof(NK_MapNodeHeader) + map->element_size)
                )
            );
        if(NK_SubmergedStringEqual(&cc_node_header->key, &s_key))
        {
            /** Assignment: */
            data =
                (NK_U8*)(
                    (NK_U8*)(cc_node_header) +
                    sizeof(NK_MapNodeHeader)
                );
            NK_RedirectMemcpy(
                data,
                src,
                map->element_size
            );

            /**
             * NOTE: Here we don't let the s_key live for more than necessary:
             */
            NK_SubmergedStringDestruct(&s_key);
            goto assignment_ending;
        }
        if(
            empty_spot == NULL &&
            NK_SubmergedStringIsForgotten(&cc_node_header->key)
        )
        {
            empty_spot = cc_node_header;
        }
    }

    if(empty_spot != NULL)
    {
        /** NOTE: In this case, we got an forgotten spot: */
        cc_node_header = empty_spot;
        cc_node_header->key = s_key;
        data = 
            (NK_U8*)(
                (NK_U8*)(cc_node_header) + 
                sizeof(NK_MapNodeHeader)
            );
        NK_RedirectMemcpy(
            data,
            src,
            map->element_size
        );
        goto empty_spot_found_ending;
    }

    /** Continue: */
    if(cc_header->explored >= cc_header->capacity)
    {
        new_capacity = cc_header->capacity * 2;
        new_chain =
            (NK_U8*)(
                NK_AllocatorResizeBlock(
                    chain,
                    sizeof(NK_MapChainHeader) + 
                    (
                        new_capacity *
                        (sizeof(NK_MapNodeHeader) + map->element_size)
                    )
                )
            );
        map->chains[look_chain] = new_chain;
        chain = new_chain;

        /** Update the variables: */
        cc_header = (NK_MapChainHeader*)(chain);
        cc_header->capacity = new_capacity;
    }

    /** 
     * We set the current to the cc_header->explored since we know it will be
     * on the next available slot anyway.
     */
    cc_node_header =
        (NK_MapNodeHeader*)(
            (NK_U8*)(chain) +
            sizeof(NK_MapChainHeader) + 
            (
                cc_header->explored *
                (sizeof(NK_MapNodeHeader) + map->element_size)
            )
        );
    cc_node_header->key = s_key;
    data =
        (NK_U8*)(
            (NK_U8*)(cc_node_header) + 
            sizeof(NK_MapNodeHeader)
        );
    NK_RedirectMemcpy(
        data,
        src,
        map->element_size
    );
    cc_header->explored++;

empty_spot_found_ending:
    map->count++;
    
    /** NOTE: During an assignment, we don't have an map->count increment! */
assignment_ending:
    return cc_node_header;
}

void*
NK_MapGet(
    NK_Map* map,
    const NK_C8* key
)
{
    NK_SubmergedString s_key;
    NK_SubmergedStringConstruct(&s_key, key);
    NK_U64 look_chain = NK_SubmergedStringGetHash(&s_key) % map->capacity;
    NK_U8* chain = map->chains[look_chain];

    NK_MapChainHeader* cc_header;
    NK_MapNodeHeader* cc_node_header;
    NK_U32 index;
    NK_U8* data = NULL;
    
    if(chain == NULL)
    {
        goto no_chain_ending;
    }

    cc_header = (NK_MapChainHeader*)(chain);
    index = 0;
    for(index; index < cc_header->explored; index++)
    {
        cc_node_header = 
            (NK_MapNodeHeader*)(
                ((NK_U8*)(chain) + sizeof(NK_MapChainHeader)) +
                (
                    index *
                    (sizeof(NK_MapNodeHeader) + map->element_size)
                )
            );
        if(NK_SubmergedStringEqual(&cc_node_header->key, &s_key))
        {
            data =
                (NK_U8*)(
                    (NK_U8*)(cc_node_header) + 
                    sizeof(NK_MapNodeHeader)
                );
            break;
        }
    }

no_chain_ending:
    NK_SubmergedStringDestruct(&s_key);
    return data;
}

NK_MapNodeHeader*
NK_MapAt(
    NK_Map* map,
    const NK_C8* key
)
{
    NK_U8* data =
        NK_MapGet(map, key);
    return (
        data != NULL
        ? (NK_MapNodeHeader*)( (NK_U8*)(data) - sizeof(NK_MapNodeHeader) )
        : NULL
    );
}

NK_Result
NK_MapRemove(
    NK_Map* map,
    const NK_C8* key
)
{
    NK_SubmergedString s_key;
    NK_SubmergedStringConstruct(&s_key, key);
    NK_U64 look_chain = NK_SubmergedStringGetHash(&s_key) % map->capacity;
    NK_U8* chain = map->chains[look_chain];
    NK_MapChainHeader* cc_header;
    NK_MapNodeHeader* cc_node_header;
    NK_U32 index;
    NK_Result removed = false;

    if(chain == NULL)
    {
        goto no_chain_ending;
    }

    index = 0;
    cc_header = (NK_MapChainHeader*)(chain);
    for(index; index < cc_header->explored; index++)
    {
        cc_node_header = 
            (NK_MapNodeHeader*)(
                ((NK_U8*)(chain) + sizeof(NK_MapChainHeader)) + 
                (
                    index *
                    (sizeof(NK_MapNodeHeader) + map->element_size)
                )
            );
        if(NK_SubmergedStringEqual(&cc_node_header->key, &s_key))
        {
            /** NOTE: This also forgets the string. */
            NK_SubmergedStringDestruct(&cc_node_header->key);
            removed = true;
            break;
        }
    }

no_chain_ending:
    NK_SubmergedStringDestruct(&s_key);
    return removed;
}

void
NK_MapIterate(
    NK_Map* map,
    const NK_MapIteratorFunction iterator,
    void* userdata
)
{
    NK_Size counter = 0;
    NK_Size chain_index = 0;
    NK_Size node_index;
    NK_U8* chain;
    NK_MapChainHeader* cc_header;
    NK_MapNodeHeader* cc_node_header;
    NK_U8* data;
    for(chain_index; chain_index < map->capacity; chain_index++)
    {
        chain = map->chains[chain_index];
        if(chain == NULL)
        {
            continue;
        }
        node_index = 0;
        cc_header = (NK_MapChainHeader*)(chain);
        for(node_index; node_index < cc_header->explored; node_index++)
        {
            cc_node_header = 
                (NK_MapNodeHeader*)(
                    ((NK_U8*)(chain) + sizeof(NK_MapChainHeader)) + 
                    (
                        node_index *
                        (sizeof(NK_MapNodeHeader) + map->element_size)
                    )
                );
            if(NK_SubmergedStringIsForgotten(&cc_node_header->key))
            {
                continue;
            }
            data = 
                (NK_U8*)(
                    (NK_U8*)(cc_node_header) +
                    sizeof(NK_MapNodeHeader)
                );
            if(
                !iterator(
                    map,
                    NK_SubmergedStringGet(&cc_node_header->key),
                    counter,
                    (void*)(data),
                    userdata
                )
            )
            {
                goto terminate_ending;
            }
            counter++;
        }
    }
terminate_ending:
    return;
}