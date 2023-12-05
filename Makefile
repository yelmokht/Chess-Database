SRC_DIR=src
SQL_DIR=test/sql
EXTENSION=chess
DATABASE=chess

chess:
	$(SRC_DIR)/$(EXTENSION)

small_tests: chess
	psql $(DATABASE) < $(SQL_DIR)/small.sql
	psql $(DATABASE) < $(SQL_DIR)/$(EXTENSION)-test.sql

large_tests: chess
	psql $(DATABASE) < $(SQL_DIR)/large.sql
	psql $(DATABASE) < $(SQL_DIR)/$(EXTENSION)-test.sql

clean:
	rm -f $(SRC_DIR)/*.o $(SRC_DIR)/*.bc $(SRC_DIR)/*.so
	dropdb $(DATABASE)