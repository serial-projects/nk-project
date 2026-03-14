/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#ifndef NKInternalsHashImplementationsFNV_inl
#define NKInternalsHashImplementationsFNV_inl

#include "NK/Types/Basics.h"

/** Magic Numbers: */
#define NK_FNV_HASH_FUNCTION_OFFSET      14695981039346656037UL
#define NK_FNV_HASH_FUNCTION_PRIME       1099511628211UL

/**
 * @brief Implementation of the FNV Hash Function.
 */
NK_U64
P_NK_FNVHashFunction(
    const NK_C8* key
)
{
    register NK_U64 hash = NK_FNV_HASH_FUNCTION_OFFSET;
    const NK_C8* __hashing_key = key;
    for(__hashing_key; *__hashing_key; *__hashing_key++)
    {
        hash ^= (NK_U64)(NK_U8)(*__hashing_key);
        hash *= NK_FNV_HASH_FUNCTION_PRIME;
    }
    return hash;
}

#endif