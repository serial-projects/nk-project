/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#include "NK/Types/String/Dynamic.h"
#include "NK/Memory.h"
#include "NK/Config.h"

NK_DynamicString*
NK_DynamicStringNew()
{
    NK_DynamicString* new_dynamic_string = 
        (NK_DynamicString*)(NK_AllocatorGet(
            sizeof(NK_DynamicString)
        ));
    return new_dynamic_string;
}

void
NK_DynamicStringFree(
    NK_DynamicString* string
)
{
    NK_AllocatorFree(string);
}

void
NK_DynamicStringConstruct(
    NK_DynamicString* string
)
{
    string->capacity = NK_CONFIG_STRING_CONSTRUCT_DEFAULT_SIZE;
    string->top = 0;
    /** Create the buffer, the layout is: buffer + end null pointer */
    string->buffer = 
        (NK_C8*)(
            NK_AllocatorGet(
                (sizeof(NK_C8) * string->capacity) + 
                (sizeof(NK_C8) * 1)
            )
        );
    NK_RedirectMemset(
        string->buffer,
        0,
        (
            (sizeof(NK_C8) * string->capacity) + 
            (sizeof(NK_C8) * 1)
        )
    );
}

void
NK_DynamicStringDestruct(
    NK_DynamicString* string
)
{
    /** 
     * NOTE: Safety, we don't want any dynamic data to be left behind since
     * it might contain sensitive data.
     */
    NK_RedirectMemset(
        string->buffer,
        0,
        (
            (sizeof(NK_C8) * string->capacity) + 
            (sizeof(NK_C8) * 1)
        )
    );
    NK_AllocatorFree(string->buffer);
    string->buffer = NULL;
    string->capacity = 0;
    string->top = 0;
}

void
NK_DynamicStringResize(
    NK_DynamicString* string,
    const NK_Size new_size
)
{
    NK_Size size_delta = 0;
    NK_C8* new_buffer = NULL;

    if(NK_SUPPORT_UNLIKELY(new_size == string->capacity))
    {
        goto same_size_ending;
    }

    new_buffer = 
        (NK_C8*)(
            NK_AllocatorResizeBlock(
                string->buffer,
                (
                    (sizeof(NK_C8) * new_size) + 
                    (sizeof(NK_C8) * 1)
                )
            )
        );
    /**
     * NOTE: In case we have upped the capacity, which is always anyway, we
     * clean the dirt that comes with realloc.
     */
    if(new_size > string->capacity)
    {
        size_delta = new_size - string->capacity;
        NK_RedirectMemset(
            (void*)( (NK_C8*)(new_buffer) + string->capacity ),
            0,
            (
                (sizeof(NK_C8) * size_delta) + 
                (sizeof(NK_C8) * 1)
            )
        );
    }
    /** We stick to the top: */
    string->top = new_size > string->capacity ? string->top : new_size;
    string->capacity = new_size;
    string->buffer = new_buffer;

    same_size_ending:
    return;
}

void
NK_DynamicStringPush(
    NK_DynamicString* string,
    const NK_C8 character
)
{
    if(string->top >= string->capacity)
    {
        NK_DynamicStringResize(
            string,
            1 + (string->capacity * 2)
        );
    }
    string->buffer[string->top] = character;
    string->buffer[string->top + 1] = 0;
    string->top++;
}

NK_C8
NK_DynamicStringPop(
    NK_DynamicString* string
)
{
    NK_C8 value = 0;
    if(string->top > 0)
    {
        /** NOTE: We need to decrement before doing anything! */
        string->top--;
        value = string->buffer[string->top];
        string->buffer[string->top] = 0;
    }
    return value;
}

NK_C8
NK_DynamicStringGet(
    NK_DynamicString* string,
    const NK_Size index
)
{
    return (
        index <= string->capacity
        ? string->buffer[index]
        : 0
    );
}
NK_C8*
NK_DynamicStringAt(
    NK_DynamicString* string,
    const NK_Size index
)
{
    return (
        index <= string->capacity
        ? (NK_C8*)(&string->buffer[index])
        : NULL
    );
}

