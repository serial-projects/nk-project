/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKDecodersArgumentParser_h
#define NKDecodersArgumentParser_h

#include "NK/Types.h"

/**
 * @brief The argument parser is a simple tool to parse user arguments.
 */
typedef struct NK_ArgumentParser
{
    /**
     * @brief This is the equivalent of the `char **argv` parameters.
     */
    const NK_C8** args_source;

    /**
     * @brief This is how far we are parsing the arguments.
     */
    NK_Size args_index;

    /**
     * @brief This is the length of the `argv`
     * 
     * @warning Bad values imply on `Segmentation Fault`, use `argc` provided
     * in the `int main()` function, *always*!
     */
    NK_Size args_length;

    /**
     * @brief Contains an pointer to any data.
     */
    void* userdata;

    /**
     * @brief This are the actions.
     */
    NK_Map actions;

    /**
     * @brief This are the links, which points to actions.
     */
    NK_Map links;

    /**
     * @brief Contains the argument parser state.
     */
    NK_U8 state;

    /**
     * @brief Contains the argument parser error handler.
     */
    NK_U8 error_register;
} NK_ArgumentParser;

/**
 * @brief This is the default pattern for `Callback` on the `NK_ArgumentParser`
 * system.
 * 
 * @return Must return if the argument parser should continue or not.
 * 
 * @note You might pull extra arguments with `NK_ArgumentParserPull()` function,
 * you make your own alignment. This argument parser is pretty lazy/dumb. It
 * only works on the entrypoint, which is "parsing" your argument.
 */
typedef
NK_Result
(*NK_ArgumentParserActionFunction)(
    NK_ArgumentParser* argument_parser
);

/**
 * @brief Creates an new `NK_ArgumentParser`
 */
NK_ArgumentParser*
NK_ArgumentParserNew();

/**
 * @brief Frees the `NK_ArgumentParser`
 */
void
NK_ArgumentParserFree(
    NK_ArgumentParser* arg_parser
);

/**
 * @brief Construct the argument parser.
 */
void
NK_ArgumentParserConstruct(
    NK_ArgumentParser* arg_parser,
    const NK_Size args_counter,
    const NK_C8** args_source,
    void* userdata
);

/**
 * @brief Destruct the argument parser.
 */
void
NK_ArgumentParserDestruct(
    NK_ArgumentParser* arg_parser
);

/**
 * @brief Add an action for the argument parser.
 */
void
NK_ArgumentParserAddAction(
    NK_ArgumentParser* arg_parser,
    const NK_C8* key,
    NK_ArgumentParserActionFunction function
);

/**
 * @brief Add an link for the argument parser.
 */
void
NK_ArgumentParserAddLink(
    NK_ArgumentParser* arg_parser,
    const NK_C8* key,
    const NK_C8* action
);

/**
 * @brief This will pull from the list (if possible), an parameter.
 * 
 * @warning This function returns null when no parameter could be adquired.
 */
const NK_C8*
NK_ArgumentParserPull(
    NK_ArgumentParser* arg_parser
);

/**
 * @brief Parse the arguments, returns the state.
 */
NK_U8
NK_ArgumentParserStep(
    NK_ArgumentParser* arg_parser
);

/**
 * @brief Get the current state of the parser.
 */
const NK_U8
NK_ArgumentParserGetState(
    NK_ArgumentParser* arg_parser
);

/**
 * @brief Get the current error register.
 */
const NK_U8
NK_ArgumentParserGetErrorRegister(
    NK_ArgumentParser* arg_parser
);

#endif