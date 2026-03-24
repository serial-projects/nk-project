/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Types/String/Submerged.h"
#include "NK/Internals/Types/StringPool.h"
#include "NK/Internals/Subsystems/GlobalStringPool.h"
#include "NK/Memory.h"

NK_SubmergedString*
NK_SubmergedStringNew()
{
    NK_SubmergedString* new_submerged_string = 
        (NK_SubmergedString*)(NK_AllocatorGet(sizeof(NK_SubmergedString)));
    return new_submerged_string;
}

void
NK_SubmergedStringFree(
    NK_SubmergedString* string
)
{
    NK_AllocatorFree(string);
}

void
NK_SubmergedStringConstruct(
    NK_SubmergedString* string,
    const NK_C8* src
)
{
    string->handler = 
        NK_StringPoolInsertOrFindMatching(
            &NK_GlobalStringPool,
            src
        );
}

void
NK_SubmergedStringDestruct(
    NK_SubmergedString* string
)
{
    NK_StringPoolDegrade(
        &NK_GlobalStringPool,
        string->handler
    );
    string->handler = 0;
}

NK_Size
NK_SubmergedStringSize(
    NK_SubmergedString* string
)
{
    const NK_C8* holding = 
        NK_StringPoolGet(
            &NK_GlobalStringPool,
            string->handler
        );
    return NK_RedirectStrlen(holding);
}

NK_Result
NK_SubmergedStringEqual(
    NK_SubmergedString* string,
    const NK_SubmergedString* with
)
{
    return (string->handler == with->handler);
}

NK_Result
NK_SubmergedStringEqualImmediate(
    NK_SubmergedString* string,
    const NK_C8* with
)
{
    const NK_C8* holding = 
        NK_StringPoolGet(
            &NK_GlobalStringPool,
            string->handler
        );
    return (NK_RedirectStrcmp(holding, with) == 0);
}

void
NK_SubmergedStringSet(
    NK_SubmergedString* string,
    const NK_SubmergedString* with
)
{
    if(string->handler != with->handler)
    {
        /** NOTE: We need to degrade our last one and THEN, up the other: */
        NK_StringPoolDegrade(&NK_GlobalStringPool, string->handler);
        NK_StringPoolConnect(&NK_GlobalStringPool, with->handler);
        
        /** Transfer, since everything went well: */
        string->handler = with->handler;
    }
}

void
NK_SubmergedStringSetImmediate(
    NK_SubmergedString* string,
    const NK_C8* value
)
{
    NK_StringPoolDegrade(&NK_GlobalStringPool, string->handler);
    string->handler =
        NK_StringPoolInsertOrFindMatching(
            &NK_GlobalStringPool,
            value
        );
}

const NK_C8*
NK_SubmergedStringGet(
    NK_SubmergedString* string
)
{
    return NK_StringPoolGet(
        &NK_GlobalStringPool,
        string->handler
    );
}

void
NK_SubmergedStringForget(
    NK_SubmergedString* string
)
{
    NK_StringPoolDegrade(&NK_GlobalStringPool, string->handler);
    string->handler = 0;
}

NK_Result
NK_SubmergedStringIsForgotten(
    NK_SubmergedString* string
)
{
    /** NOTE: In this case, the string is forgotten. */
    return string->handler == 0 ? true : false;
}

NK_U64
NK_SubmergedStringGetHash(
    NK_SubmergedString* string
)
{
    const NK_StringPoolEntry* maybe_handler =
        NK_StringPoolAt(
            &NK_GlobalStringPool,
            string->handler
        );
    return (
        maybe_handler != NULL
        ? maybe_handler->hash
        : 0
    );
}