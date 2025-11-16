#include <stdio.h>
#include <string.h>

#include "pftypes.h"   // for PF_PAGE_SIZE
#include "slotted.h"   // for slotted page interface

int main() {
    printf("Running slotted page test...\n");

    // allocate one PF page (same structure as PF layer uses)
    PFfpage page;

    // Initialize the slotted page inside page.pagebuf
    SL_InitPage(page.pagebuf);

    int id1, id2, id3;

    SL_InsertRecord(page.pagebuf, "hello", 5, &id1);
    SL_InsertRecord(page.pagebuf, "world", 5, &id2);
    SL_InsertRecord(page.pagebuf, "toydb", 5, &id3);

    printf("Inserted slot IDs: %d, %d, %d\n", id1, id2, id3);

    char buffer[128];
    int outlen;

    SL_GetRecord(page.pagebuf, id1, buffer, &outlen);
    buffer[outlen] = '\0';
    printf("rec1 = %s\n", buffer);

    SL_GetRecord(page.pagebuf, id2, buffer, &outlen);
    buffer[outlen] = '\0';
    printf("rec2 = %s\n", buffer);

    SL_GetRecord(page.pagebuf, id3, buffer, &outlen);
    buffer[outlen] = '\0';
    printf("rec3 = %s\n", buffer);

    printf("Slotted page test complete.\n");
    return 0;
}
