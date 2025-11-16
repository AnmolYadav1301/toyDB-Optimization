#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "pf.h"
#include "pftypes.h"

#define FNAME "expfile"

void run_experiment(int policy, int writePct, int nPages, int accesses,
                    int bufferSize, FILE *out)
{
    int i, fd, pnum;
    char *buf;

    PF_SetBufferSize(bufferSize);
    PF_SetReplacementPolicy(policy);
    PF_ResetStats();

    if (PF_CreateFile(FNAME) != PFE_OK) {
        PF_PrintError("create expfile");
        exit(1);
    }

    fd = PF_OpenFile(FNAME);
    if (fd < 0) {
        PF_PrintError("open");
        exit(1);
    }

    /* Allocate nPages pages */
    for (i = 0; i < nPages; i++) {
        if (PF_AllocPage(fd, &pnum, &buf) != PFE_OK) {
            PF_PrintError("alloc");
            exit(1);
        }
        buf[0] = 0;
        PF_UnfixPage(fd, pnum, TRUE);
    }

    /* Random workload */
    for (i = 0; i < accesses; i++) {
        int page = rand() % nPages;

        if (PF_GetThisPage(fd, page, &buf) != PFE_OK) {
            PF_PrintError("getthis");
            exit(1);
        }

        if ((rand() % 100) < writePct) {
            buf[0]++;
            PF_UnfixPage(fd, page, TRUE);
        } else {
            PF_UnfixPage(fd, page, FALSE);
        }
    }

    /* Write CSV */
    fprintf(out,
        "%d,%d,%d,%d,%ld,%ld,%ld,%ld,%ld,%ld\n",
        policy,
        bufferSize,
        nPages,
        writePct,
        PF_stats.logical_reads,
        PF_stats.logical_writes,
        PF_stats.physical_reads,
        PF_stats.physical_writes,
        PF_stats.cache_hits,
        PF_stats.cache_misses
    );

    PF_CloseFile(fd);
    PF_DestroyFile(FNAME);
}

int main()
{
    FILE *out;
    int p, m;

    out = fopen("pf_results.csv", "w");
    if (!out) {
        perror("pf_results.csv");
        return 1;
    }

    fprintf(out,
        "policy,bufferSize,nPages,writePct,"
        "logicalReads,logicalWrites,"
        "physicalReads,physicalWrites,"
        "cacheHits,cacheMisses\n"
    );

    srand((unsigned)time(NULL));

    {
        int policies[2] = {REPL_LRU, REPL_MRU};
        int mixes[6] = {0, 10, 30, 50, 70, 90};
        int bsize = 20;
        int nPages = 200;
        int accesses = 20000;

        for (p = 0; p < 2; p++) {
            for (m = 0; m < 6; m++) {
                run_experiment(policies[p], mixes[m], nPages,
                               accesses, bsize, out);
                fflush(out);
            }
        }
    }

    fclose(out);
    printf("Experiment finished → pf_results.csv\n");
    return 0;
}
