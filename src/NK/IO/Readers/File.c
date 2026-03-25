/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#include "NK/IO/Readers/File.h"
#include "NK/Types/Basics.h"
#include "NK/Redirects.h"
#include "NK/Memory.h"

/**
 * @brief This is the main object to be on the `NK_Reader` userdata.
 */
typedef struct NK_FileReaderUnit
{
    /**
     * @brief The handler for the file.
     */
    NK_OSFile os_file_handler;

    /**
     * @brief Stores the position.
     */
    NK_Size position;
} NK_FileReaderUnit;

/**
 * @brief This opens the file, expects the `extra_parameter` to be
 * `const NK_C8*` by default.
 */
static
NK_Result
P_NK_FileReaderUnitOpenImplementation(
    NK_FileReaderUnit* unit,
    const NK_C8* path
)
{
    /** 
     * NOTE: The NK_OSFileOpen must be compatible with this pattern, if your
     * platform on the future just happens to not support this, then we will
     * have a problem, in this case, we can create those `Implementation`
     * function specific for your platform.
     */
    unit->os_file_handler = NK_OSFileOpen(path, "rb");
    unit->position = 0;
    return (
        unit->os_file_handler != NULL
        ? true
        : false
    );
}

/**
 * @brief This closes the file.
 */
static
void
P_NK_FileReaderUnitCloseImplementation(
    NK_FileReaderUnit* unit
)
{
    if(unit->os_file_handler != NK_OSFileEmptyHandler)
    {
        NK_OSFileClose(unit->os_file_handler);
    }
    unit->position = 0;
}

/**
 * @brief This sets the position of the file.
 */
static
void
P_NK_FileReaderUnitSetPositionImplementation(
    NK_FileReaderUnit* unit,
    const NK_Size position
)
{
    if(unit->os_file_handler != NK_OSFileEmptyHandler)
    {
        NK_OSFileSeek(
            unit->os_file_handler,
            (long)(position),
            NK_OSFileSeekCurrent
        );
        unit->position = position;
    }
}

/**
 * @brief This gets the position of the file.
 */
static
const NK_Size
P_NK_FileReaderUnitGetPositionImplementation(
    NK_FileReaderUnit* unit
)
{
    NK_Size position = -1;
    if(unit->os_file_handler != NK_OSFileEmptyHandler)
    {
        position = unit->position;
    }
    return position;
}

/**
 * @brief This gets an single character, must return -1 when the character is
 * EOF, which is always expected and default on `NK` library.
 */
static
const NK_S32
P_NK_FileReaderUnitGetCharacterImplementation(
    NK_FileReaderUnit* unit
)
{
    NK_S32 value = -1;
    if(unit->os_file_handler != NK_OSFileEmptyHandler)
    {
        value = NK_OSFileGetCharacter(unit->os_file_handler);
        unit->position++;
    }
    return value;
}

/**
 * @brief This is the main table for the buffer to use.
 */
static
const NK_ReaderInterfaceFunctions
NK_FileReaderUnitImplementations =
{
    (NK_ReaderInterfaceOpenFunction)(
        P_NK_FileReaderUnitOpenImplementation
    ),
    (NK_ReaderInterfaceCloseFunction)(
        P_NK_FileReaderUnitCloseImplementation
    ),
    (NK_ReaderInterfaceSetPositionFunction)(
        P_NK_FileReaderUnitSetPositionImplementation
    ),
    (NK_ReaderInterfaceGetPositionFunction)(
        P_NK_FileReaderUnitGetPositionImplementation
    ),
    (NK_ReaderInterfaceGetCharacterFunction)(
        P_NK_FileReaderUnitGetCharacterImplementation
    )
};

NK_FileReader*
NK_FileReaderNew()
{
    NK_FileReader* new_file_reader = 
        (NK_FileReader*)(
            NK_AllocatorGet(sizeof(NK_FileReader))
        );
    return new_file_reader;
}

void
NK_FileReaderFree(
    NK_FileReader* file
)
{
    NK_AllocatorFree(file);
}

void
NK_FileReaderConstruct(
    NK_FileReader* file
)
{
    file->functions = &NK_FileReaderUnitImplementations;
    file->userdata =
        (NK_FileReaderUnit*)(
            NK_AllocatorGet(sizeof(NK_FileReaderUnit))
        );
    ((NK_FileReaderUnit*)(file->userdata))->os_file_handler =
        NK_OSFileEmptyHandler;
    ((NK_FileReaderUnit*)(file->userdata))->position = 0;
}

void
NK_FileReaderDestruct(
    NK_FileReader* file
)
{
    /** NOTE: We destroy the `Unit` object. */
    NK_AllocatorFree(file->userdata);
    file->functions = NULL;
}