SRCFIFO = $(SRCDIR)/ServidorFIFO.c $(SRCDIR)/http/ServidorHttp.c
SRCCLIENTE = $(SRCDIR)/cliente.c $(SRCDIR)/cliente_socket_functions.c $(SRCDIR)/http/ProtocoloHttp.c
SRCDIR = .
BINDIR = bin
CFLAGS = -std=gnu99 -pedantic -g -Wall

all: fifo cliente

fifo: $(SRCFIFO)
	gcc $(CFLAGS) -o $(BINDIR)/servidorFIFO $(SRCFIFO)

cliente: $(SRCCLIENTE)
	gcc -o $(BINDIR)/cliente $(SRCDIR)/cliente.c $(SRCDIR)/http/ProtocoloHttp.c -lpthread -lm

clean:
	rm -f $(BINDIR)/*.o $(BINDIR)/servidorFIFO $(BINDIR)/cliente $(BINDIR)/prueba

