# Save this as 'Makefile' in the directory with your source
# Type 'make' to compile 'mysignal.c'
# Type 'make clean' to remove the executable and any *.o files
# Type 'info make' to get more information about how make works

all: progra1

progra1: Taylor.c hilos.c principal.c Variables.c
	gcc -o pi Variables.c Taylor.c hilos.c  principal.c  -lm

clean:
	rm -f *.o pi
