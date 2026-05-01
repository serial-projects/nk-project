/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Diagnostics/ErrorHandler.h"
#include "NK/Memory.h"

/** NOTE: We will need vsnprintf() */
#include <stdarg.h>
#include <stdio.h>

NK_ErrorHandler*
NK_ErrorHandlerNew()
{
    NK_ErrorHandler* new_handler = 
        (NK_ErrorHandler*)(
            NK_AllocatorGet(sizeof(NK_ErrorHandler))
        );
    return new_handler;
}

void
NK_ErrorHandlerFree(
    NK_ErrorHandler* handler
)
{
    NK_AllocatorFree(handler);
}

void
NK_ErrorHandlerConstruct(
    NK_ErrorHandler* handler
)
{
    NK_SubmergedStringConstruct(&handler->log, NULL);
    handler->code = 0;
}


void
NK_ErrorHandlerDestruct(
    NK_ErrorHandler* handler
)
{
    NK_SubmergedStringDestruct(&handler->log);
    handler->code = 0;
}

void
NK_ErrorHandlerHold(
    NK_ErrorHandler* handler,
    const NK_U64 code,
    const NK_C8* format,
    ...
)
{
    NK_C8 rtb[NK_CONFIG_ERROR_HANDLER_BUFFER_SIZE] = {0};
    va_list args;

    /** Starts the list: */
    va_start(args, format);
    vsnprintf(
        (NK_C8*)(&rtb),
        NK_CONFIG_ERROR_HANDLER_BUFFER_SIZE,
        format,
        args
    );
    va_end(args);

    /** Set the list down: */
    NK_SubmergedStringSetImmediate(&handler->log, (const NK_C8*)(&rtb));
    handler->code = code;
}