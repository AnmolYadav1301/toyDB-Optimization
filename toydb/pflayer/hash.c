/* hash.c: Hash table for locating buffer pages */
#include <stdio.h>
#include <stdlib.h>
#include "pf.h"
#include "pftypes.h"

/* Actual hash table */
static PFhash_entry *PFhashtbl[PF_HASH_TBL_SIZE];

/* ----------------------- INTERNAL HASH FUNCTION ----------------------- */
static int hashfn(int fd, int page)
{
    /* same as original PFhash(fd,page) but implemented explicitly */
    return (fd + page) % PF_HASH_TBL_SIZE;
}

/* ----------------------- INITIALIZE HASH ------------------------------ */
void PFhashInit()
{
    int i;
    for (i = 0; i < PF_HASH_TBL_SIZE; i++)
        PFhashtbl[i] = NULL;
}

/* ----------------------- HASH FIND ----------------------------------- */
PFbpage *PFhashFind(int fd, int page)
{
    int bucket = hashfn(fd, page);
    PFhash_entry *entry;

    for (entry = PFhashtbl[bucket]; entry != NULL; entry = entry->nextentry)
        if (entry->fd == fd && entry->page == page)
            return entry->bpage;

    return NULL;
}

/* ----------------------- HASH INSERT -------------------------------- */
int PFhashInsert(int fd, int page, PFbpage *bpage)
{
    int bucket = hashfn(fd, page);
    PFhash_entry *entry;

    /* already exists? */
    for (entry = PFhashtbl[bucket]; entry != NULL; entry = entry->nextentry)
        if (entry->fd == fd && entry->page == page)
            return PFE_HASHPAGEEXIST;

    entry = (PFhash_entry *)malloc(sizeof(PFhash_entry));
    if (entry == NULL)
        return PFE_NOMEM;

    entry->fd = fd;
    entry->page = page;
    entry->bpage = bpage;

    /* insert at head */
    entry->nextentry = PFhashtbl[bucket];
    entry->preventry = NULL;

    if (PFhashtbl[bucket] != NULL)
        PFhashtbl[bucket]->preventry = entry;

    PFhashtbl[bucket] = entry;

    return PFE_OK;
}

/* ----------------------- HASH DELETE -------------------------------- */
int PFhashDelete(int fd, int page)
{
    int bucket = hashfn(fd, page);
    PFhash_entry *entry;

    for (entry = PFhashtbl[bucket]; entry != NULL; entry = entry->nextentry)
        if (entry->fd == fd && entry->page == page)
            break;

    if (entry == NULL)
        return PFE_HASHNOTFOUND;

    /* fix linked list pointers */
    if (entry->preventry != NULL)
        entry->preventry->nextentry = entry->nextentry;
    else
        PFhashtbl[bucket] = entry->nextentry;

    if (entry->nextentry != NULL)
        entry->nextentry->preventry = entry->preventry;

    free(entry);
    return PFE_OK;
}

/* ----------------------- HASH DEBUG PRINT ----------------------------- */
void PFhashPrint()
{
    int i;
    PFhash_entry *entry;

    for (i = 0; i < PF_HASH_TBL_SIZE; i++) {
        printf("Bucket %d:\n", i);
        if (PFhashtbl[i] == NULL) {
            printf("\tEMPTY\n");
        } else {
            for (entry = PFhashtbl[i]; entry != NULL; entry = entry->nextentry)
                printf("\tfd=%d page=%d bpage=%p\n",
                       entry->fd, entry->page, entry->bpage);
        }
    }
}
