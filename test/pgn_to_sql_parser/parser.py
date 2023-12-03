import sys
# Parse PGN files to SQL


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

    for game in games:
        if game == "":
            games.remove(game)
    return games

def write_to_sql(games, output_file):
    with open(output_file.replace('pgn','sql'), 'w') as f:

        f.write("CREATE TABLE games (game chessgame);\n")
        f.write("CREATE TABLE favoriteGames (game chessgame);\n")
        f.write("INSERT INTO games VALUES\n")
        for game in games[:-1]:
            f.write("('{} '),\n".format(game))
        f.write("('{}');".format(games[-1]))
        f.write("\nINSERT INTO favoriteGames VALUES ('1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6 5.O-O Be7 6.Re1 b5 7.Bb3 d6 8.c3 O-O 9.h3 Nb8 10.d4 Nbd7');") # Most common opening
         

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 parser.py <file>")
        sys.exit(1)
    
    games = parse_pgns(open_pgn(sys.argv[1]))
    write_to_sql(games, sys.argv[1])

if __name__ == '__main__':
    main()