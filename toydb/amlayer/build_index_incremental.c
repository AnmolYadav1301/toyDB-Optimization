#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../pflayer/pf.h"
#include "am.h"

#define DATAFILE "idatafile"
#define INDEXFILE "idatafile.1"

int main()
{
    int i, indexFd;
    int key, recId = 1;

    srand(time(NULL));

    PF_Init();
    PF_ResetStats();

    /* Create index (same as bulk version at start) */
    if (AM_CreateIndex(DATAFILE, 1, 'i', sizeof(int)) != AME_OK) {
        AM_PrintError("CreateIndex");
        exit(1);
    }

    /* Open index file using PF layer */
    indexFd = PF_OpenFile(INDEXFILE);
    if (indexFd < 0) {
        PF_PrintError("PF_OpenFile");
        exit(1);
    }

    /* Prepare CSV output */
    FILE *out = fopen("index_incremental_results.csv", "w");
    if (!out) {
        perror("index_incremental_results.csv");
        exit(1);
    }

    fprintf(out,
        "recordsInserted,logicalReads,logicalWrites,"
        "physicalReads,physicalWrites,cacheHits,cacheMisses\n"
    );

    /* Insert 10,000 keys INCREMENTALLY */
    for (i = 1; i <= 10000; i++) {

        key = rand() % 100000;

        if (AM_InsertEntry(indexFd, 'i', sizeof(int),
                           (char *)&key, recId) != AME_OK) {
            AM_PrintError("InsertEntry");
            exit(1);
        }

        recId++;

        /* Log PF statistics every 500 inserts */
        if (i % 500 == 0) {
            fprintf(out, "%d,%ld,%ld,%ld,%ld,%ld,%ld\n",
                i,
                PF_stats.logical_reads,
                PF_stats.logical_writes,
                PF_stats.physical_reads,
                PF_stats.physical_writes,
                PF_stats.cache_hits,
                PF_stats.cache_misses
            );
        }
    }

    fclose(out);

    PF_CloseFile(indexFd);

    printf("Incremental index creation completed.\n");
    printf("Results saved in index_incremental_results.csv\n");
    return 0;
}
