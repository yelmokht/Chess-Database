CC=gcc
CFLAGS=-I /usr/include/postgresql/14/server


test:
	$(CC) $(CFLAGS) -c ../src/chess.c -o ../src/chess.o
	$(CC) $(CFLAGS) -c test.c -o test.o
	$(CC) $(CFLAGS) ../src/chess.o test.o -o test 
clean:
	rm *.o
