/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKEnumsValidator_h
#define NKEnumsValidator_h

/**
 * @brief This is the level for the debug messages.
 */
#define NK_ENUMS_VALIDATOR_LEVEL_DEBUG                                          0

/**
 * @brief This is the level for the log messages.
 */
#define NK_ENUMS_VALIDATOR_LEVEL_LOG                                            1

/**
 * @brief This is the level for the warning messages.
 */
#define NK_ENUMS_VALIDATOR_LEVEL_WARNING                                        2

/**
 * @brief This is the level for the crash messages.
 */
#define NK_ENUMS_VALIDATOR_LEVEL_ERROR                                          3

/**
 * @brief This is the max. value for `NK_ENUMS_VALIDATOR_LEVEL`, basically
 * the maximum amount that the functions with `level` parameter can take.
 * 
 * @warning Using an value on `level` will trigger panic.
 */
#define NK_ENUMS_VALIDATOR_LEVEL_MAX_OPTION                                     1 + 1 + 1 + 1

#endif