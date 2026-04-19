/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

/** NOTE: This is to test if NK can EVEN execute. */
#include "NK/NK.h"
#include <stdio.h>

static
NK_Result
P_TestShowHelloWorldAction(
    NK_ArgumentParser* arg_parser
)
{
    /** Set the argument to be true. */
    printf("Hello World!\n");
    return true;
}

static
NK_Result
P_TestWhatProjectIsThis(
    NK_ArgumentParser* arg_parser
)
{
    /** Set the argument to be true. */
    printf("Yes, this is NK project ;-)\n");
    return true;
}

static
NK_Result
P_TestGrabNextWord(
    NK_ArgumentParser* arg_parser
)
{
    /** Get the next word, if possible */
    const NK_C8* next = NK_ArgumentParserPull(arg_parser);
    NK_Result good = true;
    if(next != NULL)
    {
        printf("Next parameter (or word was: %s)\n", next);
    }
    else
    {
        good = false;
    }
    return good;
}

static
void
Test(
    
)
{
    const char* args[] = {"Hello", "NK", "EatMyWords", "Lasagna"};
    const int argc = 1 + 1 + 1 + 1;
    const NK_C8** args_p=(const NK_C8**)(args);
    NK_ArgumentParser ap;
    NK_ArgumentParserConstruct(&ap, argc, args_p, NULL);
    NK_ArgumentParserAddAction(&ap, "show-hello", P_TestShowHelloWorldAction);
    NK_ArgumentParserAddAction(&ap, "what-project", P_TestWhatProjectIsThis);
    NK_ArgumentParserAddAction(&ap, "grab-next-word", P_TestGrabNextWord);
    NK_ArgumentParserAddLink(&ap, "Hello", "show-hello");
    NK_ArgumentParserAddLink(&ap, "NK", "what-project");
    NK_ArgumentParserAddLink(&ap, "EatMyWords", "grab-next-word");

    /** We do this until the end; */
    while(true)
    {
        if(
            NK_ArgumentParserStep(&ap) != NK_ENUMS_ARGUMENT_PARSER_STATE_RUNNING
        )
        {
            break;
        }
    }

    switch(
        NK_ArgumentParserGetState(&ap)
    )
    {
        case NK_ENUMS_ARGUMENT_PARSER_STATE_FINISHED:
            printf("Parser has finised normally!\n");
            break;
        case NK_ENUMS_ARGUMENT_PARSER_STATE_DIED:
            printf(
                "Parser finished with error = %d\n",
                NK_ArgumentParserGetErrorRegister(&ap)
            );
            break;
        default:
            break;
    }

    NK_AllocatorStatistics m_stats = NK_AllocatorGetCurrentStatistics();
    printf("\t%s: -- Memory Profile for Stress Test [Before Removing Test] --\n", __FILE__);
    printf("\t%s: MAX: %lu\n", __FILE__, m_stats.absolute_max);
    printf("\t%s: ABS (Now): %lu\n", __FILE__, m_stats.absolute_size);
    printf("\t%s: REL (Now): %lu\n", __FILE__, m_stats.relative_size);
    printf("\t%s: Online: %lu\n", __FILE__, m_stats.online_blocks);

    NK_ArgumentParserDestruct(&ap);
}

int
main(
    int argc,
    char *argv[]
)
{
    NK_Init();

    Test();

    NK_Quit();
        
    /** Reached here with no crashes? Good. */
    return 0;
}