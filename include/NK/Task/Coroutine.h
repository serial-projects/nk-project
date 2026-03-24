/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKTaskCoroutine_h
#define NKTaskCoroutine_h

#include "NK/Types/Basics.h"

/**
 * @brief This is the pattern of the `NK_Coroutine`
 */
typedef NK_U8(*NK_CoroutineActionFunction)(void* userdata);

/**
 * An `NK_Coroutine` works in the sense of, it has a function that can
 * `yield` and allow the program to not exhaust itself on an loop that it knows
 * it will take like, seconds, so by `returning earlier, but saving work` the
 * program can `tick an engine frame` and advance it. Since `C` has no way to
 * implement an yield, what we do is cheat. By using an
 * `NK_ENUM_COROUTINE_FLOW_ADVANCE` or `NK_ENUM_COROUTINE_FLOW_BACK`, etc, we
 * can go back and forth on the coroutine.
 */
typedef struct NK_Coroutine
{
    /**
     * @brief Contains the function that is called.
     */
    NK_CoroutineActionFunction* functions;

    /**
     * @brief Set the current point of the machine.
     */
    NK_Size pointer;

    /**
     * @brief Contains the `userdata` for the `NK_CoroutineActionFunction`.
     */
    void* userdata;

    /**
     * @brief Holds the state of the coroutine, set in `NK_ENUM_COROUTINE_`
     * enum.
     */
    NK_U8 state;
} NK_Coroutine;

/**
 * @brief Creates an new `NK_Coroutine`
 */
NK_Coroutine*
NK_CoroutineNew();

/**
 * @brief Frees an `NK_Coroutine` created by `NK_CoroutineNew`
 */
void
NK_CoroutineFree(
    NK_Coroutine* coroutine
);

/**
 * @brief Constructs an `NK_Coroutine`
 */
void
NK_CoroutineConstruct(
    NK_Coroutine* coroutine,
    NK_CoroutineActionFunction* functions,
    void* userdata
);

/**
 * @brief Destroys the `NK_Coroutine`
 */
void
NK_CoroutineDestruct(
    NK_Coroutine* coroutine
);

/**
 * @brief Step an `NK_Coroutine` and returns the state it is.
 */
const NK_U8
NK_CoroutineStep(
    NK_Coroutine* coroutine
);

#endif
