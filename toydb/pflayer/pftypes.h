/* pftypes.h: internal types for PF layer */

#ifndef PFTYPES_H
#define PFTYPES_H

/************** Constants *****************/

#define PF_PAGE_SIZE 4096
#define PF_MAX_BUFS 20
#define PF_FTAB_SIZE 20
#define PF_HASH_TBL_SIZE 20

#define PF_PAGE_LIST_END -1
#define PF_PAGE_USED     -2

/************** Replacement policies **************/
#define REPL_LRU 1
#define REPL_MRU 2

/************** File page (on-disk format) **************/
typedef struct PFfpage {
    int nextfree;
    char pagebuf[PF_PAGE_SIZE];
} PFfpage;

/************** Buffer page entry **************/
typedef struct PFbpage {
    int fd;             /* file descriptor */
    int page;           /* page number */
    int fixed;          /* pin count */
    int dirty;          /* dirty flag */

    PFfpage fpage;      /* actual page data */

    struct PFbpage *nextpage;
    struct PFbpage *prevpage;
} PFbpage;

/************** File table entry **************/
typedef struct PFhdr_str {
    int firstfree;
    int numpages;
} PFhdr_str;

/* header size constant (needed by pf.c) */
#define PF_HDR_SIZE sizeof(PFhdr_str)

typedef struct PFftab_ele {
    int unixfd;
    PFhdr_str hdr;
    int hdrchanged;
    char *fname;
} PFftab_ele;

/************** Hash table entry **************/
typedef struct PFhash_entry {
    struct PFhash_entry *nextentry;
    struct PFhash_entry *preventry;
    int fd;
    int page;
    PFbpage *bpage;
} PFhash_entry;

/************** Hash table API **************/
extern void PFhashInit();
extern PFbpage *PFhashFind(int fd, int page);
extern int PFhashInsert(int fd,int page,PFbpage *bpage);
extern int PFhashDelete(int fd,int page);

#endif
