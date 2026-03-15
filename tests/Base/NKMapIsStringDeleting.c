/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

/** NOTE: This is to test if NK can EVEN execute. */
#include "NK/NK.h"

#include <stdio.h>

int
main(
    int argc,
    char *argv[]
)
{
    NK_Init();

    NK_Map m_map;
    NK_MapConstruct(
        &m_map,
        1024,
        sizeof(NK_U32)
    );

    NK_U32 x = 42;
    NK_MapInsertOrAssign(
        &m_map,
        "ruka",
        &x
    );
    NK_MapInsertOrAssign(
        &m_map,
        "raposa",
        &x
    );

    NK_SubmergedString m_ss1;
    NK_SubmergedStringConstruct(&m_ss1, "ruka");

    printf("\t%s: m_ss1 = %lu\n", __FILE__, m_ss1.handler);
    NK_MapRemove(&m_map, "ruka");

    NK_SubmergedString m_ss2;
    NK_SubmergedStringConstruct(&m_ss2, "ruka");
    printf("\t%s: m_ss1 = %lu\n", __FILE__, m_ss1.handler);
    NK_SubmergedStringDestruct(&m_ss1);
    NK_SubmergedStringDestruct(&m_ss2);

    NK_SubmergedStringConstruct(&m_ss1, "mikey");
    printf("\t%s: m_ss1 = %lu\n", __FILE__, m_ss1.handler);

    NK_MapDestruct(&m_map);
    NK_SubmergedStringDestruct(&m_ss1);

    NK_Quit();   
    /** Reached here with no crashes? Good. */
    return 0;
}
