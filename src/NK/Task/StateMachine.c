/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Task/StateMachine.h"
#include "NK/Memory.h"
#include "NK/Enums.h"

NK_StateMachine*
NK_StateMachineNew()
{
    NK_StateMachine* new_state_machine =
        (NK_StateMachine*)(
            NK_AllocatorGet(sizeof(NK_StateMachine))
        );
    return new_state_machine;
}

void
NK_StateMachineFree(
    NK_StateMachine* machine
)
{
    NK_AllocatorFree(machine);
}

void
NK_StateMachineConstruct(
    NK_StateMachine* machine,
    void* userdata,
    NK_StateMachineActionFunction* list,
    NK_Size entrypoint
)
{
    machine->core.current_option = entrypoint;
    machine->core.userdata = userdata;
    machine->state = NK_ENUMS_STATE_MACHINE_RUNNING;
    machine->list = list;
    machine->counter = 0;
}

void
NK_StateMachineDestruct(
    NK_StateMachine* machine
)
{
    machine->core.current_option = 0;
    machine->core.userdata = NULL;
    machine->list = NULL;
    machine->state = NK_ENUMS_STATE_MACHINE_FINISHED;
}

const NK_U8
NK_StateMachineStep(
    NK_StateMachine* machine
)
{
    NK_StateMachineActionFunction action = 
        machine->list[machine->core.current_option];
    if(machine->state != NK_ENUMS_STATE_MACHINE_RUNNING)
    {
        machine->state = action(
            &machine->core,
            machine->counter
        );
        machine->counter++;
    }
    return machine->state;
}