MODULE_big	= chess
OBJS = \
	$(WIN32RES) \
	chess.o \
	chess_btree.o \
	chess_gin.o

EXTENSION   = chess
DATA        = chess--1.0.sql
HEADERS_chess = chess.h smallchesslib.h

PG_CONFIG ?= pg_config
PGXS = $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)