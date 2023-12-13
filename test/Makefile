PGNFOLDER=pgn
PGNFILE=$(PGNFOLDER)/Adams.pgn
PARSER=pgn_to_sql_parser/parser.py

sql_file:
	python3 $(PARSER) $(PGNFILE)

large_sql_file:
	python3 $(PARSER) $(PGNFOLDER)