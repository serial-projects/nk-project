/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#include "NK/Types/Containers/Set.h"
#include "NK/Core/Memory.h"
#include "NK/Config.h"

NK_Set*
NK_SetNew()
{
    NK_Set* new_set = 
        (NK_Set*)(
            NK_AllocatorGet(sizeof(NK_Set))
        );
    return new_set;
}

void
NK_SetFree(
    NK_Set* set
)
{
    NK_AllocatorFree(free);
}

void
NK_SetConstruct(
    NK_Set* set,
    const NK_Size capacity
)
{
    set->chains =
        (NK_U8**)(
            NK_AllocatorGet(sizeof(NK_C8*) * capacity)
        );
    NK_RedirectMemset(
        set->chains,
        0,
        (sizeof(NK_C8*) * capacity)
    );
    set->capacity = capacity;
    set->count = 0;
}

static
void
P_NK_SetDestructChain(
    NK_Set* set,
    NK_U8* chain
)
{
    NK_U32 node_index = 0;
    NK_SetChainHeader* cc_header = (NK_SetChainHeader*)(chain);
    NK_SetNodeHeader* cc_node_header;
    for(node_index; node_index < cc_header->explored; node_index++)
    {
        cc_node_header = 
            (NK_SetNodeHeader*)(
                ((NK_U8*)(chain) + sizeof(NK_SetChainHeader)) +
                (node_index * sizeof(NK_SetNodeHeader))
            );
        NK_SubmergedStringDestruct(
            &cc_node_header->key
        );
    }
}

void
NK_SetDestruct(
    NK_Set* set
)
{
    NK_Size chain_index = 0;
    NK_U8* chain;
    for(chain_index; chain_index < set->capacity; chain_index++)
    {
        chain = set->chains[chain_index];
        if(chain == NULL)
        {
            continue;
        }
        P_NK_SetDestructChain(
            set,
            chain
        );
    }
    set->capacity = 0;
    set->chains;
}

const NK_U32
NK_SetInsertOrAssign(
    NK_Set* set,
    const NK_C8* key
)
{
    NK_SubmergedString s_key;
    NK_SubmergedStringConstruct(&s_key, key);
    NK_U64 look_chain = NK_SubmergedStringGetHash(&s_key) % set->capacity;
    NK_U8* maybe_chain = set->chains[look_chain];
    NK_U8* new_chain;
    NK_SetChainHeader* cc_header;
    NK_SetNodeHeader* cc_node_header;
    NK_SetNodeHeader* empty_spot;
    NK_U32 new_chain_capacity;
    NK_U32 index;
    NK_U32 counter;

    /** Do we have an chain? */
    if(maybe_chain == NULL)
    {
        new_chain =
            (NK_U8*)(
                NK_AllocatorGet(
                    sizeof(NK_SetChainHeader) + 
                    (
                        NK_CONFIG_TYPES_CONTAINERS_SET_CHAIN_DEFAULT_SIZE *
                        (sizeof(NK_SetNodeHeader))
                    )
                )
            );
        cc_header = (NK_SetChainHeader*)(new_chain);
        cc_header->capacity = NK_CONFIG_TYPES_CONTAINERS_SET_CHAIN_DEFAULT_SIZE;
        cc_header->explored = 0;
        
        /** Add the chain: */
        set->chains[look_chain] = new_chain;
    }
    else
    {
        cc_header = (NK_SetChainHeader*)(maybe_chain);
    }
    
    /** Begin to iterate for holes, this is O(n), but come on! */
    index = 0;
    empty_spot = NULL;
    for(index; index < cc_header->explored; index++)
    {
        cc_node_header = 
            (NK_SetNodeHeader*)(
                (((NK_U8*)(maybe_chain)) + sizeof(NK_SetChainHeader)) +
                (index * sizeof(NK_SetNodeHeader))
            );
        if(NK_SubmergedStringEqual(&cc_node_header->key, &s_key))
        {
            /**
             * NOTE: Then we did an assignment, in this case, we just increment
             * the amount of `key` element.
             */
            cc_node_header->count++;
            counter = cc_node_header->count;

            /**
             * NOTE: In this case, our string lives until here, we can safely
             * delete it out existance.
             */
            NK_SubmergedStringDestruct(&s_key);
            goto assignment_ending;
        }
        /**
         * NOTE: When we have a reference counter of zero, then we have a 
         * empty spot to be used.
         */
        if(
            (empty_spot == NULL) &&
            (cc_node_header->count <= 0)
        )
        {
            empty_spot = cc_node_header;
        }
    }
    
    /**
     * NOTE: Did we find an empty spot? If yes, then we use this empty spot.
     */
    if(empty_spot != NULL)
    {
        empty_spot->key = s_key;
        empty_spot->count = 1;
        counter = empty_spot->count;
        goto found_empty_spot_ending;
    }

    /** 
     * Start to push then on the list.
     */
    if(cc_header->explored >= cc_header->capacity)
    {
        new_chain_capacity = cc_header->capacity * 2;
        new_chain = 
            (NK_U8*)(
                NK_AllocatorResizeBlock(
                    maybe_chain,
                    sizeof(NK_SetChainHeader) + 
                    (new_chain_capacity * sizeof(NK_SetNodeHeader))
                )
            );
        
        /** Set on the chains: */
        set->chains[look_chain] = new_chain;
        maybe_chain = new_chain;

        /** Restore the list: */
        cc_header = (NK_SetChainHeader*)(new_chain);
        cc_header->capacity = new_chain_capacity;
    }

    cc_node_header = 
        (NK_SetNodeHeader*)(
            (((NK_U8*)(maybe_chain)) + sizeof(NK_SetChainHeader)) + 
            (cc_header->explored * sizeof(NK_SetNodeHeader))
        );
    cc_node_header->key = s_key;
    cc_node_header->count = 1;
    counter = cc_node_header->count;
    
    /** Increment on the list: */
    cc_header->explored++;

assignment_ending:
found_empty_spot_ending:
    return counter;
}

