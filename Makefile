SRC_DIR=src
SQL_DIR=test/sql
EXTENSION=chess
DATABASE=chess

chess:
	$(SRC_DIR)/$(EXTENSION)


test: chess
	psql $(DATABASE) < $(SQL_DIR)/small.sql
	psql $(DATABASE) < $(SQL_DIR)/test.sql

small_tests: chess
	psql $(DATABASE) < $(SQL_DIR)/small.sql
	psql $(DATABASE) < $(SQL_DIR)/$(EXTENSION)-test.sql

large_tests: chess
	psql $(DATABASE) < $(SQL_DIR)/Carlsen.sql
	psql $(DATABASE) < $(SQL_DIR)/$(EXTENSION)-test.sql

clean:
	rm -f $(SRC_DIR)/*.o $(SRC_DIR)/*.bc $(SRC_DIR)/*.so
	dropdb $(DATABASE)