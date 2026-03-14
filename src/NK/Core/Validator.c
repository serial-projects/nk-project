/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#include "NK/Core/Validator.h"
#include "NK/Types/Basics.h"
#include "NK/Core/Memory.h"
#include "NK/Types/Containers/Vector.h"
#include "NK/Types/String/Dynamic.h"

NK_Validator*
NK_ValidatorNew()
{
    NK_Validator* new_validator = 
        (NK_Validator*)(
            NK_AllocatorGet(sizeof(NK_Validator))
        );
    return new_validator;
}

void
NK_ValidatorFree(
    NK_Validator* validator
)
{
    NK_AllocatorFree(validator);
}

void
NK_ValidatorConstruct(
    NK_Validator* validator,
    const NK_Size buffer_limit
)
{
    NK_Size index = 0;

    validator->content.buffer_index = 0;
    validator->content.buffer_limit = buffer_limit;
    
    /** Initialize the listeners first, it is simpler: */
    NK_VectorConstruct(&validator->listeners, sizeof(NK_ValidatorListener));

    /** Initialize the beast: */
    validator->content.buffer = 
        (NK_DynamicString*)(
            NK_AllocatorGet(
                (buffer_limit * sizeof(NK_DynamicString))
            )
        );
    for(index; index < buffer_limit; index++)
    {
        NK_DynamicStringConstruct(
            (NK_DynamicString*)(
                ((NK_U8*)(validator->content.buffer)) +
                (
                    index *
                    sizeof(NK_DynamicString)
                )
            )
        );
    }
}

void
NK_ValidatorDestruct(
    NK_Validator* validator
)
{
    NK_Size index = 0;
    for(index; index < validator->content.buffer_limit; index++)
    {
        NK_DynamicStringDestruct(
            (NK_DynamicString*)(
                ((NK_U8*)(validator->content.buffer)) + 
                (
                    index *
                    sizeof(NK_DynamicString)
                )
            )
        );
    }
    NK_AllocatorFree(validator->content.buffer);
    validator->content.buffer = NULL;

    /** Destroy the list of listeners: */
    NK_VectorDestruct(&validator->listeners);
    validator->content.buffer_index = 0;
    validator->content.buffer_limit = 0;
}