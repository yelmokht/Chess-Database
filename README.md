# Chess-Database

The goal of this project is to create a PostgreSQL extension for storing and retrieving chess games using Portable Game Notation (PGN), Standard Algebraic Notation (SAN), and Forsyth–Edwards Notation (FEN).

## Notations
- **PGN (Portable Game Notation):** Used to record full games.
- **SAN (Standard Algebraic Notation):** Represents moves in PGN.
- **FEN (Forsyth–Edwards Notation):** Stores board states.

## Installation
Before installation, make sure these packages are nstalled on your device:

```bash
sudo apt-get install postgresql-16
sudo apt-get install postgresql-client-16
sudo apt-get install libpq-dev
sudo apt-get install postgresql-server-dev-16
```

Create a user for yourself: 

```bash
sudo -u postgres createuser -e your_username
sudo -u postgres psql -c "ALTER USER your_username SUPERUSER;"
```

Install the PostgreSQL chess extension using the Makefile command:

```bash
make chess
```
## Usage

Once the PostgreSQL chess extension is installed, submit SQL files to the chess database using the `psql` tool:

```bash
psql chess < your_sql_file.sql
```

Or you can directly interact with the database using the `psql` tool.