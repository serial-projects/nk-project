/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Internals/Hash.h"
#include "NK/Internals/HashImplementations/FNV.inl"

NK_U64
NK_Hash(
    const NK_C8* key
)
{
    return P_NK_FNVHashFunction(key);
}