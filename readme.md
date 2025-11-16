# toyDB-Optimization — README

This repository contains an educational toy DBMS with three core layers:

- **PF Layer** — Paged File Manager + Buffer Manager (LRU/MRU)
- **Slotted Page Layer** — Record storage within a page
- **AM Layer** — B+ Tree secondary index
- **Experiments** — PF experiments, slotted page analysis, bulk & incremental index build

This README provides the exact commands to build and run the entire project.

---

## Project Layout
toyDB-Optimization/
│
├── pflayer/
│   ├── pf.c, buf.c, hash.c
│   ├── slotted.c, slotted.h
│   ├── experiment.c                # PF Layer experiment
│   ├── slotted_analysis.c          # Slotted page experiment
│   ├── pf_results.csv
│   ├── slotted_results.csv
│   ├── plot.py
│
├── amlayer/
│   ├── am.c, amfns.c, aminsert.c
│   ├── amsearch.c, amscan.c, amstack.c
│   ├── amprint.c, amglobals.c
│   ├── build_index_bulk.c
│   ├── build_index_incremental.c
│   ├── index_bulk_results.csv
│   ├── index_incremental_results.csv
│
├── data/
├── README.md
└── .gitignore


## Compiler Flags Used Everywhere

CFLAGS="-std=gnu89
-Wno-int-conversion -Wno-implicit-int -Wno-old-style-definition
-Wno-builtin-declaration-mismatch -Wno-implicit-function-declaration
-Wno-pointer-to-int-cast -Wno-return-type"


###############################################################################
# GLOBAL FLAGS
###############################################################################
CFLAGS="-std=gnu89 \
-Wno-int-conversion -Wno-implicit-int -Wno-old-style-definition \
-Wno-builtin-declaration-mismatch -Wno-implicit-function-declaration \
-Wno-pointer-to-int-cast -Wno-return-type"

###############################################################################
# 1. PF LAYER BUILD
###############################################################################
cc $CFLAGS -I./pflayer -c pflayer/buf.c -o pflayer/buf.o
cc $CFLAGS -I./pflayer -c pflayer/hash.c -o pflayer/hash.o
cc $CFLAGS -I./pflayer -c pflayer/pf.c  -o pflayer/pf.o

# merge into relocatable PF module
ld -r -o pflayer/pflayer.o pflayer/buf.o pflayer/hash.o pflayer/pf.o

###############################################################################
# 2. PF EXPERIMENT (pf_results.csv)
###############################################################################
cc $CFLAGS -I./pflayer -c pflayer/experiment.c -o pflayer/experiment.o
cc $CFLAGS -I./pflayer pflayer/pflayer.o pflayer/experiment.o -o pf_experiment
./pf_experiment

###############################################################################
# 3. SLOTTED PAGE EXPERIMENT (slotted_results.csv)
###############################################################################
cc $CFLAGS -I./pflayer -c pflayer/slotted.c -o pflayer/slotted.o
cc $CFLAGS -I./pflayer -c pflayer/slotted_analysis.c -o pflayer/slotted_analysis.o

cc $CFLAGS -I./pflayer \
   pflayer/pflayer.o \
   pflayer/slotted.o \
   pflayer/slotted_analysis.o \
   -o slotted_analysis

./slotted_analysis

###############################################################################
# 4. AM LAYER (B+ TREE INDEX) — compile all modules
###############################################################################
cc $CFLAGS -I./pflayer -I./amlayer -c amlayer/am.c -o amlayer/am.o
cc $CFLAGS -I./pflayer -I./amlayer -c amlayer/amfns.c -o amlayer/amfns.o
cc $CFLAGS -I./pflayer -I./amlayer -c amlayer/amsearch.c -o amlayer/amsearch.o
cc $CFLAGS -I./pflayer -I./amlayer -c amlayer/aminsert.c -o amlayer/aminsert.o
cc $CFLAGS -I./pflayer -I./amlayer -c amlayer/amscan.c -o amlayer/amscan.o
cc $CFLAGS -I./pflayer -I./amlayer -c amlayer/amstack.c -o amlayer/amstack.o
cc $CFLAGS -I./pflayer -I./amlayer -c amlayer/amprint.c -o amlayer/amprint.o
cc $CFLAGS -I./pflayer -I./amlayer -c amlayer/amglobals.c -o amlayer/amglobals.o

###############################################################################
# 5. BULK INDEX BUILD (index_bulk_results.csv)
###############################################################################
cc $CFLAGS -I./pflayer -I./amlayer \
    pflayer/pflayer.o \
    amlayer/am.o amlayer/amfns.o amlayer/amsearch.o amlayer/aminsert.o \
    amlayer/amscan.o amlayer/amprint.o amlayer/amstack.o amlayer/amglobals.o \
    amlayer/build_index_bulk.c \
    -o build_index_bulk

./build_index_bulk

###############################################################################
# 6. INCREMENTAL INDEX BUILD (index_incremental_results.csv)
###############################################################################
cc $CFLAGS -I./pflayer -I./amlayer \
    pflayer/pflayer.o \
    amlayer/am.o amlayer/amfns.o amlayer/amsearch.o amlayer/aminsert.o \
    amlayer/amscan.o amlayer/amprint.o amlayer/amstack.o amlayer/amglobals.o \
    amlayer/build_index_incremental.c \
    -o build_index_incremental

./build_index_incremental

###############################################################################
# DONE — CSV outputs generated:
###############################################################################
echo "Generated:"
echo "  pf_results.csv"
echo "  slotted_results.csv"
echo "  index_bulk_results.csv"
echo "  index_incremental_results.csv"
