\timing on

SET enable_seqscan = off;

--CREATE INDEX games_game_btree_idx ON games USING btree(game);

--CREATE INDEX games_game_gin_idx ON games USING gin(chessgame_to_chessboards(game));

VACUUM ANALYZE games;

--EXPLAIN ANALYZE SELECT * FROM games WHERE hasOpening(game, '1.e4 c6 ');

-- EXPLAIN ANALYZE SELECT * FROM games WHERE hasBoard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 3);

select jsonb_pretty(to_jsonb(games)) from games;