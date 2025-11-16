#ifndef SLOTTED_H
#define SLOTTED_H

#include "pf.h"
#include <stdint.h>

#define SL_HEADER_SIZE 8     /* adjust if you change header fields */

typedef struct {
    int16_t free_offset;    /* offset where next record will be placed (grows up) */
    int16_t slot_count;     /* number of slots allocated (including deleted) */
    int16_t free_space;     /* cached free space (optional) */
    int16_t reserved;
} SL_PageHeader;

typedef struct {
    int16_t offset;         /* -1 => deleted */
    int16_t length;
} SL_Slot;

/* API */
int SL_InitPage(char *pagebuf); /* initialize page buffer in memory */
int SL_InsertRecord(char *pagebuf, const void *rec, int reclen, int *slotid);
int SL_GetRecord(char *pagebuf, int slotid, void *outbuf, int *outlen);
int SL_DeleteRecord(char *pagebuf, int slotid);
int SL_PageUtilization(char *pagebuf); /* returns used bytes (header+slots+records) */

#endif
