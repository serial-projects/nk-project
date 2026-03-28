/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#include "NK/Types/Containers/Any.h"
#include "NK/Memory.h"
#include "NK/Redirects.h"
#include "NK/Core/Panic.h"

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
    NK_Any* any
)
{
    /**
     * NOTE: some types might not require any saving, like NULL on `3xJson`,
     * where NULL is just an certain type.
     */
    any->buffer = NULL;
    any->size = 0;
    any->type = 0;
}

void
NK_AnyDestruct(
    NK_Any* any
)
{
    /**
     * NOTE: We handle this behavior of NULL types.
     */
    if(any->buffer != NULL)
    {
        NK_AllocatorFree(any->buffer);
        any->buffer = NULL;
    }
    any->size = 0;
    any->type = 0;
}

void
NK_AnyCopy(
    NK_Any* any,
    const void* source,
    const NK_Size size
)
{
    /**
     * NOTE: When the buffer is not initialized, then we initialize
     * a new one.
     */
    if(NK_SUPPORT_LIKELY(any->buffer == NULL))
    {
        any->size = size;
        any->buffer =
            (NK_U8*)(
                NK_AllocatorGet(sizeof(NK_U8) * size)
            );
    }
    else
    {
        /**
         * TODO: You can't do this, once you set the buffer, you
         * should not be able to change. This might change on the
         * future, but right now, it is not allowed (or won't ever).
         */
        NK_Panic(
            "%s: Attempt to modify buffer of NK_Any [%p]",
            NK_CURRENT_WHERE,
            (void*)(any)
        );
    }
    NK_RedirectMemcpy(
        any->buffer,
        source,
        (sizeof(NK_U8) * any->size)
    );
}

void
NK_AnySetType(
    NK_Any* any,
    const NK_U8 type
)
{
    any->type = type;
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
    /** NOTE: Panic when getting an buffer that is empty. */
    if(NK_SUPPORT_UNLIKELY(any->buffer == NULL))
    {
        NK_Panic(
            "%s: Attempt to NK_AnyGet on NK_Any [%p] with buffer as NULL",
            NK_CURRENT_WHERE,
            any
        );

        /** Bad. */
        return NULL;
    }
    else
    {
        return (void*)(any->buffer);
    }
}
