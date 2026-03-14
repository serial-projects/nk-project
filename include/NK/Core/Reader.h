/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NKReader_h
#define NKReader_h

#include "NK/Types/Basics.h"

/**
 * @brief This is the default function to open the buffer, it is used for some
 * types of buffer that demand this.
 * 
 * @warning For files, extra_parameter is const C8*, for example. But on some
 * readers, this might be a NULL.
 */
typedef
NK_Result
(*NK_ReaderInterfaceOpenFunction)(
    void* userdata,
    void* extra_parameter
);

/**
 * @brief This is the default function to close the buffer.
 */
typedef
void
(*NK_ReaderInterfaceCloseFunction)(
    void* userdata
);


/**
 * @brief This is the default function to set_position on the buffer.
 */
typedef
void
(*NK_ReaderInterfaceSetPositionFunction)(
    void* userdata,
    const NK_Size position
);

/**
 * @brief This is the default function to get_position on the buffer.
 */
typedef
const NK_Size
(*NK_ReaderInterfaceGetPositionFunction)(
    void* userdata
);

/**
 * @brief This is the default function to get_character on the buffer.
 * @warning This also should return EOF (aka. -1) when the buffer is over.
 */
typedef
const NK_S32
(*NK_ReaderInterfaceGetCharacterFunction)(
    void* userdata
);

/**
 * @brief Container for the `NK_Reader` object's functions.
 */
typedef struct NK_ReaderInterfaceFunctions
{
    NK_ReaderInterfaceOpenFunction open;
    NK_ReaderInterfaceCloseFunction close;
    NK_ReaderInterfaceSetPositionFunction set_position;
    NK_ReaderInterfaceGetPositionFunction get_position;
    NK_ReaderInterfaceGetCharacterFunction get_character;
} NK_ReaderInterfaceFunctions;

/**
 * @brief Container for any readers.
 */
typedef struct NK_Reader
{
    /**
     * @brief Contains an virtual table with all the implementations per object.
     */
    NK_ReaderInterfaceFunctions* functions;

    /**
     * @brief Contains the object the `NK_Reader` is working with.
     */
    void* userdata;
} NK_Reader;

/** New, Free, Construct, Destruct: */

/**
 * @brief Creates an new `NK_Reader` on the heap.
 */
NK_Reader*
NK_ReaderNew();

/**
 * @brief Destroys an `NK_Reader` created by `NK_ReaderNew`.
 */
void
NK_ReaderFree(
    NK_Reader* reader
);

/**
 * @brief This functions are only to pedantic reasons, they should'nt do
 * anything. Use your `NK_XReaderConstruct` instead.
 */
void
NK_ReaderConstruct(
    NK_Reader* reader
);

/**
 * @brief This functions are only to pedantic reason.
 */
void
NK_ReaderDestruct(
    NK_Reader* reader
);

/** Open, Close, SetPosition, GetPosition and GetCharacter: */

/**
 * @brief Opens *Generically* the `NK_Reader`.
 * 
 * @warning In case the `NK_Reader::functions->open` is NULL, then we will skip
 * the call and return false.
 */
NK_Result
NK_ReaderOpen(
    NK_Reader* reader,
    void* extra_parameter
);

/**
 * @brief Closes *Generically* the `NK_Reader`.
 * 
 * @warning In case the `NK_Reader::functions->close` is NULL, then we will skip
 * the call.
 */
void
NK_ReaderClose(
    NK_Reader* reader
);

/**
 * @brief Set Position (on the reader) *Generically* the `NK_Reader`.
 * 
 * @warning In case the `NK_Reader::functions->set_position` is NULL, then we
 * will skip the call.
 */
void
NK_ReaderSetPosition(
    NK_Reader* reader,
    const NK_Size position
);

/**
 * @brief Get Position (on the reader) *Generically* the `NK_Reader`.
 * 
 * @warning In case the `NK_Reader::functions->get_position` is NULL, then we
 * will skip the call and the return value is 0, always.
 */
const NK_Size
NK_ReaderGetPosition(
    NK_Reader* reader,
    const NK_Size position
);

/**
 * @brief Get Character *Generically* the `NK_Reader`.
 * 
 * @warning In case the `NK_Reader::functions->get_character` is NULL, then we
 * will skip the call and return value is -1, always.
 */
const NK_S32
NK_ReaderGetCharacter(
    NK_Reader* reader,
    const NK_Size position
);

#endif