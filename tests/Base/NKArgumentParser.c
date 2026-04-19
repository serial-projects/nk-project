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
    const char* args[] = {"Hello", "World"};
    const int argc = 2;
    NK_ArgumentParser ap;
    NK_ArgumentParserConstruct(&ap, argc, args, NULL);
    NK_ArgumentParserDestruct(&ap);
}

int
main(
    int argc,
    char *argv[]
)
{
    NK_Init();
    NK_Quit();
        
    /** Reached here with no crashes? Good. */
    return 0;
}