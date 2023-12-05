import os
import sys

# Parse PGN files to SQL

SQL_FOLDER = 'sql'

def open_pgn(file):
    with open(file, 'r') as f:
        data = f.readlines()
    return data


def parse_pgns(data):
    games = []
    game = ""
    for line in data:
        if line[0] == '[':
            pass
        elif line == '\n' and game != "":
            games.append(game)
            game = ""
        else:
            game += line.strip() + " "

    games = [game for game in games if game]  # Remove empty games
    return games

def write_to_sql(games, output_file):
    with open(os.path.join(SQL_FOLDER, output_file.replace('pgn', 'sql')), 'w') as f:
        f.write("CREATE TABLE games (game chessgame, states chessboard[]);\n")
        f.write("CREATE TABLE favoriteGames (game chessgame);\n")
        f.write("UPDATE games SET states = chessgame_to_chessboards(game);\n")
        f.write("INSERT INTO games VALUES\n")
        non_empty_games = [game.lstrip() for game in games if game.strip()]  # Remove empty and leading space from games
        for game in non_empty_games[:-1]:
            f.write("('{}'),\n".format(game))
        if non_empty_games:
            f.write("('{}');".format(non_empty_games[-1]))
        f.write("\nINSERT INTO favoriteGames VALUES ('1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6 5.O-O Be7 6.Re1 b5 7.Bb3 d6 8.c3 O-O 9.h3 Nb8 10.d4 Nbd7');")  # Most common opening

def process_folder(folder):
    games = []
    for file_name in os.listdir(folder):
        if file_name.endswith(".pgn"):
            file_path = os.path.join(folder, file_name)
            games.extend(parse_pgns(open_pgn(file_path)))
    return games

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 parser.py <file or folder>")
        sys.exit(1)

    input_path = sys.argv[1]
    if os.path.isdir(input_path):
        games = process_folder(input_path)
    elif os.path.isfile(input_path):
        games = parse_pgns(open_pgn(input_path))
    else:
        print("Invalid input. Please provide a valid file or folder.")
        sys.exit(1)

    # Create 'sql' folder if it doesn't exist
    if not os.path.exists(SQL_FOLDER):
        os.makedirs(SQL_FOLDER)

    write_to_sql(games, 'large.sql')


if __name__ == '__main__':
    main()
