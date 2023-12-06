\timing on

ANALYZE games;

SET enable_seqscan = off;

CREATE INDEX index_btree ON games USING btree(game);

EXPLAIN ANALYZE SELECT * FROM games WHERE hasOpening(game, '1.e4 c6');