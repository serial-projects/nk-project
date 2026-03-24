/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#include "NK/Types/Containers/Any.h"
#include "NK/Memory.h"
#include "NK/Redirects.h"

NK_Any*
NK_AnyNew()
{
    NK_Any* new_any = 
        (NK_Any*)(
            NK_AllocatorGet(sizeof(NK_Any))
        );
    return new_any;
}

void
NK_AnyFree(
    NK_Any* any
)
{
    NK_AllocatorFree(any);
}

void
NK_AnyConstruct(
    NK_Any* any,
    const NK_Size size,
    const NK_U8 type
)
{
    any->buffer = 
        (NK_U8*)(
            NK_AllocatorGet(sizeof(NK_U8) * size)
        );
    NK_RedirectMemset(any->buffer, 0, sizeof(NK_U8) * size);
    any->size = size;
    any->type = type;
}

void
NK_AnyDestruct(
    NK_Any* any
)
{
    NK_AllocatorFree(any->buffer);
    any->size = 0;
    any->type = 0;
}

void
NK_AnyCopy(
    NK_Any* any,
    const void* source
)
{
    NK_RedirectMemcpy(
        any->buffer,
        source,
        (sizeof(NK_U8) * any->size)
    );
}

const NK_U8
NK_AnyGetType(
    NK_Any* any
)
{
    return any->type;
}

void*
NK_AnyGetImplementation(
    NK_Any* any
)
{
    /** 
     * NOTE: This is passed to the macro to automatically cast if for you, like
     * it is a template, get it?
     */
    return (void*)(any->buffer);
}
