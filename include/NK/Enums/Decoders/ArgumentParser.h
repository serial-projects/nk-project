/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKEnumsDecodersArgumentParser_h
#define NKEnumsDecodersArgumentParser_h

/** State: */

/**
 * @brief Describes the value for the argument parser when it is running, in 
 * this state, the index has not reached the end of the argument list.
 */
#define NK_ENUMS_ARGUMENT_PARSER_STATE_RUNNING  0

/**
 * @brief Describes the value for the argument parser when the index has reached
 * the size of the argument list with no errors, in this case, the parser has
 * finished.
 */
#define NK_ENUMS_ARGUMENT_PARSER_STATE_FINISHED 1

/**
 * @brief Describes the value for the argument parser when an callback has not
 * returned true, in this case, the callback function has failed on whatever
 * it was supposed to do, the `DIED` means not all arguments were parsed.
 */
#define NK_ENUMS_ARGUMENT_PARSER_STATE_DIED     2

/** Errors: */

/**
 * @brief This is when there are no errors.
 */
#define NK_ENUMS_ARGUMENT_PARSER_ERROR_CODE_NONE 0

/**
 * @brief This is when there is no matching argument, for example, there was an
 * '--enable-super-secret-feature' option and yet, no link was defined to it,
 * that would tell an certain action.
 */
#define NK_ENUMS_ARGUMENT_PARSER_ERROR_NO_LINK_FOUND 1

/**
 * @brief This is when there is no matching action.
 * 
 * @warning This is more of a severe error, you probably defined an link with
 * an wrong action name. This is ALWAYS something in the hardcoded options.
 */
#define NK_ENUMS_ARGUMENT_PARSER_ERROR_NO_ACTION_FOUND 2

/**
 * @brief This is when the user has returned `false` on the callback function.
 */
#define NK_ENUMS_ARGUMENT_PARSER_ERROR_USER_TRIGGER 3

#endif