/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKCoreReadersFile_h
#define NKCoreReadersFile_h

#include "NK/Core/Reader.h"
#include "NK/Types/Basics.h"

/**
 * @brief `NK_FileReader` is just an interface for the `NK_Reader`, it is fully
 * compatible with so, `NK_Reader` functions and operations.
 */
typedef NK_Reader NK_FileReader;

/**
 * @brief Creates an new `NK_FileReader` on the heap.
 * 
 * @warning Does not imply `NK_FileReaderConstruct`, using this will result on
 * `NK_ReaderOpen` not executing at all, for example.
 */
NK_FileReader*
NK_FileReaderNew();

/**
 * @brief Deletes an `NK_FileReader` created by the `NK_FileReaderNew` function.
 * 
 * @warning This doesn't imply on `NK_ReaderClose`, if you don't close the file,
 * you will have an memory leak! Always make sure to close the buffer.
 */
void
NK_FileReaderFree(
    NK_FileReader* file
);

/**
 * @brief Constructs the `NK_Reader` object to be integrated with the
 * `NK_FileReader` implementations.
 * 
 * @warning Use `NK_ReaderOpen` to initialize the internal files.
 */
void
NK_FileReaderConstruct(
    NK_FileReader* file
);

/**
 * @brief Removes the implementation. This function is more for pedantic reason
 * than anything. It does *NOT* close the file interface inside the object, you
 * must close it by `NK_ReaderClose` function!
 */
void
NK_FileReaderDestruct(
    NK_FileReader* file
);

#endif