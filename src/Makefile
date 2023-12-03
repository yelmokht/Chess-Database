MODULE_big	= chess
OBJS = \
	$(WIN32RES) \
	chess.o \
	chess_btree.o \
	chess_gin.o

EXTENSION   = chess
DATA        = chess--1.0.sql chess.control chess_btree.control chess_gin.control
HEADERS_chess = chess.h

PG_CONFIG ?= pg_config
PGXS = $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)