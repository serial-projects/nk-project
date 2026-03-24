/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

/** NOTE: This is to test if NK can EVEN execute. */
#include "NK/NK.h"

#include <stdio.h>

static
void
P_NK_ValidatorTestAnyLevelCallbackFunction(
    NK_ValidatorContent* content,
    const NK_C8* message
)
{
    printf(
        "\t[%.8lu]: %s\n",
        (NK_U64)(content->buffer_counter),
        message
    );
}

static
void
P_NK_ValidatorTestWarningSpecificLevelCallbackFunction(
    NK_ValidatorContent* content,
    const NK_C8* message
)
{
    printf(
        "\t[%f]: WARNING: %s\n",
        (float)(
            (float)(content->buffer_counter) / (float)(content->buffer_limit)
        ),
        message
    );
}

static
void
P_NK_ValidatorTest()
{
    NK_Validator m_validator;
    NK_ValidatorConstruct(&m_validator, 1024);

    /** We can write things here: */
    NK_ValidatorAddListener(
        &m_validator,
        NK_ENUMS_VALIDATOR_LEVEL_LOG,
        P_NK_ValidatorTestAnyLevelCallbackFunction
    );

    NK_ValidatorPushMessage(
        &m_validator,
        NK_ENUMS_VALIDATOR_LEVEL_LOG,
        "Hello! This should go behind me!"
    );

    NK_ValidatorPushMessage(
        &m_validator,
        NK_ENUMS_VALIDATOR_LEVEL_LOG,
        "I am here!"
    );

    NK_ValidatorAddListener(
        &m_validator,
        NK_ENUMS_VALIDATOR_LEVEL_WARNING,
        P_NK_ValidatorTestWarningSpecificLevelCallbackFunction
    );

    const NK_U64 limit = 1024;
    for(NK_U64 index = 0; index < limit; index++)
    {
        NK_ValidatorPushMessage(
            &m_validator,
            NK_ENUMS_VALIDATOR_LEVEL_LOG,
            "Counting from %lu to %lu",
            (index),
            (limit - 1)
        );
    }

    NK_ValidatorPushMessage(
        &m_validator,
        NK_ENUMS_VALIDATOR_LEVEL_WARNING,
        "Some message here!"
    );

    /** How much memory we are using? */


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