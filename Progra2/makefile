
# Save this as 'Makefile' in the directory with your source
# Type 'make' to compile 'mysignal.c'
# Type 'make clean' to remove the executable and any *.o files
# Type 'info make' to get more information about how make works

all: cliente

cliente: cliente.c cliente_socket_functions.c
	gcc -o cliente cliente.c -lpthread -lm

clean:
	rm -f *.o prueba
