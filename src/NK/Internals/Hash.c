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
    /**
     * NOTE: An hash can never be zero. Why? Zero is set inside the `->hash`
     * nodes of `NK_Set` and `NK_Table`, etc as the EMPTY node, if an `node` has
     * a ZERO hash, it is because it was marked as deleted and so, it can be
     * written again.
     * 
     * The problem happens when the function `NK_Hash` can return 0, if that is
     * true, there is $x \in S$ where $S$ is all possible strings, that will
     * make $f(x) == 0$ $f$ function being our `NK_Hash`, thus, we make sure
     * that, when got 0 by the `Hash Backend`, we increment on one.
     */
    NK_U64 hash_adquired = P_NK_FNVHashFunction(key);
    return (
        hash_adquired == 0
        ? 1
        : hash_adquired
    );
}