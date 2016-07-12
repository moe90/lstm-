CC = gcc
CFLAGS = -O3 -Wall -m64

CILKDIR = /usr/local/cilk
CILKC = $(CILKDIR)/bin/cilkc
DEFS = -DHAVE_CONFIG_H -DCILK
INCL = -I. -I$(CILKDIR)/include
CPSTAT = # -cilk-critical-path
ACML = -m64 -I../acml/gfortran64/include -L../acml/gfortran64/lib -lacml -lgfortran

test: test.cilk
	$(CILKC) $(DEFS) $(CFLAGS) $(INCL) $(CPSTAT) $(ACML) -o $@ $(LDFLAGS) $< -lm

.SUFFIXES:  .o .c

.c.o:
	${CC} ${CFLAGS} -c $<

clean:
	rm -f *.o fib

veryclean: clean
	rm -f *~ 



