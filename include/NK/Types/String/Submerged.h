/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKTypesStringSubmerged_h
#define NKTypesStringSubmerged_h

#include "NK/Types/Basics.h"

/**
 * @brief "Submerged" Strings are strings that lives in the String Pool, they
 * are not be changed.
 * 
 * @warning If you want QUICK swap, use `NK_DynamicString`, this type of string
 * has no support for several things like: Push/Pop.
 */
typedef struct NK_SubmergedString
{
    /**
     * @brief What we are holding to:
     */
    NK_U64 handler;
} NK_SubmergedString;

/**
 * @brief Creates an new string.
 */
NK_SubmergedString*
NK_SubmergedStringNew();

/**
 * @brief Frees an string created by `NK_SubmergedString`
 */
void
NK_SubmergedStringFree(
    NK_SubmergedString* string
);

/**
 * @brief Construct the string.
 */
void
NK_SubmergedStringConstruct(
    NK_SubmergedString* string,
    const NK_C8* src
);

/**
 * @brief Destruct the string.
 * 
 * @warning If you still using this string, it won't be truely destroyed.
 */
void
NK_SubmergedStringDestruct(
    NK_SubmergedString* string
);

/** Size, Equal, EqualImmediate: */

/**
 * @brief The size of the string.
 */
NK_Size
NK_SubmergedStringSize(
    NK_SubmergedString* string
);

/**
 * @brief Compare for equality on the string.
 */
NK_Result
NK_SubmergedStringEqual(
    NK_SubmergedString* string,
    const NK_SubmergedString* with
);

/**
 * @brief Compare for equality on the string (but immediately).
 */
NK_Result
NK_SubmergedStringEqualImmediate(
    NK_SubmergedString* string,
    const NK_C8* with
);

/** Set, SetImmediate: */

/**
 * @brief Set the string to be a new value.
 * 
 * @warning This is fast but, the recommended is to NK_SubmergedString for stay
 * a extended period of time for fast comparasions.
 */
void
NK_SubmergedStringSet(
    NK_SubmergedString* string,
    const NK_SubmergedString* with
);

/**
 * @brief Set the string to be a new value (from a immediate).
 * 
 * @warning This is NOT fast, the recommended is to `NK_SubmergedString` for
 * stay a extended period of time for fast comparasions.
 */
void
NK_SubmergedStringSetImmediate(
    NK_SubmergedString* string,
    const NK_C8* value
);

/**
 * @brief Get the raw data (to a NK_C8*).
 * 
 * @warning This always return an "absolute" value because NK_C8* is actually
 * just the pointer to the string on the pool, so you can't modify it. The name
 * is `NK_SubmergedStringGet` instead of AT(), where AT() does require you to 
 * be the ABSOLUTE value.
 */
const NK_C8*
NK_SubmergedStringGet(
    NK_SubmergedString* string
);

/** Forget: */

/**
 * @brief What is to forget for an `NK_SubmergedString`? It is simply the act of
 * saying: *I don't this string anymore, set handler to 0*, this function
 * will degrade the current handler and set the internal handler to zero.
 */
void
NK_SubmergedStringForget(
    NK_SubmergedString* string
);

/**
 * @brief Says if a string is forgotten, the "forgot" state is when the
 * `NK_SubmergedString` has the handler equal to zero.
 */
NK_Result
NK_SubmergedStringIsForgotten(
    NK_SubmergedString* string
);

/**
 * @brief This gets the hash value of the string.
 */
NK_U64
NK_SubmergedStringGetHash(
    NK_SubmergedString* string
);

#endif