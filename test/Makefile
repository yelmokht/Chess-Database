CC=gcc
CCFLAGS=-I C:/msys64/ucrt64/include/postgresql/server/ -I C:/msys64/ucrt64/include/postgresql/server/port/win32/
test: ../src/chess.o test.o
	$(CC) $(CCFLAGS) ../src/chess.o test.o -o test 

../src/chess.o: ../src/chess.c ../src/chess.h
	$(CC) $(CCFLAGS) -c ../src/chess.c -o ../src/chess.o

test.o: test.c
	$(CC) $(CCFLAGS) -c test.c -o test.o

clean:
	rm *.o ../src/*.o