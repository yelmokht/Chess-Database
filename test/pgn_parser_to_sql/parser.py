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

        f.write("INSERT INTO games VALUES\n")
        for game in games[:-1]:
            f.write("('{} '),\n".format(game))
        f.write("('{}');".format(games[-1]))
         

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 parser.py <file>")
        sys.exit(1)
    
    games = parse_pgns(open_pgn(sys.argv[1]))
    write_to_sql(games, sys.argv[1])

if __name__ == '__main__':
    main()