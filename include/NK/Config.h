/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKConfig_h
#define NKConfig_h

/** NK/Core/Memory/Allocator.c: */
#define NK_CONFIG_MEMORY_ALLOCATOR_GET_AND_REQUEST_SKIP_MEMORY_ZEROING
#define NK_CONFIG_MEMORY_ALLOCATOR_RESIZE_BLOCK_SKIP_MEMORY_ZEROING

/** NK/Core/Panic.c: */
#define NK_CONFIG_PANIC_BUFFER_SIZE                                             128

/** NK/Types/Containers/Vector.c: */
#define NK_CONFIG_VECTOR_CONSTRUCT_DEFAULT_SIZE                                 128

/** NK/Types/Internals/Table.c: */
#define NK_CONFIG_TABLE_CHAIN_DEFAULT_SIZE                                      10

/** NK/Types/Internals/StringPool.c: */
#define NK_CONFIG_STRING_POOL_CONSTRUCT_TABLE_CAPACITY                          8024

/** NK/Types/Queue.c: */
#define NK_CONFIG_QUEUE_CONSTRUCT_DEFAULT_SIZE                                  10

/** NK/Types/String/String.c: */
#define NK_CONFIG_STRING_CONSTRUCT_DEFAULT_SIZE                                 10

/** NK/Types/Containers/Map.c: */
#define NK_CONFIG_MAP_CHAIN_DEFAULT_SIZE                                        10

/** NK/Core/Validator.c: */
#define NK_CONFIG_VALIDATOR_PUSH_MESSAGE_BUFFER                                 512

/** NK/Types/Containers/Set.c: */
#define NK_CONFIG_SET_CHAIN_DEFAULT_SIZE                                        10

#endif