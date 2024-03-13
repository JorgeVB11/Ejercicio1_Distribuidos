#MakeFile
CC=gcc
CFLAGS=-g -Wall

CPPFLAGS = -I$(INSTALL_PATH)/include -Wall

LDFLAGS = -L$(INSTALL_PATH)/lib/

.PHONY: all clean
cliente: cliente.c libclaves.so
	$(CC) $(CFLAGS) -o cliente cliente.c -lclaves -L.
	env LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. ./cliente

libclaves.so: lib_claves.c
	
	$(CC) $(CFLAGS) -fPIC -c lib_claves.c -o lib_claves.o
	$(CC) -shared -Wl,-soname,libclaves.so -o libclaves.so.1.0 lib_claves.o
	ln -s libclaves.1.0 libclaves.so

servidor: servidor.o
	$(CC) $(CFLAGS) -o servidor servidor.o

servidor.o: servidor.c 
	$(CC) $(CFLAGS) -c servidor.c -o servidor.o

clean:
	rm -fr *.o
	rm -fr *.so*