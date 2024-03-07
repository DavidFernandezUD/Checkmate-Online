# CHECKMATE ONLINE

Checkmate Online™ is a server based multiplayer chess game written in good old C and C++.
Checkmake Online™ allows for persistence between games thanks to it's server based
architecture, but also has singleplayer functionality thanks to it's "state of the art"
minimax based Chese-e™ engine.

## IMPLEMENTATION DETAILS

Checkmate Online™ is based an a piece-centric bitboard repressentation of the chessboard 
where each type of peace is represented with a 64 bit number. This allows for efficient bit-wise
operations in the backend. This speed is necessary for the chess engine to get to suficient
depth in the move tree. After reaching the specified depth, the algorythm will use an evaluation
function to determine a score for every possible positions at that depth and will return the best
move assuming optimum play by booth players.

On the client-server side we have implemented the UCI "Univarsal Chesss Interface" open
comunication protocol since it is the standard in chess related software, and allows
for interaction with other aplications if necesry.

## USER INTERFACE

The user can interact with Checkmate Online via it's intuitive and sleak console based user
interface by passing commands through a console prompt.

## USER MANUAL

1. Login:
    1. Enter user credentials.

2. Register:
    1. Enter unique user name.
    2. Enter valid password.

3. Navigate through the menus:
    1. Press [1] to start new Player vs Player game.
    2. Press [2] to start Player vs AI game.
    4. Press [3] to enter the stats page.
    5. Press [4] to log out of the game and exit the aplication.

4. Player vs Player game:
    1. Wait until an aponent is selected for your game.
    2. Enter moves in algebraic notation as explained by the leyend.
    3. Type [exit] and confirm with [yes] to exit a game. (The oponent will win inmediately)
    3. After ending the game either with a WIN, LOSS or a DRAW, you will return to the main menu.

5. Player vs AI game:
    1. Select the depth level of the algorithm from the given options.
    2. Type [exit] and confirm with [yes] to exit a game.
    3. After ending the game you will return to the main menu.

6. Stats:
    1. You will see an overview of your chess stats in the stats menu.
    2. In the top part of the screen you will see your played game count, wins, losses and ELO score.
    3. On the bottom part the results of your last 5 games will be displayed.

7. Log Out:
    1. While on the main menu type [exit] and confirm with a [yes] to log out and exit the application.

## INTERESTING RESOURCES

- [Bitboards](https://www.chessprogramming.org/Bitboards)
- [Magic Bitboards](https://www.chessprogramming.org/Magic_Bitboards)
- [Move Generation](https://www.chessprogramming.org/Move_Generation#Staged_move_generation)
- [Negmax](https://www.chessprogramming.org/Negamax)
- [Principal Variation Search](https://www.chessprogramming.org/Principal_Variation_Search)
- [NNUE](https://www.chessprogramming.org/NNUE)

