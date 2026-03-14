/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Internals/Types/Table.h"
#include "NK/Types/Basics.h"
#include "NK/Core/Memory/Allocator.h"
#include "NK/Internals/Hash.h"
#include "NK/Config.h"

/** New, Free, Construct and Destruct functions: */

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

    /** We need to zero this since we will be holding pointers. */
    table->data = (NK_U8**)(NK_AllocatorGet(sizeof(NK_U8*) * (capacity + 1)));
    NK_RedirectMemset(table->data, 0, sizeof(NK_U8*) * (capacity + 1));
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
    NK_TableChainNodeHeader* current_node_h;
    /** Begin iterating : */
    for(index; index < current_h->explored; index++)
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
    NK_U8* chain;
    for(index; index < table->capacity; index++)
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

/** Table Operations: */

NK_Size
NK_TableSize(
    NK_Table* table
)
{
    return table->counter;
}

NK_TableChainNodeHeader*
NK_TableInsertOrAssign(
    NK_Table* table,
    const NK_U8* key,
    void* src
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

    /** Hash the value: */
    NK_Size key_length = NK_RedirectStrlen(key);
    NK_U64 hashed_value = NK_Hash(key);
    NK_U64 look_chain = hashed_value % table->capacity;
    NK_U8* working_chain = table->data[look_chain];

    NK_U8* new_chain;
    NK_TableChainHeader new_chain_header;
    new_chain_header.capacity = NK_CONFIG_INTERNALS_TABLE_CHAIN_DEFAULT_SIZE;
    new_chain_header.explored = 0;

    /** NOTE: CC = Current Chain. */
    NK_TableChainHeader* cc_header;

    /** For indexing: */
    NK_TableChainNodeHeader* empty_spot;
    NK_TableChainNodeHeader* cc_node_header;
    NK_U32 index;
    NK_U8* data;

    /** When reallocating: */
    NK_U32 new_capacity;

    /** Test for the value: */
    if(working_chain == NULL)
    {
        new_chain = 
            (NK_U8*)(
                NK_AllocatorGet(
                    sizeof(NK_TableChainHeader) + 
                    (
                        new_chain_header.capacity *
                        (sizeof(NK_TableChainNodeHeader) + table->element_size)
                    )
                )
            );

        /** Record on the chain the header: */
        cc_header = (NK_TableChainHeader*)(new_chain);
        *cc_header = new_chain_header;

        /** Give the algorithm this new chain. */
        table->data[look_chain] = new_chain;
        working_chain = new_chain;
    }

    /** Get the header: */
    cc_header = (NK_TableChainHeader*)(working_chain);
    cc_node_header = NULL;
    empty_spot = NULL;
    index = 0;
    for(index; index < cc_header->explored; index++)
    {
        cc_node_header =
            (NK_TableChainNodeHeader*)(
                ((NK_U8*)(working_chain) + sizeof(NK_TableChainHeader)) + 
                (
                    index *
                    (sizeof(NK_TableChainNodeHeader) + table->element_size)
                )
            );
        if(
            (cc_node_header->hash == hashed_value) &&
            (NK_RedirectStrcmp(cc_node_header->key, key) == 0)
        )
        {
            /** NOTE: In this case, we got an basic assignment. */
            data = 
                (NK_U8*)(
                    (NK_U8*)(cc_node_header) +
                    sizeof(NK_TableChainNodeHeader)
                );
            NK_RedirectMemcpy(
                (void*)(data),
                (const void*)(src),
                table->element_size
            );
            goto assignment_ending;
        }
        if(
            (empty_spot == NULL) &&
            (
                (cc_node_header->hash == 0) &&
                (cc_node_header->key == NULL)
            )
        )
        {
            empty_spot = cc_node_header;
        }
    }

    /** Did we find an valid empty spot? */
    if(empty_spot != NULL)
    {
        cc_node_header = empty_spot;
        cc_node_header->hash = hashed_value;
        cc_node_header->key = 
            (NK_C8*)(
                NK_AllocatorGet(sizeof(NK_C8) * (key_length + 1))
            );
        NK_RedirectMemcpy(
            cc_node_header->key,
            key,
            sizeof(NK_C8) * (key_length + 1)
        );
        data =
            (NK_U8*)(
                (NK_U8*)(cc_node_header) + sizeof(NK_TableChainNodeHeader)
            );
        NK_RedirectMemcpy(
            (void*)(data),
            (const void*)(src),
            table->element_size
        );
        goto found_empty_spot_ending;
    }

    /** Fine, we found nothing, so we have to append on the list then: */
    if(cc_header->explored >= cc_header->capacity)
    {
        new_capacity = (cc_header->capacity * 2);
        new_chain = 
            (NK_U8*)(
                NK_AllocatorResizeBlock(
                    working_chain,
                    sizeof(NK_TableChainHeader) +
                    (
                        new_capacity *
                        (sizeof(NK_TableChainNodeHeader) + table->element_size)
                    )
                )
            );
        /** Switch and store the resized chain: */
        table->data[look_chain] = new_chain;
        working_chain = new_chain;

        /** Restore the pointers again: */
        cc_header = (NK_TableChainHeader*)(new_chain);
        cc_header->capacity = new_capacity;
    }

    /** With the capacity expanded, we can continue our endeavors ;-) */
    cc_node_header = 
        (NK_TableChainNodeHeader*)(
            ((NK_U8*)(working_chain) + sizeof(NK_TableChainHeader)) + 
            (
                cc_header->explored *
                (sizeof(NK_TableChainNodeHeader) + table->element_size)
            )
        );
    cc_node_header->hash = hashed_value;
    cc_node_header->key =
        (NK_U8*)(
            NK_AllocatorGet(sizeof(NK_C8) * (key_length + 1))
        );
    NK_RedirectMemcpy(
        cc_node_header->key,
        key,
        sizeof(NK_C8) * (key_length + 1)
    );
    data = 
        (NK_U8*)(
            ((NK_U8*)(working_chain) + sizeof(NK_TableChainHeader)) + 
            (
                cc_header->explored *
                (sizeof(NK_TableChainNodeHeader) + table->element_size)
            ) + 
            sizeof(NK_TableChainNodeHeader)
        );
    NK_RedirectMemcpy(
        data,
        src,
        table->element_size
    );

    /** We increment the capacity here! */
    cc_header->explored++;
    
found_empty_spot_ending:
    table->counter++;

    /** NOTE: During an assignment, we don't have an table->counter++ */
assignment_ending:
    return cc_node_header;
}