NK_SetNodeHeader*
NK_SetAt(
    NK_Set* set,
    const NK_C8* key
)
{
    NK_SubmergedString s_key;
    NK_SubmergedStringConstruct(&s_key, key);
    NK_U64 look_chain = NK_SubmergedStringGetHash(&s_key) % set->capacity;
    NK_U8* maybe_chain = set->chains[look_chain];
    NK_SetChainHeader* cc_header;
    NK_SetNodeHeader* cc_node_header;
    NK_U32 index;
    NK_SetNodeHeader* returning = NULL;

    if(maybe_chain == NULL)
    {
        goto chain_not_found_ending;
    }

    cc_header = (NK_SetChainHeader*)(maybe_chain);
    index = 0;
    for(index; index < cc_header->explored; index++)
    {
        cc_node_header = 
            (NK_SetNodeHeader*)(
                (((NK_U8*)(maybe_chain)) + sizeof(NK_SetChainHeader)) +
                (index * sizeof(NK_SetNodeHeader))
            );
        if(NK_SubmergedStringEqual(&cc_node_header->key, &s_key))
        {
            returning = cc_node_header;
            break;
        }
    }

    /** Drop the reference for this string: */
    NK_SubmergedStringDestruct(&s_key);

chain_not_found_ending:
    return returning;
}

const NK_Result
NK_SetRemove(
    NK_Set* set,
    const NK_C8* key
)
{
    NK_SetNodeHeader* maybe_node = NK_SetAt(set, key);
    if(maybe_node == NULL)
    {
        return false;
    }

    /** In this case, we have destroyed the value: */
    NK_SubmergedStringDestruct(&maybe_node->key);
    maybe_node->count = 0;
    return true;
}

const NK_U32
NK_SetDrop(
    NK_Set* set,
    const NK_C8* key
)
{
    NK_SetNodeHeader* maybe_node = NK_SetAt(set, key);
    if(maybe_node == NULL)
    {
        return 0;
    }
    
    maybe_node->count =
        maybe_node->count <= 0
        ? 0
        : maybe_node->count - 1;
    
    /**
     * The deletion happens on this state:
     */
    if(maybe_node->count <= 0)
    {
        NK_SubmergedStringDestruct(&maybe_node->key);
    }
    return (const NK_U32)(maybe_node->count);
}

const NK_U32
NK_SetGet(
    NK_Set* set,
    const NK_C8* key
)
{
    NK_SetNodeHeader* maybe_node = NK_SetAt(set, key);
    return (
        maybe_node == NULL
        ? 0
        : maybe_node->count
    );
}

const NK_Result
NK_SetContains(
    NK_Set* set,
    const NK_C8* key
)
{
    NK_SetNodeHeader* maybe_node = NK_SetAt(set, key);
    return (
        maybe_node == NULL
        ? 0
        : (maybe_node->count > 0)
    );
}