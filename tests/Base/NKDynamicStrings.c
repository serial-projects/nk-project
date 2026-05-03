/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

/** NOTE: This is to test if NK can EVEN execute. */
#include "NK/NK.h"

#include <stdio.h>

static
void
Test(
)
{
    NK_DynamicString m_string;
    NK_DynamicStringConstruct(&m_string);

    NK_C8 c='a';
    NK_U8 index=0;

    for(
        index;
        index <= 10;
        index++
    )
    {
        c='a';
        for(c; c <= 'z'; c++)
        {
            NK_DynamicStringPush(
                &m_string,
                c
            );
        }
    }

    /** PRINT: */
    printf("GOT: %s\n", NK_DynamicStringGetBuffer(&m_string));
    NK_DynamicStringDestruct(&m_string);
}

int
main(
    int argc,
    char *argv[]
)
{
    printf("\t%s: Calling NK_Init()\n", __FILE__);
    NK_Init();

    Test();

    printf("\t%s: Calling NK_Quit()\n", __FILE__);
    NK_Quit();
        
    /** Reached here with no crashes? Good. */
    return 0;
}