NK_Size
NK_DynamicStringSize(
    NK_DynamicString* string
)
{
    return string->top;
}

NK_C8*
NK_DynamicStringGetBuffer(
    NK_DynamicString* string
)
{
    return string->buffer;
}

NK_Result
NK_DynamicStringEqual(
    NK_DynamicString* string,
    NK_DynamicString* source
)
{
    NK_Result good = true;
    if(NK_DynamicStringSize(string) == NK_DynamicStringSize(source))
    {
        /**
         * NOTE: We will continue to use the `strncmp` since this implementation
         * from the OS might provide things like SSE2 acceleration and other
         * better tricks.
         */
        good =
            (
                NK_RedirectStrncmp(
                    string->buffer,
                    source->buffer,
                    sizeof(NK_C8) * string->top
                ) == 0
            );
    }
    else
    {
        good = false;
    }
    return good;
}

NK_Result
NK_DynamicStringEqualImmediate(
    NK_DynamicString* string,
    const NK_C8* immediate
)
{
    NK_Result good = true;
    if(NK_DynamicStringSize(string) == NK_RedirectStrlen(immediate))
    {
        good = 
            (
                NK_RedirectStrncmp(
                    string->buffer,
                    immediate,
                    (sizeof(NK_C8) * string->top)
                ) == 0
            );
    }
    else
    {
        good = false;
    }
    return good;
}

void
NK_DynamicStringClear(
    NK_DynamicString* string
)
{
    NK_RedirectMemset(
        string->buffer,
        0,
        sizeof(NK_C8) * (string->capacity)
    );
    string->top = 0;
}

void
NK_DynamicStringSetImmediate(
    NK_DynamicString* string,
    const NK_C8* immediate
)
{
    const NK_Size immediate_size = NK_RedirectStrlen(string->buffer);
    if(immediate_size >= string->capacity)
    {
        NK_DynamicStringResize(
            string,
            immediate_size
        );
    }
    NK_RedirectMemcpy( 
        string->buffer,
        immediate,
        sizeof(NK_C8) * (immediate_size + 1)
    );
    string->top = immediate_size;
}

void
NK_DynamicStringDuplicate(
    NK_DynamicString* string,
    NK_DynamicString* base,
    NK_Size start,
    NK_Size end
)
{
    NK_Size base_size = NK_DynamicStringSize(base);
    NK_Size new_size;

    /** Fix the ranges: */
    if(end > base_size)
    {
        end = base_size;
    }

    if(start > end)
    {
        start = end;
    }

    /** Continue: */
    new_size = end - start;
    if((new_size + 1) >= string->capacity)
    {
        NK_DynamicStringResize(
            string,
            (new_size + 1)
        );
    }

    NK_RedirectMemcpy(
        string->buffer,
        (NK_U8*)(base->buffer) + start,
        (new_size + 1)
    );
    
    string->top = new_size;
}

/** 
 * This function will include stdio.h, but this is only present on the debug
 * version of `NK`, on the future, this functions will be hidden for good.
 */
#include <stdio.h>

void
NK_DynamicStringDebug(
    NK_DynamicString* string
)
{
    NK_Size index=0;
    printf(
        "%s: string->top = %d, string->capacity = %d, buffer = %p\n",
        NK_CURRENT_WHERE,
        string->top,
        string->capacity,
        string->buffer
    );
    for(
        index;
        index <= string->capacity;
        index++
    )
    {
        printf(
            "%s: [index = %d] %c/%d ",
            NK_CURRENT_WHERE,
            index,
            (NK_C8)(string->buffer[index]),
            (NK_S32)(string->buffer[index])
        );
        if(index == string->top)
        {
            printf("[TOP] ");
        }
        if(index == string->capacity)
        {
            printf("[CAPACITY]");
        }
        printf("\n");
    }
}