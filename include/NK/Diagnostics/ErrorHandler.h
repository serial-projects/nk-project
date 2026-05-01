/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKDiagnosticsErrorHandler_h
#define NKDiagnosticsErrorHandler_h

#include "NK/Types.h"

/**
 * @brief Might hold an error.
 * 
 * @warning You must ALWAYS `Destruct` this `NK_ErrorHandler` even when no error
 * was actually handled.
 */
typedef struct NK_ErrorHandler
{
    /**
     * @brief Contains an copy of the log.
     */
    NK_SubmergedString log;

    /**
     * @brief Contains an code for the type of error.
     * 
     * @warning This error is intentionally big, why? Simple, because in C
     * we have to worry about the geometry of the structs, using an code with
     * an `NK_U8` would be good, but the compiler likes packages of data for the
     * CPU.
     */
    NK_U64 code;
} NK_ErrorHandler;

/**
 * @brief Creates an new `NK_ErrorHandler`
 */
NK_ErrorHandler*
NK_ErrorHandlerNew();

/**
 * @brief Frees an `NK_ErrorHandler` from the heap.
 */
void
NK_ErrorHandlerFree(
    NK_ErrorHandler* handler
);

/**
 * @brief Constructs the `NK_ErrorHandler`
 */
void
NK_ErrorHandlerConstruct(
    NK_ErrorHandler* handler
);

/**
 * @brief Destructs the `NK_ErrorHandler`
 */
void
NK_ErrorHandlerDestruct(
    NK_ErrorHandler* handler
);

/**
 * @brief Assign an error for the error handler.
 */
void
NK_ErrorHandlerHold(
    NK_ErrorHandler* handler,
    const NK_U64 code,
    const NK_C8* format,
    ...
);

#endif