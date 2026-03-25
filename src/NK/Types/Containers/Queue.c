/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * NK "New Key" is part of the Ruka Raposa Project.
 * 
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Types/Containers/Queue.h"
#include "NK/Memory.h"
#include "NK/Redirects.h"
#include "NK/Config.h"

NK_Queue*
NK_QueueNew()
{
    NK_Queue* queue =
        (NK_Queue*)(
            NK_AllocatorGet(
                sizeof(NK_Queue)
            )
        );
    return queue;
}

void
NK_QueueDestroy(
    NK_Queue* queue
)
{
    NK_AllocatorFree(queue);
}

void
NK_QueueConstruct(
    NK_Queue* queue,
    const NK_Size element_size
)
{
    queue->head = 0;
    queue->count = 0;
    queue->capacity = NK_CONFIG_QUEUE_CONSTRUCT_DEFAULT_SIZE;
    queue->element_size = element_size;
    queue->buffer =
        (NK_U8*)(
            NK_AllocatorGet(
                sizeof(NK_U8) * (queue->capacity * queue->element_size)
            )
        );
}

void
NK_QueueDestruct(
    NK_Queue* queue
)
{
    if(queue->buffer != NULL)
    {
        NK_AllocatorFree(queue->buffer);
        queue->buffer = NULL;
    }
}

static
void
__NK_QueueResize(
    NK_Queue* queue,
    const NK_Size new_capacity
)
{
    /**
     * NOTE: Assumptions we take in this implementation:
     *  1-) Our buffer won't ever shrink.
     *  2-) No imparity will happen.
     */

    NK_U8* new_buffer;
    NK_Size index = 0;
    NK_U8* src;
    NK_U8* dst;

    /** We make a new buffer: */
    new_buffer = 
        (NK_U8*)NK_AllocatorGet(
            sizeof(NK_U8) * (new_capacity * queue->element_size)
        );
    for(index; index < queue->count; index++)
    {
        src = 
            (NK_U8*)queue->buffer + 
            (
                /** 
                 * What we want? To start indexing from our first element, that
                 * comes with queue->head, this keeps the order of our elements
                 * in such a way that: 
                 */
                ((queue->head + index) % queue->capacity)
            ) * queue->element_size;
        dst = 
            (NK_U8*)new_buffer + 
            (
                /** 
                 * Now, we point directly to the start, we can uncircularized
                 * the loop.
                 */
                index
            ) * queue->element_size;
        NK_RedirectMemcpy(dst, src, queue->element_size);
    }

    /**
     * We free the old buffer and the use the new one, hopefully this is growing
     * up on a good pace.
     */
    NK_AllocatorFree(queue->buffer);
    queue->buffer = new_buffer;
    queue->head = 0;
    queue->capacity = new_capacity;
}

void
NK_QueueEnter(
    NK_Queue* queue,
    const void* data
)
{
    NK_U8* dest;
    if(queue->count == queue->capacity)
    {
        /** NOTE: In this case, we do resize: */
        __NK_QueueResize(
            queue,
            queue->capacity + 
            (
                1 +
                (
                    queue->capacity * 2
                )
            )
        );
    }
    dest = 
        (NK_U8*)queue->buffer +
        (
            ((queue->head + queue->count) % queue->capacity)
        ) * queue->element_size;
    NK_RedirectMemcpy(dest, data, queue->element_size);
    queue->count++;
}

NK_Result
NK_QueueLeave(
    NK_Queue* queue,
    void* destination
)
{
    NK_U8* source;
    if(queue->count <= 0)
    {
        /** We are not taking anything from the line anymore. */
        return false;
    }
    else
    {
        source = 
            (NK_U8*)queue->buffer +
            (
                (queue->head) % queue->capacity
            ) * queue->element_size;
        if(destination != NULL)
        {
            NK_RedirectMemcpy(
                destination,
                source,
                queue->element_size
            );
        }
        NK_RedirectMemset(source, 0, queue->element_size);
        queue->head = (queue->head + 1) % queue->capacity;
        queue->count--;
    }
    return true;
}

void
NK_QueueIterate(
    NK_Queue* queue,
    const NK_QueueIteratorFunction iterator,
    void* userdata
)
{
    NK_Size index = 0;
    NK_U8* data;
    for(index; index < queue->count; index++)
    {
        data =
            (NK_U8*)queue->buffer + 
            (
                (queue->head + index) % queue->capacity
            ) * queue->element_size;
        if(
            !iterator(
                queue,
                index,
                userdata,
                (void*)(data)
            )
        )
        {
            break;
        }
    }
}

NK_Size
NK_QueueSize(
    NK_Queue* queue
)
{
    return queue->count;
}