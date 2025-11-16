#include <string.h>
#include "slotted.h"

#ifndef PF_PAGE_SIZE
#define PF_PAGE_SIZE 4096
#endif

int SL_InitPage(char *pagebuf){
    SL_PageHeader *h = (SL_PageHeader*)pagebuf;
    h->free_offset = SL_HEADER_SIZE;
    h->slot_count = 0;
    h->free_space = PF_PAGE_SIZE - SL_HEADER_SIZE;
    h->reserved = 0;
    /* zero rest */
    memset(pagebuf + SL_HEADER_SIZE, 0, PF_PAGE_SIZE - SL_HEADER_SIZE);
    return 0;
}

/* compute pointer to slot index i (0-based) */
static SL_Slot* slot_ptr(char *pagebuf, int i){
    /* slot array grows from the page end backwards */
    return (SL_Slot*)(pagebuf + PF_PAGE_SIZE - (i+1) * sizeof(SL_Slot));
}

int SL_InsertRecord(char *pagebuf, const void *rec, int reclen, int *slotid){
    SL_PageHeader *h = (SL_PageHeader*)pagebuf;
    int slotsize = (h->slot_count + 1) * sizeof(SL_Slot);
    int used_records = h->free_offset - SL_HEADER_SIZE; /* bytes used by records */
    int used_total = SL_HEADER_SIZE + used_records + slotsize;
    if (used_total + reclen > PF_PAGE_SIZE) return -1; /* no space */

    int off = h->free_offset;
    memcpy(pagebuf + off, rec, reclen);

    SL_Slot *s = slot_ptr(pagebuf, h->slot_count);
    s->offset = off;
    s->length = reclen;

    *slotid = h->slot_count;
    h->slot_count++;
    h->free_offset += reclen;
    h->free_space = PF_PAGE_SIZE - (SL_HEADER_SIZE + (h->free_offset - SL_HEADER_SIZE) + h->slot_count * sizeof(SL_Slot));
    return 0;
}

int SL_GetRecord(char *pagebuf, int slotid, void *outbuf, int *outlen){
    SL_PageHeader *h = (SL_PageHeader*)pagebuf;
    if (slotid < 0 || slotid >= h->slot_count) return -1;
    SL_Slot *s = slot_ptr(pagebuf, slotid);
    if (s->offset < 0) return -1;
    memcpy(outbuf, pagebuf + s->offset, s->length);
    *outlen = s->length;
    return 0;
}

int SL_DeleteRecord(char *pagebuf, int slotid){
    SL_PageHeader *h = (SL_PageHeader*)pagebuf;
    if (slotid < 0 || slotid >= h->slot_count) return -1;
    SL_Slot *s = slot_ptr(pagebuf, slotid);
    s->offset = -1; s->length = 0;
    /* lazy deletion; compaction optional */
    return 0;
}

int SL_PageUtilization(char *pagebuf){
    SL_PageHeader *h = (SL_PageHeader*)pagebuf;
    int slotdir = h->slot_count * sizeof(SL_Slot);
    int records = h->free_offset - SL_HEADER_SIZE;
    return SL_HEADER_SIZE + slotdir + records;
}

/* ===========================================================
   Return amount of free space left in the slotted page.
   =========================================================== */
int SL_GetFreeSpace(char *pagebuf)
{
    SL_PageHeader *hdr = (SL_PageHeader *) pagebuf;

    int header = SL_HEADER_SIZE;
    int slots_size = hdr->slot_count * sizeof(SL_Slot);
    int used = header + slots_size + hdr->free_offset;

    int freeSpace = PF_PAGE_SIZE - used;
    if (freeSpace < 0) freeSpace = 0;

    return freeSpace;
}

