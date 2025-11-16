# assignment.mk
# Build slotted-page test + index testers using legacy C flags

PF_DIR = pflayer
AM_DIR = amlayer

CFLAGS = -std=gnu89 \
         -Wno-int-conversion \
         -Wno-implicit-int \
         -Wno-old-style-definition \
         -Wno-builtin-declaration-mismatch \
         -Wno-implicit-function-declaration \
         -Wno-pointer-to-int-cast \
         -Wno-return-type \
         -I$(PF_DIR) -I$(AM_DIR)

# PF objects (actual files in your pflayer directory)
PF_OBJS = $(PF_DIR)/buf.o $(PF_DIR)/hash.o $(PF_DIR)/pf.o

# Build a combined pf layer object (like your reference Makefile)
PF_LAYER = $(PF_DIR)/pflayer.o

# Slotted page object
SLOTTED_OBJ = $(PF_DIR)/slotted.o

# -----------------------------
#  Compile PF layer components
# -----------------------------
$(PF_DIR)/buf.o: $(PF_DIR)/buf.c $(PF_DIR)/pftypes.h $(PF_DIR)/pf.h
	$(CC) $(CFLAGS) -c $< -o $@

$(PF_DIR)/hash.o: $(PF_DIR)/hash.c $(PF_DIR)/pftypes.h $(PF_DIR)/pf.h
	$(CC) $(CFLAGS) -c $< -o $@

$(PF_DIR)/pf.o: $(PF_DIR)/pf.c $(PF_DIR)/pftypes.h $(PF_DIR)/pf.h
	$(CC) $(CFLAGS) -c $< -o $@

$(PF_LAYER): $(PF_OBJS)
	ld -r -o $(PF_LAYER) $(PF_OBJS)

# -----------------------------
#  Slotted Page Test
# -----------------------------
test_slotted: $(PF_LAYER) $(SLOTTED_OBJ) $(PF_DIR)/test_slotted.o
	$(CC) $(CFLAGS) -o test_slotted $(PF_LAYER) $(SLOTTED_OBJ) $(PF_DIR)/test_slotted.o

$(PF_DIR)/slotted.o: $(PF_DIR)/slotted.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PF_DIR)/test_slotted.o: $(PF_DIR)/test_slotted.c
	$(CC) $(CFLAGS) -c $< -o $@

# -----------------------------
#  AM Layer Objects
# -----------------------------
AM_OBJS = $(AM_DIR)/am.o $(AM_DIR)/amfns.o $(AM_DIR)/amsearch.o \
          $(AM_DIR)/amglobals.o $(AM_DIR)/aminsert.o $(AM_DIR)/amscan.o \
          $(AM_DIR)/amprint.o $(AM_DIR)/amstack.o

$(AM_DIR)/%.o: $(AM_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# -----------------------------
#  Build AM executables
# -----------------------------
build_index_bulk: $(PF_LAYER) $(AM_OBJS) $(AM_DIR)/build_index_bulk.o
	$(CC) $(CFLAGS) -o build_index_bulk $(AM_DIR)/build_index_bulk.o $(AM_OBJS) $(PF_LAYER)

build_index_incremental: $(PF_LAYER) $(AM_OBJS) $(AM_DIR)/build_index_incremental.o
	$(CC) $(CFLAGS) -o build_index_incremental $(AM_DIR)/build_index_incremental.o $(AM_OBJS) $(PF_LAYER)

# -----------------------------
clean:
	rm -f $(PF_DIR)/*.o $(AM_DIR)/*.o $(PF_LAYER) \
	test_slotted build_index_bulk build_index_incremental
