/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKTypesContainersQueue_h
#define NKTypesContainersQueue_h

#include "NK/Types/Basics.h"

/**
 * @brief Queue is a object where the first enters and is the first to leave,
 * just like a real life Queue.
 * 
 * @warning This is a implementation using the circular buffer.
 */
typedef struct NK_Queue
{
    NK_U8* buffer;
    NK_Size capacity;
    NK_Size element_size;
    NK_Size head;
    NK_Size count;
} NK_Queue;

/** New, Free, Construct and Destruct: */

/**
 * @brief Creates an `NK_Queue` object on the heap.
 * @warning Does not imply on `NK_QueueConstruct`
 */
NK_Queue*
NK_QueueNew();

/**
 * @brief Frees an object created by `NK_QueueFree`
 * @warning Does not imply on `NK_QueueDestrcut`
 */
void
NK_QueueFree(
    NK_Queue* queue
);

/**
 * @brief Sets the internal buffers.
 */
void
NK_QueueConstruct(
    NK_Queue* queue,
    const NK_Size element_size
);

/**
 * @brief Frees the internal buffers of the Queue.
 * @warning In case you are holding pointers to items. Don't forget to free it.
 */
void
NK_QueueDestruct(
    NK_Queue* queue
);

/** Size, Enter and Leave: */

/**
 * @brief Gets the quantity of elements on the queue.
 * @warning This does NOT return the capacity.
 */
NK_Size NK_QueueSize(
    NK_Queue* queue
);

/**
 * @brief Push something to the end of the queue. In a POV, the element is 
 * entering the line.
 */
void
NK_QueueEnter(
    NK_Queue* queue,
    const void* data
);

/**
 * @brief Push something out the front of the list, like a QUEUE, in the
 * first in, first out structure. In a POV, the FIRST element is being
 * handled, thus it can leave the line.
 * 
 * @return Whether something left the Queue, by the default, you have no way
 * to know if there is something on the Queue unless you use `NK_QueueSize()`
 * but here you can get false when there is nothing on th destination.
 * 
 * @warning When there is nothing on the destination, we won't memset() it to
 * NULL, the value still the same, be aware.
 */
NK_Result
NK_QueueLeave(
    NK_Queue* queue,
    void* destination
);

/** Iterator: */

/**
 * @brief This is the default iterator function for the Queue function.
 */
typedef
NK_Result(*NK_QueueIteratorFunction)(
    NK_Queue* queue,
    const NK_Size index,
    void* userdata,
    void* data
);

/**
 * @brief Iterates on the Queue without changing it.
 * @warning You must return `true` to continue iterating.
 */
void
NK_QueueIterate(
    NK_Queue* queue,
    const NK_QueueIteratorFunction iterator,
    void* userdata
);

#endif