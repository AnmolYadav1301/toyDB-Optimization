#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "slotted.h"
#include "pftypes.h"

#define PAGE_SIZE PF_PAGE_SIZE

int main()
{
    FILE *csv = fopen("slotted_results.csv", "w");
    if (!csv) {
        perror("slotted_results.csv");
        return 1;
    }

    fprintf(csv, "record_size,records_inserted,free_space\n");

    /* record sizes */
    int recordSizes[] = {20, 40, 80, 160, 200, 300, 500, 800};
    int nSizes = sizeof(recordSizes) / sizeof(int);

    char page[PAGE_SIZE];
    int i;

    for (i = 0; i < nSizes; i++) {

        SL_InitPage(page);

        /* current record size */
        int recSize = recordSizes[i];
        int count = 0;
        int slot;
        char *buf = malloc(recSize);
        memset(buf, 'A', recSize);

        /* keep inserting until page is full */
        while (SL_InsertRecord(page, buf, recSize, &slot) == 0) {
            count++;
        }

        /* compute free space */
        int freeSpace = SL_GetFreeSpace(page);

        /* write CSV row */
        fprintf(csv, "%d,%d,%d\n", recSize, count, freeSpace);

        free(buf);
    }

    fclose(csv);
    printf("Slotted-page analysis complete. Output: slotted_results.csv\n");
    return 0;
}
