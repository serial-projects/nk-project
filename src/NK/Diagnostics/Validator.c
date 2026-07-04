/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#include "NK/Diagnostics/Validator.h"
#include "NK/Enums.h"
#include "NK/Memory.h"

#include "NK/Types/Basics.h"
#include "NK/Types/Containers/Vector.h"
#include "NK/Types/String/Dynamic.h"
#include "NK/Core/Panic.h"

/**
 * Include the configuration:
 */
#include "NK/Config.h"

/**
 * Support for `...` parameter:
 */
#include <stdarg.h>

/**
 * `NK_ValidatorContent` related functions:
 */
void
P_NK_ValidatorContentConstruct(
    NK_ValidatorContent* content,
    const NK_Size message_limit
)
{
    /** Create (and initialize) the ring-buffer: */
    NK_Size index = 0;
    NK_ValidatorMessage* message;
    content->buffer = 
        (NK_ValidatorMessage*)(
            NK_AllocatorGet(1 + (sizeof(NK_ValidatorMessage) * message_limit))
        );
    for(index; index < message_limit; index++)
    {
        message = &content->buffer[index];
        NK_DynamicStringConstruct(&message->string);
        /** 
         * NOTE: This guarantee that the string will be at the correct size
         * ready for the message that might arrive in. Making the string act
         * like an buffer is better since we keep the string around for longer.
         */
        NK_DynamicStringResize(
            &message->string,
            NK_CONFIG_VALIDATOR_PUSH_MESSAGE_BUFFER
        );        
    }

    /** Zero and set the content variables: */
    content->buffer_counter = 0;
    content->buffer_limit = message_limit;
    content->code = 0;
    content->userdata = NULL;
}

void
P_NK_ValidatorContentDestruct(
    NK_ValidatorContent* content
)
{
    /** Delete the message ring-buffer: */
    NK_Size index = 0;
    NK_ValidatorMessage* message;
    for(index; index < content->buffer_limit; index++)
    {
        message = &content->buffer[index];
        NK_DynamicStringDestruct(&message->string);
    }
    NK_AllocatorFree(content->buffer);

    /** Zero the variables. */
    content->buffer_counter = 0;
    content->buffer_limit = 0;
    content->code = 0;
    content->userdata = NULL;
}

void
P_NK_ValidatorContentAssignUserdata(
    NK_ValidatorContent* content,
    void* userdata
)
{
    content->userdata = userdata;
}

void
P_NK_ValidatorContentDump( 
    NK_ValidatorContent* content,
    const NK_U8 level,
    const NK_ValidatorListenerCallback callback
)
{
    NK_U64 rewind_amount = 
        (content->buffer_counter > content->buffer_limit)
        ? content->buffer_limit
        : content->buffer_counter;
    NK_U64 index = 0;
    NK_U64 actual_index;
    NK_ValidatorMessage* message;
    for(
        index;
        index < rewind_amount;
        index++
    )
    {
        actual_index = (content->buffer_counter - (rewind_amount - index));
        message = &content->buffer[(actual_index % content->buffer_limit)];
        if(message->flags == level)
        {
            callback(
                content,
                actual_index,
                NK_DynamicStringGetBuffer(&message->string)
            );
        }
    }
}

void
P_NK_ValidatorContentPushMessage(
    NK_ValidatorContent* content,
    const NK_C8* message
)
{
    NK_U64 grab = content->buffer_counter % content->buffer_limit;
    NK_DynamicString* target_string = 
        (NK_DynamicString*)(
            ((NK_U8*)(content->buffer)) + 
            (grab * sizeof(NK_DynamicString))
        );
    NK_DynamicStringClear(target_string);
}

/**
 * `NK_Validator` related functions:
 */

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
    const NK_Size message_limit
)
{
    /** NOTE: Unwrap this loop. */
    NK_U8 index = 0;
    for(
        index;
        index < NK_ENUMS_VALIDATOR_LEVEL_MAX_OPTION;
        index++
    )
    {
        NK_VectorConstruct(
            &validator->sinks[index],
            sizeof(NK_ValidatorListenerCallback)
        );
    }

    /** Now, construct the ring: */
    P_NK_ValidatorContentConstruct(
        &validator->content,
        message_limit
    );
}

