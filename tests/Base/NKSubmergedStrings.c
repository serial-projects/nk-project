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

    NK_SubmergedString m_ss1;
    NK_SubmergedStringConstruct(&m_ss1, "Ruka");

    NK_SubmergedString m_ss2;
    NK_SubmergedStringConstruct(&m_ss2, "Ruka");

    printf(
        "\t%s: Does %s is same handler? [m_ss1 = %ld, m_ss2 = %ld]\n",
        __FILE__,
        NK_SubmergedStringGet(&m_ss1),
        m_ss1.handler,
        m_ss2.handler
    );

    if(m_ss1.handler != m_ss2.handler)
    {
        /** BAD: */
        return -1;
    }

    NK_SubmergedStringSetImmediate(&m_ss1, "Raposa");
    printf(
        "\t%s: %s %s\n",
        __FILE__,
        NK_SubmergedStringGet(&m_ss2),
        NK_SubmergedStringGet(&m_ss1)
    );

    NK_SubmergedString m_ss3;
    NK_SubmergedStringConstruct(&m_ss3, "Mikey");
    printf(
        "\t%s: %s %lu\n",
        __FILE__,
        NK_SubmergedStringGet(&m_ss3),
        m_ss3.handler
    );

    NK_AllocatorStatistics m_stats = NK_AllocatorGetCurrentStatistics();
    printf("\t%s: -- Memory Profile for Stress Test [Before Removing Test] --\n", __FILE__);
    printf("\t%s: MAX: %lu\n", __FILE__, m_stats.absolute_max);
    printf("\t%s: ABS (Now): %lu\n", __FILE__, m_stats.absolute_size);
    printf("\t%s: REL (Now): %lu\n", __FILE__, m_stats.relative_size);
    printf("\t%s: Online: %lu\n", __FILE__, m_stats.online_blocks);

    NK_SubmergedString m_ss4;
    NK_SubmergedStringConstruct(&m_ss4, "Raposa");
    printf("\t%s: m_ss4 = %lu, m_ss1 = %lu\n", __FILE__, m_ss4.handler, m_ss1.handler);

    NK_SubmergedStringDestruct(&m_ss1);
    NK_SubmergedStringDestruct(&m_ss2);
    NK_SubmergedStringDestruct(&m_ss3);

    NK_Quit();
    
    /** Reached here with no crashes? Good. */
    return 0;
}