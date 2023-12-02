SRC_DIR=src
TEST_DIR=test
EXTENSION=chess
DATABASE=chess

chess:
	$(SRC_DIR)/$(EXTENSION)


test: chess
	psql $(DATABASE) < $(TEST_DIR)/small.sql
	psql $(DATABASE) < $(TEST_DIR)/test.sql
	dropdb $(DATABASE)

small_tests: chess
	psql $(DATABASE) < $(TEST_DIR)/small.sql
	psql $(DATABASE) < $(TEST_DIR)/$(EXTENSION)-test.sql
	dropdb $(DATABASE)

large_tests: chess
	psql $(DATABASE) < $(TEST_DIR)/Carlsen.sql
	psql $(DATABASE) < $(TEST_DIR)/$(EXTENSION)-test.sql
	dropdb $(DATABASE)

clean:
	rm -f $(SRC_DIR)/*.o $(SRC_DIR)/*.bc $(SRC_DIR)/*.so
	dropdb $(DATABASE)