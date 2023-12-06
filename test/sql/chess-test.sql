\timing on

SET enable_seqscan = off;

CREATE INDEX games_game_idx ON games USING btree(game);

VACUUM ANALYZE games;

EXPLAIN ANALYZE SELECT * FROM games WHERE hasOpening(game, '1.e4 c6 ');