void*
NK_TableGet(
    NK_Table* table,
    const NK_U8* key
)
{
    void* got = NULL;
    NK_U64 hashed_value = NK_Hash(key);
    NK_U64 look_chain = hashed_value % table->capacity;
    NK_U8* working_chain = table->data[look_chain];
    NK_TableChainHeader* cc_header;
    NK_TableChainNodeHeader* cc_node_header;
    NK_U32 index = 0;

    /** In this case, the chain is NULL. */
    if(working_chain == NULL)
    {
        goto empty_chain_ending;
    }
    
    /** Iterate: */
    cc_header = (NK_TableChainHeader*)(working_chain);
    for(index; index < cc_header->explored; index++)
    {
        cc_node_header = 
            (NK_TableChainNodeHeader*)(
                (NK_U8*)(working_chain) + 
                sizeof(NK_TableChainHeader) + 
                (
                    index *
                    (sizeof(NK_TableChainNodeHeader) + table->element_size)
                )
            );
        if(
            (cc_node_header->hash == hashed_value) &&
            (NK_RedirectStrcmp(cc_node_header->key, key) == 0)
        )
        {
            got =
                (NK_U8*)(
                    (NK_U8*)(cc_node_header) +
                    sizeof(NK_TableChainNodeHeader)
                );
            break;
        }
    }
empty_chain_ending:
    return got;
}

NK_TableChainNodeHeader*
NK_TableAt(
    NK_Table* table,
    const NK_U8* key
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

/**
 * NOTE: This NK_TableRemove must NOT compare after NK_AllocatorFree() because
 * many functions use NK_TableHeader::key to remove the element itself! So,
 * always implement using an way that doesn't use the key parameter after
 * deleting or NK_AllocateFree(NK_TableHeader::key)!
 */
NK_Result
NK_TableRemove(
    NK_Table* table,
    const NK_U8* key
)
{
    NK_Result got = false;
    NK_U64 hashed_value = NK_Hash(key);
    NK_U64 look_chain = hashed_value % table->capacity;
    NK_U8* working_chain = table->data[look_chain];
    NK_TableChainHeader* cc_header;
    NK_TableChainNodeHeader* cc_node_header;
    NK_U32 index = 0;
    
    /** Set the chain here: */
    if(working_chain == NULL)
    {
        goto empty_chain_ending;
    }

    cc_header = (NK_TableChainHeader*)(working_chain);
    for(index; index < cc_header->explored; index++)
    {
        cc_node_header = 
            (NK_TableChainNodeHeader*)(
                (NK_U8*)(working_chain) + 
                sizeof(NK_TableChainHeader) + 
                (
                    index *
                    (sizeof(NK_TableChainNodeHeader) + table->element_size)
                )
            );
        if(
            (
                (cc_node_header->hash == hashed_value) &&
                (NK_RedirectStrcmp(cc_node_header->key, key) == 0)
            )
        )
        {
            NK_AllocatorFree(cc_node_header->key);
            cc_node_header->key = NULL;
            cc_node_header->hash = 0;
            table->counter--;
            got = true;
            break;
        }
    }

empty_chain_ending:
    return got;
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
                    (const NK_U8*)(cc_node_header->key),
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
