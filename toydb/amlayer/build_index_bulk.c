#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../pflayer/pf.h"
#include "am.h"

#define DATAFILE  "bdatafile"
#define INDEXFILE "bdatafile.1"

int main()
{
    int i, indexFd;
    int key, recId = 1;

    srand(time(NULL));

    /* Initialize PF */
    PF_Init();
    PF_ResetStats();

    /* Create index file */
    if (AM_CreateIndex(DATAFILE, 1, 'i', sizeof(int)) != AME_OK) {
        AM_PrintError("CreateIndex");
        exit(1);
    }

    /* Open index file using PF layer (same as your incremental code) */
    indexFd = PF_OpenFile(INDEXFILE);
    if (indexFd < 0) {
        PF_PrintError("PF_OpenFile");
        exit(1);
    }

    /* Prepare CSV */
    FILE *out = fopen("index_bulk_results.csv", "w");
    if (!out) {
        perror("index_bulk_results.csv");
        exit(1);
    }

    fprintf(out,
        "recordsInserted,logicalReads,logicalWrites,"
        "physicalReads,physicalWrites,cacheHits,cacheMisses\n"
    );

    /* BULK INSERT: insert 10,000 entries */
    for (i = 1; i <= 10000; i++) {

        key = rand() % 100000;

        if (AM_InsertEntry(indexFd, 'i', sizeof(int),
                           (char *)&key, recId) != AME_OK)
        {
            AM_PrintError("InsertEntry");
            exit(1);
        }

        recId++;
    }

    /* Log only once after full bulk load */
    fprintf(out, "%d,%ld,%ld,%ld,%ld,%ld,%ld\n",
        10000,
        PF_stats.logical_reads,
        PF_stats.logical_writes,
        PF_stats.physical_reads,
        PF_stats.physical_writes,
        PF_stats.cache_hits,
        PF_stats.cache_misses
    );

    fclose(out);

    PF_CloseFile(indexFd);

    printf("Bulk index creation completed.\n");
    printf("Results saved in index_bulk_results.csv\n");

    return 0;
}
