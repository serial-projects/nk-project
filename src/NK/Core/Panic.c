/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#include "NK/Core/Panic.h"
#include "NK/Config.h"

/**
 * NOTE: This is the only file this is even allowed.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void NK_Panic(
    const NK_C8* format,
    ...
)
{
    NK_C8 buffer[NK_CONFIG_PANIC_BUFFER_SIZE];
    
    va_list args;
    va_start(args, format);

    vsnprintf((char*)&buffer, NK_CONFIG_PANIC_BUFFER_SIZE, format, args);

    va_end(args);

    /** Print & Abort() */
    printf("panic: %s\n", buffer);
    abort();
}