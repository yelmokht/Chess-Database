chess:
	./src/chess

test: chess
	./test/test

test_import: chess test
	./test/test_import

clean:
	rm -f ./src/*.o ./src/*.bc ./src/*.so
	dropdb chess