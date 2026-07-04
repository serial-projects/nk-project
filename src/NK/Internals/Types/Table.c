/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
 
#include "NK/Internals/Types/Table.h"
#include "NK/Types/Basics.h"
#include "NK/Internals/Hash.h"
#include "NK/Memory.h"
#include "NK/Config.h"

NK_Table*
NK_TableNew()
{
    NK_Table* new_table =
        (NK_Table*)(
            NK_AllocatorGet(sizeof(NK_Table))
        );
    return new_table;
}

void
NK_TableFree(
    NK_Table* table
)
{
    NK_AllocatorFree(table);
}

void
NK_TableConstruct(
    NK_Table* table,
    const NK_Size capacity,
    const NK_Size element_size
)
{
    table->capacity = capacity;
    table->counter = 0;
    table->element_size = element_size;

    /**
     * NOTE: We need to zero this out, we will be holding points, it is not
     * safe to have dirty (garbage data) on here.
     */
    table->data =
        (NK_U8**)(
            NK_AllocatorGet(
                sizeof(NK_U8*) * (capacity + 1)
            )
        );
    NK_RedirectMemset(
        table->data,
        0,
        (sizeof(NK_U8*) * (capacity + 1))
    );
}

static
void
P_NK_TableDestructChain(
    NK_Table* table,
    NK_U8* chain
)
{
    /**
     * The layout of a chain is:
     * [TableChainHeader]
     *  [TableChainNodeHeader(0)][Element(0)]
     *  [TableChainNodeHeader(1)][Element(1)]
     *  ...
     *  [TableChainNodeHeader(n)][Element(n)]
     * 
     * Everything is densely packed together.
     * The "explored" key says how deep we are on the chain;
     * The "capacity" key says how big the chain is.
     */
    NK_Size index = 0;
    NK_TableChainHeader* current_h = (NK_TableChainHeader*)(chain);
    NK_TableChainNodeHeader* current_node_h = NULL;
    for(
        index;
        index < current_h->explored;
        index++
    )
    {
        current_node_h = 
            (NK_TableChainNodeHeader*)(
                ((NK_U8*)(current_h) + sizeof(NK_TableChainHeader)) + 
                (
                    sizeof(NK_TableChainNodeHeader) + table->element_size
                ) * index
            );
        if(
            (current_node_h->key != NULL) &&
            (current_node_h->hash != 0)
        )
        {
            /** NOTE: Set this free. */
            NK_AllocatorFree(current_node_h->key);
        }
    }
}

void
NK_TableDestruct(
    NK_Table* table
)
{
    NK_Size index = 0;
    NK_U8* chain = NULL;
    for(
        index;
        index < table->capacity;
        index++
    )
    {
        chain = table->data[index];
        if(chain != NULL)
        {
            P_NK_TableDestructChain(table, chain);
            NK_AllocatorFree(chain);
        }
    }
    NK_AllocatorFree(table->data);
}

NK_Size
NK_TableSize(
    NK_Table* table
)
{
    return table->counter;
}

NK_TableChainNodeHeader*
NK_TableAt(
    NK_Table* table,
    const NK_C8* key
)
{
    /** NOTE: We can enjoy the NK_TableGet ;-) */
    NK_U8* dirty_value = (NK_U8*)(NK_TableGet(table, key));
    return (
        dirty_value == NULL
        ? NULL 
        : 
        (NK_TableChainNodeHeader*)(
                dirty_value - sizeof(NK_TableChainNodeHeader
            )
        )
    );
}

void
NK_TableIterate(
    NK_Table* table,
    const NK_TableIteratorFunction iterator,
    void* userdata
)
{
    NK_U8* chain;
    NK_TableChainHeader* cc_header;
    NK_TableChainNodeHeader* cc_node_header;
    NK_Size counter = 0;
    NK_Size data_index;
    NK_U32 node_index;
    NK_U8* data;

    for(data_index; data_index < table->capacity; data_index++)
    {
        chain = table->data[data_index];
        if(chain == NULL)
        {
            continue;
        }
        /** But we don't have a NULL here ;-) */
        cc_header = (NK_TableChainHeader*)(chain);
        node_index = 0;
        for(node_index; node_index < cc_header->explored; node_index++)
        {
            cc_node_header = 
                (NK_TableChainNodeHeader*)(
                    ((NK_U8*)(chain) + sizeof(NK_TableChainHeader)) + 
                    (
                        node_index *
                        (sizeof(NK_TableChainNodeHeader) + table->element_size)
                    )
                );
            if(
                (cc_node_header->hash == 0) &&
                (cc_node_header->key == NULL)
            )
            {
                continue;
            }
            /** NOTE: We know the data is always after this anyway: */
            data = 
                (NK_U8*)(
                    ((NK_U8*)(cc_node_header) + sizeof(NK_TableChainNodeHeader))
                );
            if(
                !iterator(
                    table,
                    (const NK_C8*)(cc_node_header->key),
                    counter,
                    (void*)data,
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
