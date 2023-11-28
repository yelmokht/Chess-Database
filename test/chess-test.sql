CREATE TABLE games (game chessgame);

CREATE TABLE favoriteGames (game chessgame);

INSERT INTO games VALUES
('1.e4 e6 2.d4 d5 3.Nd2 Nf6 4.e5 Nfd7 5.f4 c5 6.c3 Nc6 7.Ndf3 cxd4 '),
('1.e4 e6 2.d4 d5 3.Nd2 c5 4.exd5 Qxd5 5.Ngf3 cxd4 6.Bc4 Qd6 7.O-O Nf6 '),
('1.e4 c6 2.c4 d5 3.exd5 cxd5 4.cxd5 Nf6 5.Nc3 g6 6.Bc4 Bg7 7.Nf3 O-O '),
('1.e4 c6 2.c4 d5 3.exd5 cxd5 4.cxd5 Nf6 5.Nc3 Nxd5 6.d4 Nc6 7.Nf3 e6 '),
('1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 a6 6.Bg5 e6 7.f4 Be7 ');

\echo
\echo Games inserted succesfully.
\echo
\echo Test 1: SELECT * FROM games;
\echo
SELECT * FROM games;
\echo
\echo Test 2: getBoard
\echo
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
\echo
\echo Test 3: getFirstMoves
\echo
\echo Test 3.1: SELECT getFirstMoves(game, 0) FROM games;
SELECT getFirstMoves(game, 0) FROM games;
\echo Test 3.2: SELECT getFirstMoves(game, 1) FROM games;
SELECT getFirstMoves(game, 1) FROM games;
\echo Test 3.3: SELECT getFirstMoves(game, 7) FROM games;
SELECT getFirstMoves(game, 7) FROM games;
\echo Test 3.4: SELECT getFirstMoves(game, 14) FROM games;
SELECT getFirstMoves(game, 14) FROM games;
\echo Test 3.5: SELECT getFirstMoves(game, 15) FROM games;
SELECT getFirstMoves(game, 15) FROM games;
\echo Test 3.6: SELECT getFirstMoves(game, 100) FROM games;
SELECT getFirstMoves(game, 100) FROM games;
\echo
\echo Test 4: hasOpening
\echo
\echo Test 4.1: SELECT hasOpening(game, '1.e4 e6 2.d4 d5 3.Nd2 Nf6 4.e5 Nfd7 5.f4 c5 6.c3 Nc6 7.Ndf3 cxd4 ') FROM games;
SELECT hasOpening(game, '1.e4 e6 2.d4 d5 3.Nd2 Nf6 4.e5 Nfd7 5.f4 c5 6.c3 Nc6 7.Ndf3 cxd4 ') FROM games; -- Only 1 true
\echo Test 4.2: SELECT hasOpening(game, '1.e4 c6 2.c4 d5 3.exd5 cxd5 4.cxd5 Nf6 5.Nc3 g6 6.Bc4 Bg7 7.Nf3 O-O ') FROM games;
SELECT hasOpening(game, '1.e4 c6 2.c4 d5 3.exd5 cxd5 4.cxd5 Nf6 5.Nc3 g6 6.Bc4 Bg7 7.Nf3 O-O ') FROM games; -- Only 3 true
\echo Test 4.3: SELECT hasOpening(game, '1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 a6 6.Bg5 e6 7.f4 Be7 ') FROM games;
SELECT hasOpening(game, '1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 a6 6.Bg5 e6 7.f4 Be7 ') FROM games; -- Only 5 true
\echo Test 4.4: SELECT hasOpening(game, '1.e1 c1') FROM games;
SELECT hasOpening(game, '1.e1 c1') FROM games; -- All false
\echo Test 4.5: SELECT hasOpening(game, '') FROM games;
SELECT hasOpening(game, '') FROM games; -- All true
\echo
\echo Test 5: hasBoard
\echo
\echo Test 5.1: SELECT hasBoard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 0) FROM games;
SELECT hasBoard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 0) FROM games; -- All true
\echo Test 5.2: SELECT hasBoard(game, 'rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1', 1) FROM games;
SELECT hasBoard(game, 'rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1', 1) FROM games; -- All true
\echo Test 5.3: SELECT hasBoard(game, 'rnbqkbnr/pp2pppp/8/3P4/8/8/PP1P1PPP/RNBQKBNR b KQkq - 0 4', 7) FROM games;
SELECT hasBoard(game, 'rnbqkbnr/pp2pppp/8/3P4/8/8/PP1P1PPP/RNBQKBNR b KQkq - 0 4', 7) FROM games; -- 3 and 4 true
\echo Test 5.4: SELECT hasBoard(game, 'rnbqk2r/1p2bppp/p2ppn2/6B1/3NPP2/2N5/PPP3PP/R2QKB1R w KQkq - 1 8', 14) FROM games;
SELECT hasBoard(game, 'rnbqk2r/1p2bppp/p2ppn2/6B1/3NPP2/2N5/PPP3PP/R2QKB1R w KQkq - 1 8', 14) FROM games; -- Only 5 true
\echo Test 5.5: SELECT hasBoard(game, 'rnbqk2r/1p2bppp/p2ppn2/6B1/3NPP2/2N5/PPP3PP/R2QKB1R w KQkq - 1 8', 15) FROM games;
SELECT hasBoard(game, 'rnbqk2r/1p2bppp/p2ppn2/6B1/3NPP2/2N5/PPP3PP/R2QKB1R w KQkq - 1 8', 15) FROM games; -- Only 5 true
\echo Test 5.6: SELECT hasBoard(game, '', 100) FROM games;
SELECT hasBoard(game, '', 100) FROM games; -- All false
\echo
\echo Test 6: SELECT count(*) FROM games WHERE hasboard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 10);
SELECT count(*) FROM games WHERE hasboard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 10);
\echo
\echo Test 7: SELECT count(*) FROM games WHERE hasopening(game, '1. e4 e5 2. Nf3 Nf6 3. d3');
SELECT count(*) FROM games WHERE hasopening(game, '1. e4 e5 2. Nf3 Nf6 3. d3');
\echo
\echo Test 8: SELECT g.game FROM games g, favoriteGames f WHERE hasopening(g.game, getFirstMoves(f.game, 10));