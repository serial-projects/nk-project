/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKCoreMemoryAllocatorRedirects_h
#define NKCoreMemoryAllocatorRedirects_h

#include "NK/Core/Memory/Allocator/Implementations.h"
#include "NK/Redirects.h"
#include "NK/Support.h"

#define NK_AllocatorGet(size)                                                   NK_AllocatorImplementationGet(NK_CURRENT_WHERE, size)
#define NK_AllocatorRequest(size)                                               NK_AllocatorImplementationRequest(NK_CURRENT_WHERE, size)
#define NK_AllocatorFree(block)                                                 NK_AllocatorImplementationFree(NK_CURRENT_WHERE, block)
#define NK_AllocatorResizeBlock(block, new_size)                                NK_AllocatorImplementationResizeBlock(NK_CURRENT_WHERE, block, new_size)
#define NK_AllocatorGetCurrentStatistics()                                      NK_AllocatorImplementationGetCurrentStatistics()

#endif