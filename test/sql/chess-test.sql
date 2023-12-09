\timing on

SET enable_seqscan = off;

--CREATE INDEX games_game_idx ON games USING btree(game);


CREATE INDEX games_game_idx ON games USING gin(states);

VACUUM ANALYZE games;

--EXPLAIN ANALYZE SELECT * FROM games WHERE hasOpening(game, '1.e4 c6 ');

EXPLAIN ANALYZE SELECT * FROM games WHERE hasBoard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 10);