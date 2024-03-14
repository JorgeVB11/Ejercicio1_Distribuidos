CC = gcc
CFLAGS = -fPIC -Wall -g
LDFLAGS =
LDLIBS =

PROXY_SOURCES = lib_claves.c

PROXY_OBJECTS = $(PROXY_SOURCES:.c=.o)

PROXY = libclaves.so
SERVIDOR = servidor
CLIENT = cliente

all: $(PROXY) $(SERVIDOR) $(CLIENT)

$(PROXY): $(PROXY_OBJECTS)
	$(CC) -shared -o $(PROXY) $(PROXY_OBJECTS) $(LDFLAGS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT): cliente.o libclaves.so
	$(CC) -o $(CLIENT) cliente.o $(LDFLAGS) $(LDLIBS) -lrt -L. -lclaves -Wl,-rpath,.

$(SERVIDOR): servidor.o 
	$(CC) -g -o $(SERVIDOR) servidor.o

clean:
	rm -f $(PROXY) $(PROXY_OBJECTS) $(CLIENT) $(SERVIDOR) *.o

re: clean all