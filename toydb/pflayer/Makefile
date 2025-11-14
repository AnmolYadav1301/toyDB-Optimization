CFLAGS = -std=gnu89 -Wno-int-conversion -Wno-implicit-int \
         -Wno-old-style-definition -Wno-builtin-declaration-mismatch \
         -Wno-implicit-function-declaration -Wno-pointer-to-int-cast

SRC = buf.c hash.c pf.c
OBJ = buf.o hash.o pf.o
HDR = pftypes.h pf.h

all: clean pflayer.o testpf testhash

buf.o: buf.c $(HDR)
	cc $(CFLAGS) -c buf.c

pf.o: pf.c $(HDR)
	cc $(CFLAGS) -c pf.c

hash.o: hash.c $(HDR)
	cc $(CFLAGS) -c hash.c

testpf.o: testpf.c $(HDR)
	cc $(CFLAGS) -c testpf.c

testhash.o: testhash.c $(HDR)
	cc $(CFLAGS) -c testhash.c

pflayer.o: $(OBJ)
	ld -r -o pflayer.o $(OBJ)

testpf: testpf.o pflayer.o
	cc $(CFLAGS) -o testpf testpf.o pflayer.o

testhash: testhash.o pflayer.o
	cc $(CFLAGS) -o testhash testhash.o pflayer.o

clean:
	rm -f *.o pflayer.o testpf testhash
