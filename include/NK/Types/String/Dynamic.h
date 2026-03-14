/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKTypesStringDynamic_h
#define NKTypesStringDynamic_h

#include "NK/Types/Basics.h"

/** 
 * @brief This is a implementation of the dynamic strings.
 * 
 * @warning Make sure this are only for strings that change all the time,
 * because `NK_SubmergedString`(s) are for more static strings (that doesn't 
 * change all the time).
 */
typedef struct NK_DynamicString
{
    NK_U8* buffer;
    NK_Size top;
    NK_Size capacity;
} NK_DynamicString;

/** New, Free, Construct and Destruct: */

/**
 * @brief Creates an new `NK_DynamicString`
 */
NK_DynamicString*
NK_DynamicStringNew();

/**
 * @brief Frees an string created by `NK_DynamicStringNew`
 */
void
NK_DynamicStringFree(
    NK_DynamicString* string
);

/**
 * @brief Construct the internal buffers of the string.
 */
void
NK_DynamicStringConstruct(
    NK_DynamicString* string
);

/**
 * @brief Frees the internal buffers of the string.
 */
void
NK_DynamicStringDestruct(
    NK_DynamicString* string
);

/** Resize, Push and Pop: */

/**
 * @brief Resize the string.
 */
void
NK_DynamicStringResize(
    NK_DynamicString* string,
    const NK_Size new_size
);

/**
 * @brief Push the character on the string.
 */
void
NK_DynamicStringPush(
    NK_DynamicString* string,
    const NK_C8 character
);

/**
 * @brief Return the character from the top and give you.
 */
NK_C8
NK_DynamicStringPop(
    NK_DynamicString* string
);

/** Get, At, Size: */

/**
 * @brief Get the character at a position.
 * 
 * @warning This returns the character itself, not the pointer.
 */
NK_C8
NK_DynamicStringGet(
    NK_DynamicString* string,
    const NK_Size index
);

/**
 * @brief Get the POINTER to the character at a position.
 * 
 * @warning This returns an pointer to the character.
 */
NK_C8*
NK_DynamicStringAt(
    NK_DynamicString* string,
    const NK_Size index
);

/**
 * @brief Get the size of the string.
 */
NK_Size
NK_DynamicStringSize(
    NK_DynamicString* string
);

/** GetBuffer: */

/**
 * @brief This returns the buffer (that can be used as const char*).
 */
NK_C8*
NK_DynamicStringGetBuffer(
    NK_DynamicString* string
);

/** Equal, EqualImmediate: */

/**
 * @brief Test if a string is equal.
 */
NK_Result
NK_DynamicStringEqual(
    NK_DynamicString* string,
    NK_DynamicString* source
);

/**
 * @brief Test if a string is equal (immediate).
 */
NK_Result
NK_DynamicStringEqualImmediate(
    NK_DynamicString* string,
    const NK_C8* immediate
);

/** Clear: */

/**
 * @brief Clean the string.
 */
void
NK_DynamicStringClear(
    NK_DynamicString* string
);

/** SetImmediate: */

/**
 * @brief Set the immediate value.
 */
void
NK_DynamicStringSetImmediate(
    NK_DynamicString* string,
    const NK_C8* immediate
);

/** Duplicate: */

/**
 * @brief Duplicate the string given a start and end range.
 */
void
NK_DynamicStringDuplicate(
    NK_DynamicString* string,
    NK_DynamicString* base,
    NK_Size start,
    NK_Size end
);

#endif