SRCFIFO = $(SRCDIR)/ServidorFIFO.c $(SRCDIR)/http/ServidorHttp.c
SRCTHREADED = $(SRCDIR)/ServidorThreaded.c $(SRCDIR)/http/ServidorHttp.c
SRCPRETHREADED = $(SRCDIR)/ServidorPreThreaded.c $(SRCDIR)/http/ServidorHttp.c
SRCFORKED = $(SRCDIR)/ServidorForked.c $(SRCDIR)/http/ServidorHttp.c
SRCPREFORKED = $(SRCDIR)/ServidorPreForked.c $(SRCDIR)/http/ServidorHttp.c
SRCCLIENTE = $(SRCDIR)/cliente.c $(SRCDIR)/cliente_socket_functions.c
SRCDIR = .
BINDIR = bin
#CFLAGS = -std=gnu99 -pedantic -g -Wall

all: fifo cliente threaded prethreaded forked preforked

fifo: $(SRCFIFO)
	gcc $(CFLAGS) -o $(BINDIR)/servidorFIFO $(SRCFIFO)

threaded: $(SRCTHREADED)
	gcc $(CFLAGS) -o $(BINDIR)/servidorThreaded $(SRCTHREADED) -lpthread

prethreaded: $(SRCPRETHREADED)
	gcc $(CFLAGS) -o $(BINDIR)/servidorPreThreaded $(SRCPRETHREADED) -lpthread

forked: $(SRCFORKED)
	gcc $(CFLAGS) -o $(BINDIR)/servidorForked $(SRCFORKED)

preforked: $(SRCPREFORKED)
	gcc $(CFLAGS) -o $(BINDIR)/servidorPreForked $(SRCPREFORKED)

cliente: $(SRCCLIENTE)
	gcc -o $(BINDIR)/cliente $(SRCDIR)/cliente.c $(SRCDIR)/http/ClienteHttp.c -lpthread -lm

clean:
	rm -f $(BINDIR)/*.o $(BINDIR)/servidorFIFO $(BINDIR)/cliente $(BINDIR)/prueba

