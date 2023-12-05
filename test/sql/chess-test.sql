\timing on

ANALYZE games;

EXPLAIN ANALYZE SELECT count(*) FROM games WHERE hasOpening(game, '1.e4 c6 '); -- without BTREE index
SELECT count(*) FROM games WHERE hasOpening(game, '1.e4 c6 '); -- without BTREE index
CREATE INDEX index_btree ON games USING btree(game);
EXPLAIN ANALYZE SELECT count(*) FROM games WHERE hasOpening(game, '1.e4 c6 '); -- with BTREE index
SELECT count(*) FROM games WHERE hasOpening(game, '1.e4 c6 '); -- with BTREE index

EXPLAIN ANALYZE SELECT count(*) FROM games WHERE hasBoard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 10); -- without GIN index
SELECT count(*) FROM games WHERE hasBoard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 10); -- without GIN index
CREATE INDEX index_gin ON games USING gin(states);
EXPLAIN ANALYZE SELECT count(*) FROM games WHERE hasBoard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 10); -- with GIN index
SELECT count(*) FROM games WHERE hasBoard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 10) = false; -- with GIN index