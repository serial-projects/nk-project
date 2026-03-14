/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#include "NK/Types/Containers/Set.h"
#include "NK/Core/Memory.h"

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