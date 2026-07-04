/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Internals/Types/Table.h"
#include "NK/Types/Basics.h"
#include "NK/Internals/Hash.h"
#include "NK/Memory.h"
#include "NK/Config.h"

void*
NK_TableGet(
    NK_Table* table,
    const NK_C8* key
)
{
    void* got = NULL;
    NK_U64 hashed_value = 0;
    NK_U64 look_at_chain = 0;
    NK_U8* using_chain = NULL;
    NK_TableChainHeader* cc_header = NULL;
    NK_TableChainNodeHeader* cc_node_header = NULL;
    NK_U32 index = 0;
    
    /** Hash the key: */
    hashed_value = NK_Hash(key);
    look_at_chain = hashed_value % table->capacity;
    using_chain = table->data[look_at_chain];

    /**
     * Scenario I:
     * ---
     * 
     * In this scenario, we didn't find an `table->data` that was initialized
     * for the `key`, which means it DOESN'T exist.
     */
    if (using_chain == NULL)
    {
        goto end;
    }

    /**
     * Scenario II:
     * ---
     * 
     * In this scenario, the `chain` exists, and because of this, we need to
     * iterate on it to find, iterate on the `explored`.
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
                (cc_node_header->hash == hashed_value) && 
                (NK_RedirectStrcmp(cc_node_header->key, key))
            )
        )
        {
            got = 
                (NK_U8*)(
                    (NK_U8*)(cc_node_header) + 
                    sizeof(NK_TableChainNodeHeader)
                );
            goto end;
        }
    }

    end:
    return got;
}
