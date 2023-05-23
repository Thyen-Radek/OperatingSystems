CC=gcc
CFLAGS=-Wall
$(info ************ Wykonano zadanie 2 ************)

all:
	make client
	make server

client: client.c
	$(CC) $(CFLAGS) client.c -o client -pthread

server: server.c
	$(CC) $(CFLAGS) server.c -o server -pthread

clean:
	rm -f client server