/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKTypesContainersVector_h
#define NKTypesContainersVector_h

#include "NK/Types/Basics.h"

/**
 * @brief Sequential Access List.
 * 
 * @warning Do not modify the `NK_Vector` yourself, use the functions.
 * 
 * @note `NK_Vector` will always have a '\0's on the end.
 * @note Use `sizeof(T)` for the stride.
 */
typedef struct NK_Vector
{
    /**
     * @brief Contains the top (or count of objects).
     */
    NK_Size top;

    /**
     * @brief Contains the capacity of the vector.
     */
    NK_Size capacity;

    /**
     * @brief The size of the elements you using on the vector.
     */
    NK_Size element_size;
    
    /**
     * @brief Do not access this directly!
     */
    NK_U8* data;
} NK_Vector;


/** Basic New/Free, Construct/Destruct: */

/**
 * @brief Gives you a new vector.
 * 
 * @warning This doesn't call `NK_VectorConstruct`
 */
NK_Vector*
NK_VectorNew();

/**
 * @brief Destroys your vector created by `NK_VectorNew`
 * 
 * @warning This doesn't call `NK_VectorDestruct`
 */
void
NK_VectorFree(
    NK_Vector* vector
);

/**
 * @brief Construct your vector.
 * 
 * @warning This uses `NK_CONFIG_CONTAINERS_VECTOR_CONSTRUCT_DEFAULT_SIZE` as early size.
 * @warning Always use `sizeof(T)` for element_size!
 */
void
NK_VectorConstruct(
    NK_Vector* vector,
    const NK_Size element_size
);

/**
 * @brief Destruct your vector.
 * 
 * @warning This DOESN'T call `NK_VectorFree`, only frees internal buffer.
 * @warning This sets everything to zero.
 */
void
NK_VectorDestruct(
    NK_Vector* vector
);

/** Resize, Push, Pop and At (basic functions): */

/**
 * @brief Resizes the vector to another size.
 * 
 * @warning NOP when same size is applied.
 * @warning If your value got deleted (shrinking) and it was on HEAP, you got a memory leak!
 */
void
NK_VectorResize(
    NK_Vector* vector,
    const NK_Size new_size
);

/**
 * @brief Push an value to the top.
 */
void
NK_VectorPush(
    NK_Vector* vector,
    const void* data
);

/**
 * @brief Pop an value from the top.
 * 
 * @warning When `dst` is NULL, then the value is just erased.
 * @warning This function is NOT safe, make sure `dst` can fit!
 * @warning If your value got deleted and it was on HEAP, you got a memory leak!
 */
NK_Result
NK_VectorPop(
    NK_Vector* vector,
    void* dst
);

/**
 * @brief Get the element from a certain index.
 * 
 * @warning When the index is not initialized (out bounds), the value is NULL.
 * @warning This returns the pointer to the object.
 * 
 * @note `At` functions returns the absolute value, since Vector has no internal
 * working mechanisms, `At` is the default way to get something.
 */
void*
NK_VectorAt(
    NK_Vector* vector,
    const NK_Size index
);

/**
 * @brief Get the number of elements on the vector.
 * 
 * @returns The number of elements on the vector.
 */
NK_Size
NK_VectorSize(
    NK_Vector* vector
);

/** Functional: */

/**
 * @brief Vector Iterator Function.
 * 
 * @note Return `true` to continue the iteration, `false` to stop the iteration.
 * @note `userdata` is always the same.
 */
typedef 
NK_Result
(*NK_VectorIteratorFunction)(
    NK_Vector* vector,
    const NK_Size index,
    void* data,
    void* userdata
);

/**
 * @brief Iterate the vector.
 * 
 * @note `true` continues the iterator, `false` stops.
 * @note `userdata` is always the same.
 * @note This will only traverse 0 < x < top.
 */
void
NK_VectorIterate(
    NK_Vector* vector,
    NK_VectorIteratorFunction iterator,
    void* userdata
);

#endif