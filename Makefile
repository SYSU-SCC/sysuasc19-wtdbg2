VERSION=2.4
RELEASE=20190312

# CC  := gcc
# CX := g++

CC  := mpiicc
CX := mpiicpc

BIN := /usr/local/bin

ifeq (0, ${MAKELEVEL})
TIMESTAMP=$(shell date)
endif

ifeq (1, ${DEBUG})
CFLAGS=-g3 -W -Wall -Wno-unused-but-set-variable -O0 -DDEBUG=1 -DVERSION="$(VERSION)" -DRELEASE="$(RELEASE)" -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -mpopcnt -msse4.2
else
CFLAGS=-g3 -w -Wall -Wno-unused-but-set-variable -DVERSION="$(VERSION)" -DRELEASE="$(RELEASE)" -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE  -xHost -O3 -mpopcnt -msse4.2
endif
CFLAGS+= -DLT_STLSORT -DHCH_TIMER

GLIBS=-lm -lrt -lpthread -lz -L./ -llt_sort -lz
GENERIC_SRC=hch_timer.h hch_timer.c mem_share.h chararray.h sort.h list.h pgzf.h  sort.h list.h dna.h thread.h filereader.h filewriter.h bitvec.h bit2vec.h bitsvec.h hashset.h

PROGS=ltsort.a kbm2 wtdbg2 wtdbg-cns wtpoa-cns pgzf

all: $(PROGS)

kbm2: $(GENERIC_SRC) kbm.c kbm.h hch_timer.c  hch_timer.h kbmpoa.h wtpoa.h tripoa.h poacns.h kswx.h ksw.h ksw.c
	$(CC) $(CFLAGS) -o $@ kbm.c ksw.c hch_timer.c $(GLIBS)

wtdbg2: $(GENERIC_SRC) wtdbg.c hch_timer.c  hch_timer.h wtdbg-graph.h wtdbg.h kbm.h kswx.h ksw.h ksw.c kbmpoa.h wtpoa.h tripoa.h poacns.h
	$(CC) $(CFLAGS) -o $@ wtdbg.c ksw.c hch_timer.c $(GLIBS)

wtdbg-cns: $(GENERIC_SRC) wtdbg-cns.c hch_timer.c  hch_timer.h kswx.h ksw.h ksw.c dbgcns.h dagcns.h queue.h general_graph.h
	$(CC) $(CFLAGS) -o wtdbg-cns wtdbg-cns.c ksw.c hch_timer.c $(GLIBS)

wtpoa-cns: $(GENERIC_SRC) wtpoa.h hch_timer.c  hch_timer.h wtpoa-cns.c poacns.h tripoa.h ksw.h ksw.c
	$(CC) $(CFLAGS) -o $@ wtpoa-cns.c ksw.c hch_timer.c $(GLIBS)

pgzf: mem_share.h hch_timer.c  hch_timer.h sort.h list.h thread.h pgzf.h pgzf.c
	$(CC) $(CFLAGS) -o $@ pgzf.c hch_timer.c $(GLIBS)

lt_sort.o:lt_sort.cpp
	# $(CX) $(CFLAGS) -fPIC -static-intel -c lt_sort.cpp -lm -lrt -lpthread -lz -std=c++11 
	$(CX) $(CFLAGS)  -c lt_sort.cpp -lm -lrt -lpthread -lz -std=c++11 

ltsort.a:lt_sort.o
	ar -crv liblt_sort.a lt_sort.o

clean:
	rm -f *.o *.gcda *.gcno *.gcov gmon.out $(PROGS)

clear:
	rm -f *.o *.gcda *.gcno *.gcov gmon.out

install: $(PROGS)
	mkdir -p $(BIN) && cp -fvu $(PROGS) $(BIN)
