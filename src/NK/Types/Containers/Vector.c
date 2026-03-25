/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Types/Containers/Vector.h"
#include "NK/Memory.h"
#include "NK/Config.h"

/** Basic New/Free, Construct/Destruct: */

NK_Vector*
NK_VectorNew()
{
    NK_Vector* new_vector;
    new_vector = (NK_Vector*)( NK_AllocatorGet(sizeof(NK_Vector)) );
    return new_vector;
}

void
NK_VectorFree(
    NK_Vector* vector
)
{
    NK_AllocatorFree(vector);
}

void
NK_VectorConstruct(
    NK_Vector* vector,
    const NK_Size element_size
)
{
    vector->element_size = element_size;
    vector->top = 0;
    vector->capacity = NK_CONFIG_VECTOR_CONSTRUCT_DEFAULT_SIZE;
    vector->data = 
        (NK_U8*)(
            NK_AllocatorGet(
                sizeof(NK_U8) * 
                (
                    element_size *
                    NK_CONFIG_VECTOR_CONSTRUCT_DEFAULT_SIZE
                )
            )
        );
}

void
NK_VectorDestruct(
    NK_Vector* vector
)
{
    /** Set everything to 0. */
    vector->element_size = 0;
    vector->top = 0;
    vector->capacity = 0;

    /** Did we clean the buffer? */
    if(vector->data != NULL)
    {
        NK_AllocatorFree(vector->data);
        vector->data = NULL;
    }
}

/** Resize, Push, Pop and At (basic functions): */

void
NK_VectorResize(
    NK_Vector* vector,
    const NK_Size new_size
)
{
    NK_U8* new_buffer;
    new_buffer = 
        (NK_U8*)(
            NK_AllocatorResizeBlock(
                vector->data,
                (vector->element_size * new_size)
            )
        );
    vector->top = vector->top > new_size ? new_size : vector->top;
    vector->capacity = new_size;
    vector->data = new_buffer;
}

void
NK_VectorPush(
    NK_Vector* vector,
    const void* data
)
{
    NK_U8* current_top;

    if(vector->top >= vector->capacity)
    {
        NK_VectorResize(
            vector,
            1 + (vector->capacity * 2)
        );
    }

    /** Get the address and get ready to write: */
    current_top = (NK_U8*)(vector->data) + (vector->top * vector->element_size);
    NK_RedirectMemcpy(current_top, data, sizeof(NK_U8) * vector->element_size);
    vector->top++;
}

NK_Result
NK_VectorPop(
    NK_Vector* vector,
    void* dst
)
{
    NK_Result good = true;
    NK_U8* current_top;

    /** Can we pop? */
    if(vector->top > 0)
    {
        vector->top--;
        current_top = vector->data + (vector->top * vector->element_size);
        if(NK_SUPPORT_LIKELY(dst != NULL))
        {
            NK_RedirectMemcpy(
                dst,
                current_top,
                sizeof(NK_U8) * vector->element_size
            );
        }
        NK_RedirectMemset(current_top, 0, sizeof(NK_U8) * vector->element_size);
    }
    else
    {
        good = false;
    }

    return good;
}

void*
NK_VectorAt(
    NK_Vector* vector,
    const NK_Size index
)
{
    return (void*)(
        index < vector->top 
            ? vector->data + (vector->element_size * index)
            : NULL
    );
}

NK_Size
NK_VectorSize(
    NK_Vector* vector
)
{
    return vector->top;
}

/** Functional: */

void
NK_VectorIterate(
    NK_Vector* vector,
    NK_VectorIteratorFunction iterator,
    void* userdata
)
{
    NK_Size index = 0;
    for(index; index < vector->top; index++)
    {
        if(
            !iterator(
                vector,
                index,
                (void*)(
                    vector->data +
                    (index * (sizeof(NK_U8) * vector->element_size))
                ),
                (void*)(userdata)
            )
        )
        {
            break;
        }
    }
}