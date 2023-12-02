CREATE TABLE games (game chessgame, list chessboard[]);

CREATE TABLE favoriteGames (game chessgame);

INSERT INTO games VALUES
('1.e4 e6 2.d4 d5 3.Nd2 Nf6 4.e5 Nfd7 5.f4 c5 6.c3 Nc6 7.Ndf3 cxd4 '),
('1.e4 e6 2.d4 d5 3.Nd2 c5 4.exd5 Qxd5 5.Ngf3 cxd4 6.Bc4 Qd6 7.O-O Nf6 '),
('1.e4 c6 2.c4 d5 3.exd5 cxd5 4.cxd5 Nf6 5.Nc3 g6 6.Bc4 Bg7 7.Nf3 O-O '),
('1.e4 c6 2.c4 d5 3.exd5 cxd5 4.cxd5 Nf6 5.Nc3 Nxd5 6.d4 Nc6 7.Nf3 e6 '),
('1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 a6 6.Bg5 e6 7.f4 Be7 ');

UPDATE games SET list = chessgame_to_chessboard_list(game);

--CREATE INDEX game_list_idx ON games USING GIN(list) with (fastupdate = true);

INSERT INTO favoriteGames VALUES
('1.e4 c6 2.c4 d5 3.exd5 cxd5 4.cxd5 Nf6 5.Nc3 g6 6.Bc4 Bg7 7.Nf3 O-O ');