/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Internals/Types/Table.h"
#include "NK/Types/Basics.h"
#include "NK/Internals/Hash.h"
#include "NK/Memory.h"
#include "NK/Config.h"

NK_Result
NK_TableRemove(
    NK_Table* table,
    const NK_C8* key
)
{
    NK_Result deleted = false;
    NK_U64 key_hashed;
    NK_U64 look_at_chain; 
    NK_U8* using_chain;
    NK_TableChainHeader* cc_header;
    NK_TableChainNodeHeader* cc_node_header;
    NK_U32 index = 0;
    NK_U8* element_region = NULL;

    /** We do pretty much the same, hash the key, get the chain, etc. */
    key_hashed = NK_Hash(key);
    look_at_chain = key_hashed % table->capacity;
    using_chain = table->data[look_at_chain];
    
    /**
     * Scenario I:
     * ---
     * 
     * In this case, the chain does not exist, and thus, we can't delete
     * anything.
     */
    if(using_chain == NULL)
    {
        goto end;
    }
    
    /**
     * Scenario II:
     * ---
     * 
     * In this case, the chain is real and we can iterate to find, possibly,
     * the node.
     */
    cc_header = (NK_TableChainHeader*)(using_chain);
    index = 0;
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
            (cc_node_header->hash != 0) &&
            (
                cc_node_header->hash == key_hashed &&
                (NK_RedirectStrcmp(cc_node_header->key, key) == 0)
            )
        )
        {
            /** We actually erase the data. */
            NK_AllocatorFree(
                cc_node_header->key
            );
            cc_node_header->key = NULL;
            cc_node_header->hash = 0;
            
            /** Erase the data: */
            element_region =
                (NK_U8*)(cc_node_header) + sizeof(NK_TableChainNodeHeader);
            NK_RedirectMemset(
                element_region,
                0,
                table->element_size
            );

            /** By the end of here, we did delete something. */
            deleted = true;
            goto end;
        }
    }

    end:
    return deleted;
}