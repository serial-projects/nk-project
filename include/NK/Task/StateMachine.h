/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKTaskStateMachine_h
#define NKTaskStateMachine_h

#include "NK/Types/Basics.h"

/**
 * @brief The content that is passed to the functions.
 */
typedef struct NK_StateMachineCore
{
    /**
     * @brief This is the data that is shared across the function.
     */
    void* userdata;

    /**
     * @brief This is the current option (the current function) being indexed
     * on the state machine.
     */
    NK_Size current_option;
} NK_StateMachineCore;

/**
 * @brief The pattern for the function that should be inside an list function
 * table.
 */
typedef
NK_U8
(*NK_StateMachineActionFunction)(
    NK_StateMachineCore* core,
    NK_Size count
);

/**
 * @brief State Machine contains an `function list` and a `current_option`,
 * this is a automatic way to select the functions, all the functions must
 * althrough, return `NK_ENUMS_STATE_MACHINE_STATE` enum values to indicate
 * if the state machine is dead, finished, alive or waiting (special state).
 */
typedef struct NK_StateMachine
{
    /** 
     * @brief This is what is shared across the actions.
     */
    NK_StateMachineCore core;

    /**
     * @brief An pointer to `some` virtual list.
     */
    NK_StateMachineActionFunction* list;

    /**
     * @brief The current state of the machine.
     */
    NK_U8 state;

    /**
     * @brief Holds an counter for the how many times the `NK_StateMachine`
     * has called `NK_StateMachineStep` and we got an good result.
     */
    NK_Size counter;
} NK_StateMachine;

/**
 * @brief Creates an new `NK_StateMachine*`
 */
NK_StateMachine*
NK_StateMachineNew();

/**
 * @brief Frees an `NK_StateMachine` created by `NK_StateMachineNew`
 */
void
NK_StateMachineFree(
    NK_StateMachine* machine
);

/**
 * @brief Constructs an `NK_StateMachine`
 *
 * @param entrypoint Set where the `NK_StateMachine` will start.
 */
void
NK_StateMachineConstruct(
    NK_StateMachine* machine,
    void* userdata,
    NK_StateMachineActionFunction* list,
    NK_Size entrypoint
);

/**
 * @brief Destroys an `NK_StateMachine`
 */
void
NK_StateMachineDestruct(
    NK_StateMachine* machine
);

/**
 * @brief This will advance on the `NK_StateMachine` and will return an
 * `NK_ENUM_STATE_MACHINE` state.
 * 
 * @warning Once the `machine->state` enters any other state like
 * `NK_ENUMS_STATE_MACHINE_RUNNING` state, the machine won't step and the
 * `machine->counter` will NOT be incremented.
 */
const NK_U8
NK_StateMachineStep(
    NK_StateMachine* machine
);

#endif