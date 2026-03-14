/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKCoreValidator_h
#define NKCoreValidator_h

#include "NK/Types/Basics.h"
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
 * @brief Is the container for the message internally!
 */
typedef struct NK_ValidatorMessage
{
    NK_DynamicString string;
    NK_U8 flags;
} NK_ValidatorMessage;


/**
 * @brief This the main content passed to the callbacks.
 */
typedef struct NK_ValidatorContent
{
    /**
     * @brief Circular buffer containing the messages.
     */
    NK_ValidatorMessage* buffer;

    /**
     * @brief Size of the buffer.
     */
    NK_Size buffer_limit;

    /**
     * @brief The counter of the buffer.
     */
    NK_Size buffer_counter;

    /** 
     * @brief Contains the userdata.
     */
    void* userdata;

    /**
     * @brief Contains the actual (and current) code.
     */
    NK_U8 code;
} NK_ValidatorContent;

/**
 * @brief This is the default listener callback function that is present
 * everywhere on the map.
 */
typedef
void(*NK_ValidatorListenerCallback)(
    NK_ValidatorContent* content,
    const NK_C8* message
);

/**
 * @brief The main structure for the `NK_Validator`
 * 
 * @warning This can be a logger and a validator at the same time, it works 
 * by having four levels of messages (and listeners): Debug, Log, Warning and
 * Error.
 */
typedef struct NK_Validator
{
    /**
     * @brief The general content for this sink.
     */
    NK_ValidatorContent content;

    /**
     * @brief This is the sink for `debug` messages, your listener can have an
     * ID if you want so.
     */
    NK_Vector debug_sink;

    /** 
     * @brief This the sink for `log` messages.
     */
    NK_Vector log_sink;

    /**
     * @brief This is the sink for `warning` messages.
     */
    NK_Vector warning_sink;

    /**
     * @brief This is the sink for `error` messages.
     */
    NK_Vector error_sink;
} NK_Validator;

/**
 * @brief This creates an new `NK_Validator` on the heap.
 */
NK_Validator*
NK_ValidatorNew();

/**
 * @brief This frees an `NK_Validator` created by the `NK_ValidatorNew`
 */
void
NK_ValidatorFree(
    NK_Validator* validator
);

/**
 * @brief This constructs the validator.
 */
void
NK_ValidatorConstruct(
    NK_Validator* validator,
    const NK_Size message_limit
);

/**
 * @brief This destructs the validator.
 */
void
NK_ValidatorDestruct(
    NK_Validator* validator
);

/**
 * @brief This assigns an _userdata_ to the `NK_Validator`
 */
void
NK_ValidatorAssignUserdata(
    NK_Validator* validator,
    void* userdata
);

/**
 * @brief This assigns an listener to the `NK_Validator` using the 
 * `NK_ValidatorListenerCallback` function pattern. It returns an ID you can
 * later "unplug" from the `NK_Validator`
 */
const NK_U64
NK_ValidatorAddListener(
    NK_Validator* validator,
    const NK_U8 level,
    const NK_ValidatorListenerCallback callback
);

/**
 * @brief This writes an message to the `NK_Validator` to the level you assigned
 * to. This function supports '...' and will have an internal buffer (for
 * `vsprintf` function) assigned in the `NK/Config.h` function.
 * 
 * @warning Due the nature of the validator implementation, there is no ID from
 * where the message came from, so, it is recommended you do yourself.
 */
void
NK_ValidatorPushMessage(
    NK_Validator* validator,
    const NK_U8 level,
    const NK_C8* format,
    ...
);

#endif