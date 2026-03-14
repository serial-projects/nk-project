/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKCoreValidator_h
#define NKCoreValidator_h

#include "NK/Types/Containers/Vector.h"

/**
 * Due the nature of the validator holding ephemeral messages, it makes no
 * sense to be holding messages forever, it would only clog the system.
 */
#include "NK/Types/String/Dynamic.h"

/**
 * @brief This is the level for the debug messages.
 */
#define NK_VALIDATOR_LEVEL_DEBUG    0b10000000

/**
 * @brief This is the level for the log messages.
 */
#define NK_VALIDATOR_LEVEL_LOG      0b01000000

/**
 * @brief This is the level for the warning messages.
 */
#define NK_VALIDATOR_LEVEL_WARNING  0b00100000

/**
 * @brief This is the level for the crash messages.
 */
#define NK_VALIDATOR_LEVEL_ERROR    0b00010000

/**
 * @brief Custom level (0) for messages.
 */
#define NK_VALIDATOR_LEVEL_CUSTOM_0 0b00001000

/**
 * @brief Custom level (1) for messages.
 */
#define NK_VALIDATOR_LEVEL_CUSTOM_1 0b00000100

/**
 * @brief Custom level (2) for messages.
 */
#define NK_VALIDATOR_LEVEL_CUSTOM_2 0b00000010

/**
 * @brief Custom level (3) for messages.
 */
#define NK_VALIDATOR_LEVEL_CUSTOM_3 0b00000001

/**
 * @brief This is content for the `NK_Validator` which comes first definition
 * than the `NK_Validator` itself.
 */
typedef struct NK_ValidatorContent
{
    /**
     * @brief This is a userdata that is defined by the programmer.
     */
    void* userdata;

    /**
     * @brief This is the buffer, containing the messages. This is actually an
     * ring buffer, so we cycle on the new and past messages.
     */
    NK_DynamicString* buffer;

    /**
     * @brief The index of the buffer.
     */
    NK_U64 buffer_index;

    /**
     * @brief The index of the 
     */
    NK_U64 buffer_limit;
} NK_ValidatorContent;

/**
 * @brief This is the function that is called when the `NK_Validator` has an 
 * `Broadcast`, each broadcast has a level(s), and the `NK_ValidatorListener`
 * contains the *TUNED* channels the `NK_ValidatorListener` is listening to.
 */
typedef
void(*NK_ValidatorCallback)(
    NK_ValidatorContent* content,
    const NK_C8* message
);

/**
 * @brief This is a listener, it contains an `NK_ValidatorCallback` and a 
 * `NK_U8` holding the channels it can listen to.
 */
typedef struct NK_ValidatorListener
{
    /**
     * @brief The validator itself.
     */
    NK_ValidatorCallback callback;

    /**
     * @brief There are a few channels you can tune: DEBUG, LOG, WARNING, etc.
     */
    NK_U8 channels_tuned;
} NK_ValidatorListener;

/**
 * @brief This is a union of a logger and a validator. You can have: userdata,
 * code (holds the current state) and the buffer containing all the messages.
 */
typedef struct NK_Validator
{
    /**
     * @brief This is the validator content, which contains the code.
     */
    NK_ValidatorContent content;

    /**
     * @brief This is a listener.
     */
    NK_Vector listeners;
} NK_Validator;

/**
 * @brief Creates an new `NK_Validator` on the heap.
 */
NK_Validator*
NK_ValidatorNew();

/**
 * @brief Frees an `NK_Validator` created by `NK_ValidatorNew`
 * @warning Does not imply on `NK_ValidatorDestruct`!
 */
void
NK_ValidatorFree(
    NK_Validator* validator
);

/**
 * @brief Construct the `NK_Validator` object, allocates the buffers and all.
 */
void
NK_ValidatorConstruct(
    NK_Validator* validator,
    const NK_Size buffer_limit
);

/**
 * @brief Desconstruct the buffer `NK_Validator`, frees the strings and all.
 */
void
NK_ValidatorDestruct(
    NK_Validator* validator
);

#endif