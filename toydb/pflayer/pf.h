/* pf.h: externs and error codes for Paged File Interface */

#ifndef PF_H
#define PF_H

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/**************** Error Codes ****************/
#define PFE_OK              0
#define PFE_NOMEM          -1
#define PFE_NOBUF          -2
#define PFE_PAGEFIXED      -3
#define PFE_PAGENOTINBUF   -4
#define PFE_UNIX           -5
#define PFE_INCOMPLETEREAD -6
#define PFE_INCOMPLETEWRITE -7
#define PFE_HDRREAD        -8
#define PFE_HDRWRITE       -9
#define PFE_INVALIDPAGE    -10
#define PFE_FILEOPEN       -11
#define PFE_FTABFULL       -12
#define PFE_FD             -13
#define PFE_EOF            -14
#define PFE_PAGEFREE       -15
#define PFE_PAGEUNFIXED    -16

/* Internal errors */
#define PFE_PAGEINBUF      -17
#define PFE_HASHNOTFOUND   -18
#define PFE_HASHPAGEEXIST  -19

/* Replacement policies (must match pftypes.h) */
#define REPL_LRU 1
#define REPL_MRU 2

/************ PF Statistics Struct ************/
typedef struct {
    long logical_reads;
    long logical_writes;
    long physical_reads;
    long physical_writes;
    long cache_hits;
    long cache_misses;
} PFStats;

/******************* Extern Globals *******************/
extern int PFerrno;
extern PFStats PF_stats;

extern int PF_BUFFER_SIZE;        /* dynamic buffer pool size */
extern int PF_Replacement_Policy; /* LRU (default) / MRU */

/******************* PF Layer API ********************/
extern void PF_Init();
extern void PF_PrintError(char *);

/* Assignment-required additions */
void PF_SetReplacementPolicy(int policy);
void PF_SetBufferSize(int size);

/* mark dirty explicitly */
int PF_MarkDirty(int fd, int pagenum);

/* stats */
void PF_PrintStats(void);
void PF_ResetStats(void);

int PF_CreateFile(char *fname);
int PF_DestroyFile(char *fname);
int PF_OpenFile(char *fname);
int PF_CloseFile(int fd);
int PF_AllocPage(int fd, int *pagenum, char **pagebuf);
int PF_GetThisPage(int fd, int pagenum, char **pagebuf);
int PF_UnfixPage(int fd, int pagenum, int dirty);

#endif
