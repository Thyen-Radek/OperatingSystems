CC = gcc
CFLAGS = -Wall -Wextra

static:
	$(CC) $(CFLAGS) -c library.c
	ar rcs library.a library.o
shared:
	$(CC) $(CFLAGS) -fPIC -c library.c
	$(CC) -shared -fPIC -o library.so library.o
	# $(CC) -shared -fPIC -o ../zad2/library.so library.o
	# $(CC) -shared -fPIC -o ../zad3/library.so library.o
	# $(CC) -shared -fPIC -o ../zad4/library.so library.o
clean:
	rm -rf *.o *.a *.so *.gch
