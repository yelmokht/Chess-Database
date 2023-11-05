# 1. Source code of the extension in C

## Data Types (10 points)
- **chessgame:** Represents a chess game using SAN notation.
- **chessboard:** Represents a chess board state using FEN notation.

=> Implementing the two data types (10 points). Favor an internal representation which optimizes the storage space, without jeopardizing the query time.

## Functions (10 points)
1. **getBoard(chessgame, integer) -> chessboard:**
   - Returns the board state at a given half-move.
   - Integer parameter indicates the count of half moves since the beginning.

2. **getFirstMoves(chessgame, integer) -> chessgame:**
   - Returns the chessgame truncated to its first N half-moves.
   - Integer parameter is zero-based.

3. **hasOpening(chessgame, chessgame) -> bool:**
   - Returns true if the first chess game starts with the exact set of moves as the second chess game.

4. **hasBoard(chessgame, chessboard, integer) -> bool:**
   - Returns true if the chessgame contains the given board state in its first N half-moves.

## Indexes (20 points)
1. **Index for hasOpening:**
   - Implemented using a B-tree for total order property.

2. **Index for hasBoard:**
   - Implemented using GIN index to support indexing individual states per chessgame.

=>  Favor implementations that minimize the false positives of the index query. Optimally the predicates will be answered by an index-only scan, i.e., without needing to actually execute the predicate on the tuples that the index returns. Also favor implementations that make the index access transparent to user queries. In other words, it shouldn’t be the case that the user needs to change her query in order to make use of the index.

# 2. SQL file for testing the extension
Along with the query plans and short snippets of the results that you obtained during your tests.

# 3. Presentation
A presentation that explains your work, focusing on the technical details and even samples of the important code. We will publish in UV slots for booking your project evaluation, close to the end of the semester. Then you shall use this presentation for explaining your work.