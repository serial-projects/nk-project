/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Task/Coroutine.h"
#include "NK/Memory.h"
#include "NK/Core/Panic.h"
#include "NK/Enums.h"

NK_Coroutine*
NK_CoroutineNew()
{
    NK_Coroutine* new_coroutine = 
        (NK_Coroutine*)(
            NK_AllocatorGet(sizeof(NK_Coroutine))
        );
    return new_coroutine;
}

void
NK_CoroutineFree(
    NK_Coroutine* coroutine
)
{
    NK_AllocatorFree(coroutine);
}

void
NK_CoroutineConstruct(
    NK_Coroutine* coroutine,
    NK_CoroutineActionFunction* functions,
    void* userdata
)
{
    coroutine->functions = functions;
    coroutine->pointer = 0;
    coroutine->state = NK_ENUMS_COROUTINE_RUNNING;
    coroutine->userdata = userdata;
}

void
NK_CoroutineDestruct(
    NK_Coroutine* coroutine
)
{
    coroutine->functions = NULL;
    coroutine->pointer = 0;
    coroutine->state = NK_ENUMS_COROUTINE_FINISHED;
    coroutine->userdata = NULL;
}

const NK_U8
NK_CoroutineStep(
    NK_Coroutine* coroutine
)
{
    NK_CoroutineActionFunction function = 
        coroutine->functions[coroutine->pointer];
    NK_U8 state;
    if(coroutine->state != NK_ENUMS_COROUTINE_RUNNING)
    {
        goto skip_ending;
    }
    
    /** Get the return key. */
    state = function(coroutine->userdata);
    switch(state)
    {
        case NK_ENUMS_COROUTINE_FLOW_BACK_TRACK:
            {
                if(coroutine->pointer <= 0)
                {
                    NK_Panic(
                        "%s: "
                        "Attempt to back track on NK_Coroutine with stack on 0",
                        NK_CURRENT_WHERE
                    );
                }
                coroutine->pointer--;
            };
            break;
        case NK_ENUMS_COROUTINE_FLOW_STAY:
            /** Set the stay: */
            break;
        case NK_ENUMS_COROUTINE_FLOW_NEXT_TRACK:
            {
                /* Set the pointer to the next track: */
                coroutine->pointer++;
            };
            break;
        case NK_ENUMS_COROUTINE_FLOW_FINISH:
            {
                coroutine->state = NK_ENUMS_COROUTINE_FINISHED;
            };
            break;
        case NK_ENUMS_COROUTINE_FLOW_DIE:
            {
                coroutine->state = NK_ENUMS_COROUTINE_DIED;
            };
            break;
        default:
            {
                NK_Panic(
                    "%s: "
                    "Invalid function return on NK_Coroutine = %d\n",
                    NK_CURRENT_WHERE,
                    state
                );
            };
            break;
    }
skip_ending:
    return coroutine->state;
}