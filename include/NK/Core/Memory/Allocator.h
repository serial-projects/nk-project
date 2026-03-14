/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKMemoryAllocator_h
#define NKMemoryAllocator_h

#include "NK/Types/AllocatorStatistics.h"
#include "NK/Redirects.h"
#include "NK/Types/Basics.h"
#include "NK/Support.h"

/**
 * @brief This is a information to ALL allocated pointers. It contains some
 * information you can get.
 */
typedef struct NK_AllocatorBlockInformation
{
    /** 
     * @brief Tells the size of the pointer, used when resize.
     */
    NK_Size size;
} NK_AllocatorBlockInformation;

/**
 * @brief Gets a new memory block for you to use.
 * 
 * @note Use the intended macro `NK_AllocatorGet` to automatically get `from`
 * @warning This does check for bad allocations and will panic!
 * @warning All the new returned blocks are ZERO'ed for security!
 */
void*
NK_AllocatorImplementationGet(
    const NK_C8* from,
    NK_Size size
);

/**
 * @brief Gets an new memory block for you to use, but does not check for
 * errors. Pretty useful in case you didn't need the memory block.
 * 
 * @note Use the intended macro `NK_AllocatorRequest`
 * @warning Only use this when you WANT to skip check-up on bad memory!
 */
void*
NK_AllocatorImplementationRequest(
    const NK_C8* from,
    NK_Size size
);

/**
 * @brief Frees the block from memory.
 * 
 * @note Use the intended macro `NK_AllocatorFree` to automatically get `from`
 * @warning This does not check if `block` is NULL.
 */
void
NK_AllocatorImplementationFree(
    const NK_C8* from,
    void* block
);

/**
 * @brief This resizes the block from memory.
 * 
 * @note Use the intended macro `NK_AllocatorResizeBlock` to get `from`
 * @warning This does not check if `original_block` is NULL.
 * @warning Always panic when failures happen.
 * @warning When using new_size == past_size, nothing happens.
 */
void*
NK_AllocatorImplementationResizeBlock(
    const NK_C8* from,
    void* block,
    const NK_Size new_size
);

/**
 * @brief This gets the current memory information about how much memory the
 * program is using.
 */
NK_AllocatorStatistics
NK_AllocatorImplementationGetCurrentStatistics();

/**
 * @brief This gets an new memory block for you to use and crash when NULL is
 * returned (automatically).
 */
#define \
NK_AllocatorGet(size) \
    NK_AllocatorImplementationGet(NK_CURRENT_WHERE, size)

/**
 * @brief This gets an new memory block for you to use BUT it won't crash when
 * NULL is returned by malloc() or any memory allocator you use.
 */
#define \
NK_AllocatorRequest(size) \
    NK_AllocatorImplementationRequest(NK_CURRENT_WHERE, size)

/**
 * @brief This frees your memory block.
 */
#define \
NK_AllocatorFree(block) \
    NK_AllocatorImplementationFree(NK_CURRENT_WHERE, block)

/**
 * @brief This resizes an previous memory block to a new size.
 */
#define \
NK_AllocatorResizeBlock(block, new_size) \
    NK_AllocatorImplementationResizeBlock(NK_CURRENT_WHERE, block, new_size)

/**
 * @brief This gets the current memory information.
 */
#define \
NK_AllocatorGetCurrentStatistics() \
    NK_AllocatorImplementationGetCurrentStatistics()

#endif