void
NK_ValidatorDestruct(
    NK_Validator* validator
)
{
    /** Destruct the sinks: */
    NK_U8 index = 0;
    for(
        index;
        index < NK_ENUMS_VALIDATOR_LEVEL_MAX_OPTION;
        index++
    )
    {
        NK_VectorDestruct(
            &validator->sinks[index]
        );
    }

    /** Now, remove the ring: */
    P_NK_ValidatorContentDestruct(&validator->content);
}

void
NK_ValidatorAssignUserdata(
    NK_Validator* validator,
    void* userdata
)
{
    P_NK_ValidatorContentAssignUserdata(
        &validator->content,
        userdata
    );
}

const NK_U64
NK_ValidatorAddListener(
    NK_Validator* validator,
    const NK_U8 level,
    const NK_ValidatorListenerCallback callback
)
{
    NK_Vector* selected_sink = 
        (level >= NK_ENUMS_VALIDATOR_LEVEL_MAX_OPTION)
        ? NULL
        : &validator->sinks[level];
    NK_U64 added_on;

    /** When NULL on `selected_sink` then, we got an IndexOutOfBounds error. */
    if(selected_sink == NULL)
    {
        NK_Panic(
            "%s: Impossible sink level required: %d",
            NK_CURRENT_WHERE,
            (int)level
        );
    }

    added_on = NK_VectorSize(selected_sink);
    NK_VectorPush(selected_sink, (const void*)(&callback));

    /** Now, we dump everything on the sink: */
    P_NK_ValidatorContentDump(
        &validator->content,
        level,
        callback
    );

    /** Set the return to be the slot we added it: */
    return (const NK_U64)(added_on);
}

void
NK_ValidatorPushMessage(
    NK_Validator* validator,
    const NK_U8 level,
    const NK_C8* format,
    ...
)
{
    /**
     * We need to work directly here:
     */
    NK_U64 grab =
        validator->content.buffer_counter % validator->content.buffer_limit;
    NK_ValidatorMessage* message = &validator->content.buffer[grab];
    NK_C8* dest = NK_DynamicStringGetBuffer(&message->string);

    /* When we be broadcasting everything to the sinks: */
    NK_Vector* selected_sink = 
        (level >= NK_ENUMS_VALIDATOR_LEVEL_MAX_OPTION)
        ? NULL
        : &validator->sinks[level];
    NK_Size sink_index;

    /* `...` parameter preparation `...` */
    va_list args;
    va_start(args, format);

    /** NOTE: Check if the `selected_sink` is NULL. */
    if(selected_sink == NULL)
    {
        NK_Panic(
            "%s: Impossible sink level required: %d",
            NK_CURRENT_WHERE,
            (int)level
        );
    }
    
    /* We use the buffer that is the string ;-) */
    vsnprintf(
        (char*)(dest),
        NK_CONFIG_VALIDATOR_PUSH_MESSAGE_BUFFER,
        format,
        args
    );

    /** Record the type of message we are working with: */
    message->flags = level;

    /** Free this from the stack: */
    va_end(args);

    /** Increment and then broadcast the message to the listeners: */
    validator->content.buffer_counter++;

    /* We got it: */
    sink_index = 0;
    for(
        sink_index;
        sink_index < NK_VectorSize(selected_sink);
        sink_index++
    )
    {
        NK_ValidatorListenerCallback* maybe_callback = 
            (NK_ValidatorListenerCallback*)(
                NK_VectorAt(selected_sink, sink_index)
            );
        if(maybe_callback == NULL)
        {
            continue;
        }
        /** We need to point to the function: */
        (*maybe_callback)(
            &validator->content,
            validator->content.buffer_counter,
            (const NK_C8*)(dest)
        );
    }

    /** Done: */
}