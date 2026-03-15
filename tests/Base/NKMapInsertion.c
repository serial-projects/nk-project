/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

/** NOTE: This is to test if NK can EVEN execute. */
#include "NK/NK.h"

#include <stdio.h>
#include <time.h>

static
NK_Result
P_main_PerformStressTestDeleteSomeIterator(
    NK_Map* map,
    const NK_C8* key,
    const NK_Size index,
    void* data,
    void* userdata
)
{
    if((index % 0xF) == 0)
    {
        /** Remove this iteration: */
        NK_MapRemove(
            map,
            key
        );
    }
    return true;
}

static
void
P_main_PerformStressTest(
    const NK_Size limits
)
{
    NK_Map m_map;
    NK_MapConstruct(&m_map, 1024, sizeof(NK_U64));

    NK_U64 bruh = 0;
    NK_Size index = 0;
    NK_Size sub_index = 0;

    /** Simple: */
    NK_MapInsertOrAssign(
        &m_map,
        "ruka",
        &bruh
    );
    NK_U64* maybe_ruka = (NK_U64*)(NK_MapGet(&m_map, "ruka"));
    printf("\t%s: RUKA = %p\n", __FILE__, maybe_ruka);

    /** Random BS String: */
    const NK_Size bs_string_size = 125;
    NK_C8* bs_string = 
        (NK_C8*)(
            NK_AllocatorGet(sizeof(NK_C8) * (bs_string_size + 1))
        );
    NK_RedirectMemset(
        bs_string,
        0,
        sizeof(NK_C8) * (bs_string_size + 1)
    );

    /** We start: */
    srand(time(NULL));
    for(index; index < limits; index++)
    {
        sub_index = 0;
        for(sub_index; sub_index < bs_string_size; sub_index++)
        {
            bs_string[sub_index] = 'z' - (rand() % ('z' - 'A'));
        }
        NK_MapInsertOrAssign(
            &m_map,
            bs_string,
            (void*)(&bruh)
        );

        void* maybe_bruh = 
            NK_MapGet(
                &m_map,
                bs_string
            );
        if(*(NK_U64*)(maybe_bruh) != bruh)
        {
            NK_Panic("horrible error, maybe_bruh is not bruh!");
        }
        bruh++;
    }

    /** NOTE: We delete some using this iterator: */

    NK_AllocatorStatistics m_stats = NK_AllocatorGetCurrentStatistics();
    printf("\t%s: -- Memory Profile for Stress Test [Before Removing Test] --\n", __FILE__);
    printf("\t%s: MAX: %lu\n", __FILE__, m_stats.absolute_max);
    printf("\t%s: ABS (Now): %lu\n", __FILE__, m_stats.absolute_size);
    printf("\t%s: REL (Now): %lu\n", __FILE__, m_stats.relative_size);
    printf("\t%s: Online: %lu\n", __FILE__, m_stats.online_blocks);

    NK_MapIterate(
        &m_map,
        P_main_PerformStressTestDeleteSomeIterator,
        NULL
    );

    /** Do it all again! */
    bruh = 0;
    index = 0;
    sub_index = 0;
    for(index; index < limits; index++)
    {
        sub_index = 0;
        for(sub_index; sub_index < bs_string_size; sub_index++)
        {
            bs_string[sub_index] = 'z' - (rand() % ('z' - 'A'));
        }
        NK_MapInsertOrAssign(
            &m_map,
            bs_string,
            (void*)(&bruh)
        );

        void* maybe_bruh = 
            NK_MapGet(
                &m_map,
                bs_string
            );
        if(*(NK_U64*)(maybe_bruh) != bruh)
        {
            NK_Panic("horrible error, maybe_bruh is not bruh!");
        }
        bruh++;
    }

    m_stats = NK_AllocatorGetCurrentStatistics();
    printf("\t%s: -- Memory Profile for Stress Test [Before Removing Test] --\n", __FILE__);
    printf("\t%s: MAX: %lu\n", __FILE__, m_stats.absolute_max);
    printf("\t%s: ABS (Now): %lu\n", __FILE__, m_stats.absolute_size);
    printf("\t%s: REL (Now): %lu\n", __FILE__, m_stats.relative_size);
    printf("\t%s: Online: %lu\n", __FILE__, m_stats.online_blocks);

    NK_AllocatorFree(bs_string);
    NK_MapDestruct(&m_map);
}

int
main(
    int argc,
    char* argv[]
)
{
    NK_Init();

    /** Do some stress ;-) */
    P_main_PerformStressTest(100000);

    NK_Quit();
    return 0;
}    