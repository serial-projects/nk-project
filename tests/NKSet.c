/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

/** NOTE: This is to test if NK can EVEN execute. */
#include "NK/NK.h"

#include <stdio.h>

static
void
P_NK_SetTest(
    
)
{
    NK_Set m_set;
    NK_SetConstruct(&m_set, 1024);
    NK_SetDestruct(&m_set);
}

int
main(
    int argc,
    char *argv[]
)
{
    printf("\t%s: Calling NK_Init()\n", __FILE__);
    NK_Init();

    printf("\t%s: Calling NK_Quit()\n", __FILE__);
    NK_Quit();
    
    /** Reached here with no crashes? Good. */
    return 0;
}