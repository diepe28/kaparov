SRCFIFO = $(SRCDIR)/ServidorFIFO.c $(SRCDIR)/http/ServidorHttp.c
SRCTHREADED = $(SRCDIR)/ServidorThreaded.c $(SRCDIR)/http/ServidorHttp.c
SRCCLIENTE = $(SRCDIR)/cliente.c $(SRCDIR)/cliente_socket_functions.c $(SRCDIR)/http/ProtocoloHttp.c
SRCDIR = .
BINDIR = bin
#CFLAGS = -std=gnu99 -pedantic -g -Wall

all: fifo cliente threaded

fifo: $(SRCFIFO)
	gcc $(CFLAGS) -o $(BINDIR)/servidorFIFO $(SRCFIFO)

threaded: $(SRCTHREADED)
	gcc $(CFLAGS) -o $(BINDIR)/servidorThreaded $(SRCTHREADED) -lpthread


cliente: $(SRCCLIENTE)
	gcc -o $(BINDIR)/cliente $(SRCDIR)/cliente.c $(SRCDIR)/http/ProtocoloHttp.c $(SRCDIR)/http/ClienteHttp.c -lpthread -lm

clean:
	rm -f $(BINDIR)/*.o $(BINDIR)/servidorFIFO $(BINDIR)/cliente $(BINDIR)/prueba

