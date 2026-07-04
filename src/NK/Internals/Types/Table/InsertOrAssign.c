/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Internals/Types/Table.h"
#include "NK/Types/Basics.h"
#include "NK/Internals/Hash.h"
#include "NK/Memory.h"
#include "NK/Config.h"


NK_TableChainNodeHeader*
NK_TableInsertOrAssign(
    NK_Table* table,
    const NK_C8* key,
    void* src
)
{
    /** In the first moment, we have this variables: */
    NK_Size key_length;
    NK_U64 key_hashed;
    NK_U64 look_at_chain;
    NK_U8* using_chain;

    /** In the second (and the first), when we iterate: */
    NK_TableChainHeader* cc_header = NULL;
    NK_TableChainNodeHeader* cc_node_header = NULL;
    NK_U8* element_region = NULL;
    NK_U32 index = 0;
    NK_TableChainNodeHeader* empty_spot_found = NULL;

    /** In the third (and last), when we got to resize the chain: */
    NK_U8* resized_chain = NULL;
    NK_U64 calculated_resize;

    /** We need to hash the value: */
    key_length = NK_RedirectStrlen(key);
    key_hashed = NK_Hash(key);
    look_at_chain = key_hashed % table->capacity;
    using_chain = table->data[look_at_chain];

    /**
     * Scenario I: The chain does not exist.
     * ---
     * 
     * In this scenario, our chain doesn't exist and our object an simply
     * (trivially) be inserted, because, it will be always object n = 0;
     */
    if(using_chain == NULL)
    {
        /** NOTE: We allocate following our layout. */
        using_chain = 
            (NK_U8*)(
                NK_AllocatorGet(
                    sizeof(NK_TableChainHeader) + 
                    (
                        NK_CONFIG_TABLE_CHAIN_DEFAULT_SIZE *
                        (sizeof(NK_TableChainNodeHeader) + table->element_size)
                    )
                )
            );
        cc_header =
            (NK_TableChainHeader*)(
                using_chain
            );
        cc_header->capacity = NK_CONFIG_TABLE_CHAIN_DEFAULT_SIZE;
        cc_header->explored = 1;
        
        /** Record the element on the first index: */
        cc_node_header =
            (NK_TableChainNodeHeader*)(
                using_chain + sizeof(NK_TableChainHeader)
            );
        cc_node_header->hash = key_hashed;
        cc_node_header->key = 
            (NK_C8*)(
                NK_AllocatorGet(
                    sizeof(NK_C8) * (key_length + 1)
                )
            );
        element_region = (
            using_chain + 
            sizeof(NK_TableChainHeader) + 
            sizeof(NK_TableChainNodeHeader)
        );
        /**
         * Begin copying, we start by the name and then, at last, the element.
         */
        NK_RedirectMemcpy(
            cc_node_header->key,
            key,
            sizeof(NK_C8) * key_length
        );
        NK_RedirectMemcpy(
            element_region,
            src,
            table->element_size
        );

        /**
         * Record it on the chains and continue.
         */
        table->data[look_at_chain] = using_chain;

        /** Return the `cc_node_header` */
        goto non_assignment_endings;
    }

    /**
     * Scenario II: The chain does exist.
     * ---
     * 
     * In this scenario, our chain is real, it exists, then, what we do is
     * find an empty spot or a spot that MIGHT contain our key, if that is
     * the case, if we find a duplicated `key`, then, we can just `Assign` the
     * value instead of deleting it.
     */
    index = 0;
    cc_header = (NK_TableChainHeader*)(using_chain);
    for(
        index;
        index < cc_header->explored;
        index++
    )
    {
        cc_node_header = 
            (NK_TableChainNodeHeader*)(
                using_chain + sizeof(NK_TableChainHeader) + 
                (
                    index *
                    (sizeof(NK_TableChainNodeHeader) + table->element_size)
                )
            );
        if(
            (cc_node_header->hash == 0 && cc_node_header->key == NULL) &&
            empty_spot_found == NULL
        )
        {
            empty_spot_found = cc_node_header;
        }

        /** Check for `Assignment` here, an hint is the hash to be the same. */
        if(
            (cc_node_header->hash == key_hashed) &&
            (NK_RedirectStrcmp(cc_node_header->key, key) == 0)
        )
        {
            /** We just update the region. */
            element_region = (
                (NK_U8*)(cc_node_header) + 
                sizeof(NK_TableChainNodeHeader)
            );
            NK_RedirectMemcpy(
                element_region,
                src,
                table->element_size
            );

            goto assignment_ending;
        }
    }

    /**
     * Scenario III:
     * ---
     * 
     * In this scenario, despite having iterated the list, we didn't do any
     * assignment, but we have found an empty spot, that means something was
     * deleted and we have a spot for our element.
     * 
     * NOTE: This is very unlikely, though, except on a table where you are
     * always inserting and deleting things with hashes who are pretty close
     * to each other.
     */
    if(empty_spot_found != NULL)
    {
        empty_spot_found->hash = key_hashed;
        empty_spot_found->key = 
            (NK_C8*)(
                NK_AllocatorGet(
                    sizeof(NK_C8) * (key_length + 1)
                )
            );
        element_region = 
            (
                (NK_U8*)(empty_spot_found) +
                sizeof(NK_TableChainNodeHeader)
            );
        /** Copy Data: */
        NK_RedirectMemcpy(
            empty_spot_found->key,
            key,
            (sizeof(NK_C8) * key_length)
        );
        NK_RedirectMemcpy(
            element_region,
            src,
            table->element_size
        );

        cc_node_header = empty_spot_found;
        goto non_assignment_endings;
    }

    /**
     * Scenario IV:
     * ---
     * 
     * In this scenario, we have that, we have iterated on the entire list,
     * and found no empty spot, which means we have to touch on the `explored`,
     * in case `explored` is bigger than `capacity`, we will resize the chain.
     */
    if( cc_header->explored >= cc_header->capacity )
    {
        calculated_resize = (cc_header->capacity * 2);
        resized_chain = 
            (NK_U8*)(
                NK_AllocatorResizeBlock(
                    /** 
                     * NOTE: This will make table->data[] reference unsafe, we
                     * NEED to update it after the resize.
                     */
                    using_chain,
                    (
                        sizeof(NK_TableChainHeader) +
                        (
                            calculated_resize *
                            (
                                sizeof(NK_TableChainNodeHeader) +
                                table->element_size
                            )
                        )
                    )
                )
            );
        /** Like we said, update the values: */
        table->data[look_at_chain] = resized_chain;
        using_chain = resized_chain;

        /** Update the capacity on the node itself: */
        cc_header = (NK_TableChainHeader*)(using_chain);
        cc_header->capacity = calculated_resize;
    }

    /** Append on the list (it is guaranteed to fit) */
    cc_node_header = 
        (NK_TableChainNodeHeader*)(
            using_chain + sizeof(NK_TableChainHeader) +
            (
                cc_header->explored *
                (sizeof(NK_TableChainNodeHeader) + table->element_size)
            )
        );
    cc_node_header->hash = key_hashed;
    cc_node_header->key = 
        (NK_C8*)(
            NK_AllocatorGet(
                sizeof(NK_C8) * (key_length + 1)
            )
        );
    element_region =
        (NK_U8*)(
            (NK_U8*)(cc_node_header) +
            sizeof(NK_TableChainNodeHeader)
        );

    /**
     * Copy the data:
     */
    NK_RedirectMemcpy(
        cc_node_header->key,
        key,
        (sizeof(NK_C8) * key_length)
    );
    NK_RedirectMemcpy(
        element_region,
        src,
        table->element_size
    );

    /**
     * We increment the explored amount
     */
    cc_header->explored++;

    /**
     * Final V:
     * ---
     * 
     * At the end, it is guaranteed that for any scenario, we have appended 
     * on the list. Although, for most scenarios, we are going to increment
     * the counter `table->counter`, except on assignments (which isn't
     * adding an element to the `NK_Table`).
     */
    non_assignment_endings:
    table->counter++;

    assignment_ending:
    return cc_node_header;
}
