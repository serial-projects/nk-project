/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKTypesContainersAny_h
#define NKTypesContainersAny_h

#include "NK/Types/Basics.h"

/**
 * @brief NK_Any can hold any value of any type and it can be checked runtime.
 */
typedef struct NK_Any
{
    /**
     * @brief This is where the element lives in. It can hold anything and you
     * can easily iterate the type.
     */
    NK_U8* buffer;

    /**
     * @brief This says the size of the buffer.
     */
    NK_Size size;

    /**
     * @brief This says the type of the `NK_Any`, the object is to have a formal
     * way to create element variance.
     */
    NK_U8 type;
} NK_Any;

/**
 * @brief Create an `NK_Any`
 */
NK_Any*
NK_AnyNew();

/**
 * @brief Frees an `NK_Any` created by the `NK_AnyNew`
 */
void
NK_AnyFree(
    NK_Any* any
);

/**
 * @brief Constructs an `NK_Any` object.
 */
void
NK_AnyConstruct(
    NK_Any* any,
    const NK_Size size,
    const NK_U8 type
);

/**
 * @brief Destroys an `NK_Any` object.
 * 
 * @warning Does not imply in freeing an object that is inside the buffer, this
 * function simply frees the buffer and resets the `NK_Any` object.
 */
void
NK_AnyDestruct(
    NK_Any* any
);

/**
 * @brief Copy an element to be inside the buffer.
 */
void
NK_AnyCopy(
    NK_Any* any,
    const void* source
);

/**
 * @brief Get type of the `NK_Any`
 */
const NK_U8
NK_AnyGetType(
    NK_Any* any
);

/**
 * @brief This gets the buffer that you can cast to the type you so want.
 */
void*
NK_AnyGetImplementation(
    NK_Any* any
);

/**
 * @brief This automatically casts the type to whatever you want.
 */
#define NK_AnyGet(type, any) ((type)NK_AnyGetImplementation(any))

#endif