/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

/** NOTE: This is to test if NK can EVEN execute. */
#include "NK/NK.h"

#include <stdio.h>

static
void
P_NK_ValidatorTest()
{
    NK_Validator m_validator;
    NK_ValidatorConstruct(&m_validator, 1024);

    /** We can write things here: */
    
    NK_ValidatorDestruct(&m_validator);
}

int
main(
    int argc,
    char *argv[]
)
{
    printf("\t%s: Calling NK_Init()\n", __FILE__);
    NK_Init();

    /** Test the validator: */
    P_NK_ValidatorTest();

    printf("\t%s: Calling NK_Quit()\n", __FILE__);
    NK_Quit();
    
    /** Reached here with no crashes? Good. */
    return 0;
}