chess:
	./src/chess

test: chess
	./test/test

clean:
	rm -f ./src/*.o ./src/*.bc ./src/*.so
	dropdb chess