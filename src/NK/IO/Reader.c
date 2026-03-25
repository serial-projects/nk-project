/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/IO/Reader.h"
#include "NK/Memory.h"

NK_Reader*
NK_ReaderNew()
{
    NK_Reader* new_reader =
        (NK_Reader*)(
            NK_AllocatorGet(sizeof(NK_Reader))
        );
    return new_reader;
}

void
NK_ReaderFree(
    NK_Reader* reader
)
{
    NK_AllocatorFree(reader);
}

void
NK_ReaderConstruct(
    NK_Reader* reader
)
{
    /** NOTE: This function only exists for pedantic reasons. */
    return;
}

void
NK_ReaderDestruct(
    NK_Reader* reader
)
{
    /** NOTE: This function only exists for pedantic reasons. */
    return;
}

/** Open, Close, SetPosition, GetPosition and GetCharacter: */

NK_Result
NK_ReaderOpen(
    NK_Reader* reader,
    void* extra_parameter
)
{
    return (
        reader->functions->open != NULL
        ? reader->functions->open(reader->userdata, extra_parameter)
        : false
    );
}

void
NK_ReaderClose(
    NK_Reader* reader
)
{
    if(reader->functions->close != NULL)
    {
        reader->functions->close(reader->userdata);
    }
}

void
NK_ReaderSetPosition(
    NK_Reader* reader,
    const NK_Size position
)
{
    if(reader->functions->set_position != NULL)
    {
        reader->functions->set_position(reader->userdata, position);
    }
}

const NK_Size
NK_ReaderGetPosition(
    NK_Reader* reader,
    const NK_Size position
)
{
    return (
        reader->functions->get_position != NULL
        ? reader->functions->get_position(reader->userdata)
        : 0
    );
}

const NK_S32
NK_ReaderGetCharacter(
    NK_Reader* reader,
    const NK_Size position
)
{
    return (
        reader->functions->get_character != NULL
        ? reader->functions->get_character(reader->userdata)
        : -1
    );
}