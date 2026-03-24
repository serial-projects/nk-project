/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKEnumsTaskCoroutine_h
#define NKEnumsTaskCoroutine_h

/**
 * @brief Defines an number for when the `NK_Coroutine` state is running.
 */
#define NK_ENUMS_COROUTINE_RUNNING                                              0

/**
 * @brief Defines an number for when the `NK_Coroutine` state is finished.
 */
#define NK_ENUMS_COROUTINE_FINISHED                                             1

/**
 * @brief Defines an number for when the `NK_Coroutine` state is died.
 */
#define NK_ENUMS_COROUTINE_DIED                                                 2

/**
 * @brief When returned by a function, it says: `Back track`, return on the 
 * stack of the function.
 */
#define NK_ENUMS_COROUTINE_FLOW_BACK_TRACK                                      0

/**
 * @brief When returned by a function, it says: `Stay`, return on the stack
 * of the function.
 */
#define NK_ENUMS_COROUTINE_FLOW_STAY                                            1

/**
 * @brief When returned by a function, it says: `Next track`, skip on the
 * stack of the function.
 */
#define NK_ENUMS_COROUTINE_FLOW_NEXT_TRACK                                      2

/**
 * @brief When returned by a function, it says: `Finish`, it changes the state.
 */
#define NK_ENUMS_COROUTINE_FLOW_FINISH                                          3

/**
 * @brief When returned by a function, it says: `Die`, it changes the state.
 */
#define NK_ENUMS_COROUTINE_FLOW_DIE                                             4

#endif