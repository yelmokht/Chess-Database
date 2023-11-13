all: clean run

run: using_scl
	./using_scl

using_scl: using_scl.c
	gcc -o using_scl.out using_scl.c

clean:
	rm -rf *.out

valgrind:
	valgrind --leak-check=full --track-origins=yes ./using_scl

chess_utils: clean
	gcc -c ../src/chess.c
	gcc -c ../src/chess_utils.c
	gcc -o chess ./chess_utils.o