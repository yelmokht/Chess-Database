\timing on

\echo Test 0: SELECT * FROM games;
SELECT * FROM games;

\echo Test 1: Size of data types

\echo Test 1.1: Size of each game - SELECT game, pg_size_pretty(pg_column_size(game)::bigint) AS game_size FROM games;
SELECT game, pg_size_pretty(pg_column_size(game)::bigint) AS game_size FROM games;
\echo Test 1.2: Size of all the games in the column game - SELECT pg_size_pretty(SUM(pg_column_size(game))) AS game_column_size FROM games;
SELECT pg_size_pretty(SUM(pg_column_size(game))) AS game_column_size FROM games;
\echo Test 1.3: Size of the games table - SELECT pg_size_pretty(pg_total_relation_size('games')) AS games_size;
SELECT pg_size_pretty(pg_total_relation_size('games')) AS games_size;
\echo Test 1.4: Size of a single block in PostgreSQL - SELECT current_setting('block_size');
SELECT current_setting('block_size');

\echo Test 2: getBoard

\echo Test 2.1: SELECT getBoard(game, 0) FROM games;
SELECT getBoard(game, 0) FROM games;
\echo Test 2.2: SELECT getBoard(game, 1) FROM games;
SELECT getBoard(game, 1) FROM games;
\echo Test 2.3: SELECT getBoard(game, 7) FROM games;
SELECT getBoard(game, 7) FROM games;
\echo Test 2.4: SELECT getBoard(game, 14) FROM games;
SELECT getBoard(game, 14) FROM games;
\echo Test 2.5: SELECT getBoard(game, 15) FROM games;
SELECT getBoard(game, 15) FROM games;
\echo Test 2.6: SELECT getBoard(game, 100) FROM games;
SELECT getBoard(game, 100) FROM games;

\echo Test 3: getFirstMoves

\echo Test 3.1: SELECT getFirstMoves(game, 1) FROM games;
SELECT getFirstMoves(game, 1) FROM games;
\echo Test 3.2: SELECT getFirstMoves(game, 7) FROM games;
SELECT getFirstMoves(game, 7) FROM games;
\echo Test 3.3: SELECT getFirstMoves(game, 14) FROM games;
SELECT getFirstMoves(game, 14) FROM games;
\echo Test 3.4: SELECT getFirstMoves(game, 15) FROM games;
SELECT getFirstMoves(game, 15) FROM games;
\echo Test 3.5: SELECT getFirstMoves(game, 100) FROM games;
SELECT getFirstMoves(game, 100) FROM games;

\echo Test 4: hasOpening

\echo Test 4.1: SELECT hasOpening(game, '1.e4 e6 2.d4 d5 3.Nd2 Nf6 4.e5 Nfd7 5.f4 c5 6.c3 Nc6 7.Ndf3 cxd4 ') FROM games;
SELECT hasOpening(game, '1.e4 e6 2.d4 d5 3.Nd2 Nf6 4.e5 Nfd7 5.f4 c5 6.c3 Nc6 7.Ndf3 cxd4 ') FROM games;
\echo Test 4.2: SELECT hasOpening(game, '1.e4 c6 2.c4 d5 3.exd5 cxd5 4.cxd5 Nf6 5.Nc3 g6 6.Bc4 Bg7 7.Nf3 O-O ') FROM games;
SELECT hasOpening(game, '1.e4 c6 2.c4 d5 3.exd5 cxd5 4.cxd5 Nf6 5.Nc3 g6 6.Bc4 Bg7 7.Nf3 O-O ') FROM games;
\echo Test 4.3: SELECT hasOpening(game, '1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 a6 6.Bg5 e6 7.f4 Be7 ') FROM games;
SELECT hasOpening(game, '1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 a6 6.Bg5 e6 7.f4 Be7 ') FROM games;
\echo Test 4.4: SELECT hasOpening(game, '1.e1 c1') FROM games;
SELECT hasOpening(game, '1.e1 c1') FROM games;
\echo Test 4.5: SELECT hasOpening(game, '1.e4 ') FROM games;
SELECT hasOpening(game, '1.e4 ') FROM games;

\echo Test 5: hasboard

\echo Test 5.1: SELECT hasboard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 0) FROM games;
SELECT hasboard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 0) FROM games;
\echo Test 5.2: SELECT hasboard(game, 'rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1', 1) FROM games;
SELECT hasboard(game, 'rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1', 1) FROM games;
\echo Test 5.3: SELECT hasboard(game, 'rnbqkbnr/pp2pppp/8/3P4/8/8/PP1P1PPP/RNBQKBNR b KQkq - 0 4', 7) FROM games;
SELECT hasboard(game, 'rnbqkbnr/pp2pppp/8/3P4/8/8/PP1P1PPP/RNBQKBNR b KQkq - 0 4', 7) FROM games;
\echo Test 5.4: SELECT hasboard(game, 'rnbqk2r/1p2bppp/p2ppn2/6B1/3NPP2/2N5/PPP3PP/R2QKB1R w KQkq - 1 8', 14) FROM games;
SELECT hasboard(game, 'rnbqk2r/1p2bppp/p2ppn2/6B1/3NPP2/2N5/PPP3PP/R2QKB1R w KQkq - 1 8', 14) FROM games;
\echo Test 5.5: SELECT hasboard(game, 'rnbqk2r/1p2bppp/p2ppn2/6B1/3NPP2/2N5/PPP3PP/R2QKB1R w KQkq - 1 8', 15) FROM games;
SELECT hasboard(game, 'rnbqk2r/1p2bppp/p2ppn2/6B1/3NPP2/2N5/PPP3PP/R2QKB1R w KQkq - 1 8', 15) FROM games;

\echo Test 6: hasOpening

\echo Test 6.1: without btree index
EXPLAIN ANALYZE SELECT count(*) FROM games WHERE hasOpening(game, '1.e4 c6 2.c4 d5 3.exd5 cxd5 ');
\echo Test 6.2: with btree index
SET enable_seqscan = off;
CREATE INDEX games_game_btree_idx ON games USING btree(game);
VACUUM ANALYZE games;
EXPLAIN ANALYZE SELECT count(*) FROM games WHERE hasOpening(game, '1.e4 c6 2.c4 d5 3.exd5 cxd5 ');
\echo Test 6.3: with btree index and hasOpening2() function
EXPLAIN ANALYZE SELECT count(*) FROM games WHERE hasOpening2(game, '1.e4 c6 2.c4 d5 3.exd5 cxd5 ');
DROP INDEX games_game_btree_idx;
SET enable_seqscan = on;

\echo Test 7: hasboard

\echo Test 7.1: without gin index
EXPLAIN ANALYZE SELECT count(*) FROM games WHERE hasboard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 10);
\echo Test 7.2: with gin index
SET enable_seqscan = off;
CREATE INDEX games_game_gin_idx ON games USING gin(chessgame_to_chessboards(game)) WITH (fastupdate = on);
VACUUM ANALYZE games;
\echo with hasBoard() function
EXPLAIN ANALYZE SELECT count(*) FROM games WHERE hasboard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 10);

\echo with hasBoard2() function
EXPLAIN ANALYZE SELECT count(*) FROM games WHERE hasboard2(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 10); -- don't use the last parameter
SET enable_seqscan = on;

-- \echo Test 8: Display chessgame and chessboards
-- CREATE TABLE g AS (SELECT * FROM games, chessgame_to_chessboards(game) AS chessboards);
-- select jsonb_pretty(to_jsonb(g)) from g;