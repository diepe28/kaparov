SOURCES = $(SRCDIR)/Taylor.c $(SRCDIR)/hilos.c $(SRCDIR)/principal.c $(SRCDIR)/Variables.c \
    $(SRCDIR)/interfaz.c
SRCDIR = .
BINDIR = .
CFLAGS = -std=gnu99 -pedantic -g -Wall -lm -lrt `pkg-config --cflags --libs gtk+-2.0`

all: progra1

progra1: $(SOURCES)
	gcc $(CFLAGS) -o $(BINDIR)/pi \$(SOURCES)

cleanall:
	rm -f $(BINDIR)/*.o $(BINDIR)/pi
