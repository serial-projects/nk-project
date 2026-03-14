/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKSupport_h
#define NKSupport_h

/**
 * NOTE: Decoration (or not).
 */

#if defined(__GNUC__) || defined(__clang__)
    #define NK_SUPPORT_LIKELY(x)   __builtin_expect(!!(x), 1)
    #define NK_SUPPORT_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
    #define NK_LIKELY(x)   (x)
    #define NK_UNLIKELY(x) (x)
#endif

/** 
 * NOTE: This is for the text macro'ing.
 */
#define __NK_SUPPORT_STR_GET(x) #x
#define NK_SUPPORT_STR(x) __NK_SUPPORT_STR_GET(x)

/**
 * NOTE: This is SOME compiler work, but I swear, it is to DEBUGGING! The only
 * we tolerate is like NK_CURRENT_FUNCTION which we NEED to debugging.
 */
#if defined(__GNUC__) || defined(__clang__)
    /**
     * IN GCC/CLANG, this NK_CURRENT_FUNCTION is possible with
     * NK_CURRENT_FUNCTION
     */
    #define NK_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
    /**
     * IN MSC, this is the __FUNCSIG__ function.
     */
    #define NK_CURRENT_FUNCTION __FUNCSIG__
#else
    /**
     * In any case, use the __func__ C "standard"
     */
    #define NK_CURRENT_FUNCTION __func__
#endif

#define NK_CURRENT_WHERE __FILE__ ":" NK_SUPPORT_STR(__LINE__)

#endif