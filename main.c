#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//enumerations
enum chessPieces {noPiece, whitePawn, blackPawn, whiteKnight, blackKnight, whiteBishop, blackBishop, whiteRook, blackRook, whiteQueen, blackQueen, whiteKing, blackKing};
enum whoseMoveEnum  {whitesMove, blacksMove};
enum files {a, b, c, d, e, f, g, h};//vertical rows on a chess board
enum ranks {eight, seven, six, five, four, three, two, one};//horizontal rows on a chess board

//global variables/arrays
int moveCount;
int whoseMove;
char moveHolderInCharForm[4];
int moveHolderInSemiAlgebraicForm[4];
int chessBoardMatrix[8][8];

//this array stores all the necessary info to deduce if a en passant move is legal
int lastPawnFileThatMoved2SquaresAndItsMoveCount[2];//index 0 stores the last pawn-file that moved up two squares. Index 1 stores what the move-count was when it occurred

//legality functions (original microcontroller functions)
bool isTheMoveLegal(int fromFile, int fromRank, int toFile, int toRank);
bool isSquareToSquareMoveLegal(int fromFile, int fromRank, int toFile, int toRank);
bool isTheMovingFromSquareLegal(int fromFile, int fromRank);
bool isDestinationSquareUnoccupiedOrOpponentOccupied(int toFile, int toRank);
bool isDestinationSquareUnoccupied(int toFile, int toRank);
bool isDestinationSquareOpponentOccupied(int toFile, int toRank);
bool isThePieceMoveLegal(int fromFile, int fromRank, int toFile, int toRank);
bool isRookMoveLegal(int fromFile, int fromRank, int toFile, int toRank);
bool isBishopMoveLegal(int fromFile, int fromRank, int toFile, int toRank);
bool isknightMoveLegal(int fromFile, int fromRank, int toFile, int toRank);
bool isQueenMoveLegal(int fromFile, int fromRank, int toFile, int toRank);
bool isKingMoveLegal(int fromFile, int fromRank, int toFile, int toRank);
bool isPawnMoveLegal(int fromFile, int fromRank, int toFile, int toRank);

//prototype declarations for miscellaneous functions
void simulateGamePlay();
int getPiece(int fileOfSquare, int rankOfSquare);
void setPieceOnSquare(int piece, int fileOfSquare, int rankOfSquare);
void setPiecesToInitialPosition();
void clearEntireBoard();
void showchessBoardMatrix();
void showchessBoard();
void printFile(int file);
void printMove(int fromFile, int fromRank, int toFile, int toRank);
void printWhatMoveTheComputerPlayed();
void printWelcomingMessage();
int randomNumber(int min, int max);
void charToIntConverter();
void randomlyLookForLegalMove();

//functions/variables for manual testing
int initialFile, initialRank, finalFile, finalRank;
void manuallyTestingFuction();
void moveThePieceFromSquare(int file, int rank);
void moveThePieceToSquare(int file, int rank);
void checkIfTheMoveIsLegal();

int main(){

    //randomly look for bugs by simulating chess play
    simulateGamePlay();

    //when a new functionally is added, set up a testing position in the next function to test it out directly
    //manuallyTestingFuction();

    return 0;
}

//check if the move the move is legal
bool isTheMoveLegal(int fromFile, int fromRank, int toFile, int toRank){

    //preliminary check, followed by a thorough check for legality
    if(isSquareToSquareMoveLegal(fromFile, fromRank, toFile, toRank) && isThePieceMoveLegal(fromFile, fromRank, toFile, toRank))
        return true;
    else
        return false;
}

/*
    this function does a preliminary check for legality by simply checking if the square that you're trying to move from
    actually has one of the player's pieces on it, and that the destination square is unoccupied or opponent occupied.
*/
bool isSquareToSquareMoveLegal(int fromFile, int fromRank, int toFile, int toRank){

    if(isTheMovingFromSquareLegal(fromFile, fromRank) && isDestinationSquareUnoccupiedOrOpponentOccupied(toFile, toRank))
            return true;
    else
        return false;

}

//this function checks if the square that you're trying to move FROM is legal
bool isTheMovingFromSquareLegal(int fromFile, int fromRank){

    if(chessBoardMatrix[fromRank][fromFile] == noPiece)//if the square has no piece on it, return a false
        return false;
    else if(whoseMove == whitesMove){//if it's whites move and the square has a white piece, then return true. Else return false

        if(chessBoardMatrix[fromRank][fromFile] % 2 == 1)
            return true;
        else
            return false;
    }
    else{//if it's blacks move and the square has a black piece, then return true. Else return false

        if(chessBoardMatrix[fromRank][fromFile] % 2 == 0)
            return true;
        else
            return false;
    }
}

//returns true if the go-to square is unoccupied or if the square is occupied with an opposite color piece
bool isDestinationSquareUnoccupiedOrOpponentOccupied(int toFile, int toRank){

    if(chessBoardMatrix[toRank][toFile] == noPiece)//if the destination square is unoccupied
        return true;

    else if(whoseMove == whitesMove){

        if(chessBoardMatrix[toRank][toFile] % 2 == 0)//if the destination square is occupied with a black piece
            return true;
        else
            return false;
    }
    else{//hence whoseMove = blacksMove

        if(chessBoardMatrix[toRank][toFile] % 2 == 1)//if the destination square is occupied with a white piece
            return true;
        else
            return false;
    }
}

//to write a comment here would be redundant, and thus a waist of time (due to this function's self-descriptive name).....crap, too late! :)
bool isDestinationSquareUnoccupied(int toFile, int toRank){

    if(chessBoardMatrix[toRank][toFile] == noPiece)
        return true;
    else
        return false;

}

//returns true if the go-to square is occupied with an opposite color piece
bool isDestinationSquareOpponentOccupied(int toFile, int toRank){

    if(chessBoardMatrix[toRank][toFile] == noPiece)//if the destination square is unoccupied
        return false;
    else if(whoseMove == whitesMove){

        if(chessBoardMatrix[toRank][toFile] % 2 == 0)//if the destination square is occupied with a black piece
            return true;
        else
            return false;
    }
    else{

        if(chessBoardMatrix[toRank][toFile] % 2 == 1)//if the destination square is occupied with a white piece
            return true;
        else
            return false;
    }
}

//this function gets the piece on the moves square, then return true only if the move is legal
bool isThePieceMoveLegal(int fromFile, int fromRank, int toFile, int toRank){

    int piece = getPiece(fromFile, fromRank);//get the moving piece

    bool isItLegal = false;

    if(piece % 2 == 0)//if the moving piece is a black piece...
        piece--;//...make it a white piece (shortens the following switch statement in half)

    switch(piece){
        case whiteRook:
            isItLegal = isRookMoveLegal(fromFile, fromRank, toFile, toRank);
            break;
        case whiteBishop:
            isItLegal = isBishopMoveLegal(fromFile, fromRank, toFile, toRank);
            break;
        case whiteKnight:
            isItLegal = isknightMoveLegal(fromFile, fromRank, toFile, toRank);
            break;
        case whiteQueen:
            isItLegal = isQueenMoveLegal(fromFile, fromRank, toFile, toRank);
            break;
        case whiteKing:
            isItLegal = isKingMoveLegal(fromFile, fromRank, toFile, toRank);
            break;
        case whitePawn:
            isItLegal = isPawnMoveLegal(fromFile, fromRank, toFile, toRank);
    }

    return isItLegal;
}

//returns true if the indicated square-to-square move is legal for a rook
bool isRookMoveLegal(int fromFile, int fromRank, int toFile, int toRank){

    int i;

    if(fromRank == toRank){// if the rook moves within the same rank (i.e. if it moves horizontally)

        int smallerFileNumber, largerFileNumber;

        //this if-else finds which file  is numerically larger (sets up the following for-loop)
        if(fromFile < toFile){
            smallerFileNumber = fromFile;
            largerFileNumber = toFile;
        }
        else{
            smallerFileNumber = toFile;
            largerFileNumber = fromFile;
        }

        //look for a piece for every square in between the initial and final squares. Finding one would imply that the rook hopped over a piece, and is why it returns false.
        for(i = (smallerFileNumber + 1); i < largerFileNumber; i++){

            if(chessBoardMatrix[fromRank][i] != noPiece)
                return false;
        }

        return true;//this statement will only be reached if the rook moved horizontally and there was no piece in between its path. Hence it would be a legal move and thus returns true
    }
    else if(fromFile == toFile){//if the rook moves within the same file (i.e. if it moves vertically)

        int smallerRankNumber, largerRankNumber;

        //this if-else finds which rank is numerically larger
        if(fromRank < toRank){
            smallerRankNumber = fromRank;
            largerRankNumber = toRank;
        }
        else{
            smallerRankNumber = toRank;
            largerRankNumber = fromRank;
        }

        //look for a piece for every square in between the initial and final squares. Finding one would imply that the rook hopped over a piece, and is why it returns false.
        for(i = (smallerRankNumber + 1); i < largerRankNumber; i++){
            if(chessBoardMatrix[i][fromFile] != noPiece)
                return false;
        }

        return true;//this statement will only be reached if the rook moved vertically and there was no piece in between its path. Hence it would be a legal move and thus returns true
    }
    else//hence the rook moved illegally since it did not move strait up or down, or strait side-to-side and thus returns false
        return false;
}

bool isBishopMoveLegal(int fromFile, int fromRank, int toFile, int toRank){

    /*
        most of the logic in this function is build upon the fact that a legal bishop move depends on whether or not its point-to-point movement constitutes a mathematical slope of +-1
        on a Cartesian plane. i.e. a bishop move can only be legal when its two points form a +-1 slope. However, note that the converse of this is not true (e.g. if there is piece in
        between the initial and final positions, the move is illegal regardless the slope value).
    */

    int smallestFile = fromFile;
    int largestFile = toFile;

    //find which file is numerically larger
    if(toFile < fromFile){
        smallestFile = toFile;
        largestFile = fromFile;
    }

    int smallestRank = fromRank;
    int largestRank = toRank;

    //find which rank is numerically larger
    if(toRank < fromRank){
        smallestRank = toRank;
        largestRank = fromRank;
    }

    if(largestFile - smallestFile == largestRank - smallestRank){//if the bishop moved diagonally

        int rankIncrement = 1;
        int rankOfSmallestFile = smallestRank;

        //if the mathematical point-to-point slope is negative (the two above declarations assume it's positive), then invert the above declarations
        if((toRank - fromRank)/(toFile - fromFile) == -1){
            rankIncrement = -1;
            rankOfSmallestFile = largestRank;
        }

        int i;

        //look for a piece for every square in between the initial and final squares. Finding one would imply that the bishop hopped over a piece, and hence is why it returns false.
        for(i = (smallestFile + 1); i < largestFile; i++){
            if(chessBoardMatrix[rankOfSmallestFile + rankIncrement][i] != noPiece)
                return false;

            rankOfSmallestFile += rankIncrement;//increment by +1 or -1 (depends on whether or not the point-to-point slope is positive or negative, already established above)
        }

        return true;//this statement will only be reached if the bishop moved diagonally and there was no piece in between its path. Hence it would be a legal move and thus returns true
    }
    else//i.e. the bishop did not move diagonally
        return false;
}

bool isknightMoveLegal(int fromFile, int fromRank, int toFile, int toRank){

    int deltaFile = toFile - fromFile;
    int deltaRank = toRank - fromRank;

    if(deltaFile == 1){// if the knight moved horizontally by one AND...

        if(deltaRank == 2 || deltaRank == -2)//...if the knight moved vertically by two, then...
            return true;//...return true because it moved in an "L" shape (the rest of this else-if latter follows similar logic)
    }
    else if(deltaFile == -1){

        if(deltaRank == 2 || deltaRank == -2)
            return true;
    }
    else if(deltaFile == 2){

        if(deltaRank == 1 || deltaRank == -1)
            return true;
    }
    else if(deltaFile == -2){

        if(deltaRank == 1 || deltaRank == -1)
            return true;
    }

    return false;//this code segment will be reached only if the knight did NOT move in an "L" shape. Hence return false
}

bool isQueenMoveLegal(int fromFile, int fromRank, int toFile, int toRank){

    //if the queen moved like a bishop or a rook, then return true. Else return false. No need to develop new code :)
    if(isBishopMoveLegal(fromFile, fromRank, toFile, toRank) || isRookMoveLegal(fromFile, fromRank, toFile, toRank))
        return true;
    else
        return false;
}

bool isKingMoveLegal(int fromFile, int fromRank, int toFile, int toRank){

    int deltaFile = toFile - fromFile;
    int deltaRank = toRank - fromRank;

    //find the absolute value of the above declarations
    if(deltaFile < 0)
        deltaFile = -deltaFile;

    if(deltaRank < 0)
        deltaRank = -deltaRank;

    if(deltaFile < 2 && deltaRank < 2){//if the king moved only one square AND...

        if(isQueenMoveLegal(fromFile, fromRank, toFile, toRank))//...if the king moved look like a queen, then...
            return true;//...the king moved like a girl, hence return true :)
    }

    return false;//this part will only be reached if the king did not move legally. Hence return false
}

bool isPawnMoveLegal(int fromFile, int fromRank, int toFile, int toRank){

    if(getPiece(fromFile, fromRank) == whitePawn){//if the moving piece is a white pawn...

        if(fromRank != two && toRank - fromRank == -1){//if the moving white pawn is NOT in its initial position AND it moved FORWFARD one square...

            if(toFile == fromFile && isDestinationSquareUnoccupied(toFile, toRank))//if moving white pawn stayed in the same file AND moved into an unoccupied square...
                return true;
            else if(isDestinationSquareOpponentOccupied(toFile, toRank) && (toFile - fromFile == 1 || toFile - fromFile == -1))//else if the moving pawn captured (non en-passant)...
                return true;
            else if (fromRank == five && (toFile - fromFile == 1 || toFile - fromFile == -1) && lastPawnFileThatMoved2SquaresAndItsMoveCount[0] == toFile &&
                lastPawnFileThatMoved2SquaresAndItsMoveCount[1] == (moveCount - 1)){//else if white pawn captures via en-passant

                setPieceOnSquare(noPiece, toFile, five);
                return true;
            }
        }
        else if(fromRank == two){//else if white pawn moves from its initial position...

            if(toRank - fromRank == -1){//if white pawn moves FORWFARD one square...

                if(toFile == fromFile && isDestinationSquareUnoccupied(toFile, toRank))//if moving white pawn stayed in the same file AND moved into an unoccupied square...
                    return true;
                else if(isDestinationSquareOpponentOccupied(toFile, toRank) && (toFile - fromFile == 1 || toFile - fromFile == -1))//else if the moving pawn captured (non en-passant)...
                    return true;
            }
            //else if  white pawn moved strait forward 2 squares legally: update following array to prepare for possible en passant on the next opponent move
            else if(toRank - fromRank == -2 && fromFile == toFile && isDestinationSquareUnoccupied(toFile, toRank) && isDestinationSquareUnoccupied(fromFile, three)){
                    lastPawnFileThatMoved2SquaresAndItsMoveCount[0] = fromFile;
                    lastPawnFileThatMoved2SquaresAndItsMoveCount[1] = moveCount;
                    return true;
            }
        }
    }
    else{//i.e. back pawn logic starts here (basically the same logic as above)

        if(fromRank != seven && toRank - fromRank == 1){

            if(toFile == fromFile && isDestinationSquareUnoccupied(toFile, toRank))
                return true;
            else if(isDestinationSquareOpponentOccupied(toFile, toRank) && (toFile - fromFile == 1 || toFile - fromFile == -1))
                return true;
            else if (fromRank == four && (toFile - fromFile == 1 || toFile - fromFile == -1) && lastPawnFileThatMoved2SquaresAndItsMoveCount[0] == toFile &&
                lastPawnFileThatMoved2SquaresAndItsMoveCount[1] == (moveCount - 1)){//else if black pawn captures via en-passant

                setPieceOnSquare(noPiece, toFile, four);
                return true;
            }
        }
        else if(fromRank == seven){

            if(toRank - fromRank == 1){

                if(toFile == fromFile && isDestinationSquareUnoccupied(toFile, toRank))
                    return true;
                else if(isDestinationSquareOpponentOccupied(toFile, toRank) && (toFile - fromFile == 1 || toFile - fromFile == -1))
                    return true;
            }
            else if(toRank - fromRank == 2 && fromFile == toFile && isDestinationSquareUnoccupied(toFile, toRank) && isDestinationSquareUnoccupied(fromFile, six)){
                    lastPawnFileThatMoved2SquaresAndItsMoveCount[0] = fromFile;
                    lastPawnFileThatMoved2SquaresAndItsMoveCount[1] = moveCount;
                    return true;
            }
        }
    }

    return false;//this part of the code will only be reached if the pawn move was illegal. Hence return false
}

void simulateGamePlay(){

    srand(time(NULL));//initiate random-generating capabilities

    int pieceOnMovingSquare;//this will hold the piece that is being moved
    int playersColorChoice;//this will hold what color the player chooses at run time
    char playersChosenColorChar;//holds a character that determines what color to assign to the user at run time

    setPiecesToInitialPosition();//sets the chess board to its stating playing position

    //ask the user a if he or she wants to play as white or black
    printWelcomingMessage();
    scanf("%c", &playersChosenColorChar);//get the the input user's choice

    //transfer the above info into the playersColorChoice variable
    if(playersChosenColorChar == 'w')
        playersColorChoice = whitesMove;
    else
        playersColorChoice = blacksMove;

    printf("\n");

    showchessBoard();//shows chess board starting position

    while(true){//temporary until a function that determines check-mate is written

        whoseMove = moveCount % 2;//change whose turn it is

        if(whoseMove == playersColorChoice){//if it is the user's turn

            while(true){
                printf("Enter your move: ");//tell the user to enter a move
                scanf("%s", moveHolderInCharForm);//get the user's move
                printf("\n");
                charToIntConverter();//transfer the move into the moveHolderInSemiAlgebraicForm array, which stores (i.e holds) the move

                //if the move is legal, break out of the while loop. Else, print a message to the user signifying that the move was illegal
                if(isTheMoveLegal(moveHolderInSemiAlgebraicForm[0], moveHolderInSemiAlgebraicForm[1], moveHolderInSemiAlgebraicForm[2], moveHolderInSemiAlgebraicForm[3]))
                    break;
                else
                    printf("Illegal Move! Please try again. \n");
            }
        }
        else{//i.e. else it is computer's turn
            printf("Computer is thinking... ");//show this message while the computer "thinks"
            randomlyLookForLegalMove();//computer randomly looks for the first legal move that it finds, then stores it in moveHolderInSemiAlgebraicForm array
            printWhatMoveTheComputerPlayed();
        }

        pieceOnMovingSquare = getPiece(moveHolderInSemiAlgebraicForm[0], moveHolderInSemiAlgebraicForm[1]);//get piece that moved, then...
        setPieceOnSquare(pieceOnMovingSquare, moveHolderInSemiAlgebraicForm[2], moveHolderInSemiAlgebraicForm[3]);//...place it on the go-to square (i.e. destination square)
        setPieceOnSquare(noPiece, moveHolderInSemiAlgebraicForm[0], moveHolderInSemiAlgebraicForm[1]);//empty the square the moving piece moved from
        showchessBoard();//show the new board set-up
        moveCount++;//update the move count
    }
}

//this function returns the enum value (i.e. integer value) of the piece that's on the specified square
int getPiece(int fileOfSquare, int rankOfSquare){

    return chessBoardMatrix[rankOfSquare][fileOfSquare];
}

//this function sets the piece of your choice on the specified square
void setPieceOnSquare(int piece, int fileOfSquare, int rankOfSquare){

    chessBoardMatrix[rankOfSquare][fileOfSquare] = piece;
}

//this sets the game board to its starting position
void setPiecesToInitialPosition(){

    int i, j;

    //set black non-pawn pieces
    chessBoardMatrix[0][0] = blackRook;
    chessBoardMatrix[0][1] = blackKnight;
    chessBoardMatrix[0][2] = blackBishop;
    chessBoardMatrix[0][3] = blackQueen;
    chessBoardMatrix[0][4] = blackKing;
    chessBoardMatrix[0][5] = blackBishop;
    chessBoardMatrix[0][6] = blackKnight;
    chessBoardMatrix[0][7] = blackRook;

    //set black pawns
    for(j = 0; j < 8; j++)
        chessBoardMatrix[1][j] = blackPawn;

    //set rank's three through six to empty squares (i.e. noPiece)
    for(i = 2; i < 6; i++){
        for(j = 0; j < 8; j++)
            chessBoardMatrix[i][j] = noPiece;
    }

    //set white pawns
    for(j = 0; j < 8; j++)
        chessBoardMatrix[6][j] = whitePawn;

    //set white non-pawn pieces
    chessBoardMatrix[7][0] = whiteRook;
    chessBoardMatrix[7][1] = whiteKnight;
    chessBoardMatrix[7][2] = whiteBishop;
    chessBoardMatrix[7][3] = whiteQueen;
    chessBoardMatrix[7][4] = whiteKing;
    chessBoardMatrix[7][5] = whiteBishop;
    chessBoardMatrix[7][6] = whiteKnight;
    chessBoardMatrix[7][7] = whiteRook;
}

//clears the entire board
void clearEntireBoard(){

    int i, j;

    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++)
            chessBoardMatrix[i][j] = noPiece;
    }
}

//shows the enum value (i.e. integer value) of the pieces on the board
void showchessBoardMatrix(){

    int i, j;

    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++)
            printf("%d,",  chessBoardMatrix[i][j]);

        printf("\n");
    }
}

/*
    This function shows the board with the white pieces represented as follows:
    pawn = p, bishop = b, knight(aka horse) = h, rook = r, queen = q, king = k.
    The black pieces are represented with the same letter, only all in upper case.
*/
void showchessBoard(){

    int i, j;

    for(i = 0; i < 8; i++){

        printf("%d| ", 8 - i);//prints rank followed by left-board boundary

        for(j = 0; j < 8; j++){

            if(chessBoardMatrix[i][j] == whitePawn)
                printf("p ");
            else if(chessBoardMatrix[i][j] == blackPawn)
                printf("P ");
            else if(chessBoardMatrix[i][j] == whiteBishop)
                printf("b ");
            else if(chessBoardMatrix[i][j] == blackBishop)
                printf("B ");
            else if(chessBoardMatrix[i][j] == whiteKnight)
                printf("h ");
            else if(chessBoardMatrix[i][j] == blackKnight)
                printf("H ");
            else if(chessBoardMatrix[i][j] == whiteRook)
                printf("r ");
            else if(chessBoardMatrix[i][j] == blackRook)
                printf("R ");
            else if(chessBoardMatrix[i][j] == whiteQueen)
                printf("q ");
            else if(chessBoardMatrix[i][j] == blackQueen)
                printf("Q ");
            else if(chessBoardMatrix[i][j] == whiteKing)
                printf("k ");
            else if(chessBoardMatrix[i][j] == blackKing)
                printf("K ");
            else
                printf("+ ");
        }

        printf("\n");

    }
        printf("  ----------------\n");//lower board boundary
        printf("   a b c d e f g h\n\n");//files
}

//prints char from of the enum/integer input
void printFile(int file){

    switch(file){

        case a:
            printf("a");
            break;
        case b:
            printf("b");
            break;
        case c:
            printf("c");
            break;
        case d:
            printf("d");
            break;
        case e:
            printf("e");
            break;
        case f:
            printf("f");
            break;
        case g:
            printf("g");
            break;
        default:
            printf("h");
            break;
    }
}

//e.g. will print "e7e5" if black moves his king's pawn up two squares
void printMove(int fromFile, int fromRank, int toFile, int toRank){

    printFile(fromFile);
    printf("%d", 8 - fromRank);// the (8 - fromRank) is correction due to the fact that rows (called "ranks" in chess) are counted bottom-up in chess, and vise-versa in 2D-arrays/matrices
    printFile(toFile);
    printf("%d\n\n", 8 - toRank);
}

void printWhatMoveTheComputerPlayed(){//used at run time

    printf("Computer played: ");//this, (in conjunction with the next statement below) informs the user what move the computer chose to play
    printMove(moveHolderInSemiAlgebraicForm[0], moveHolderInSemiAlgebraicForm[1], moveHolderInSemiAlgebraicForm[2], moveHolderInSemiAlgebraicForm[3]);
}

//this message will be printed at the top of every game-play simulation
void printWelcomingMessage(){

    printf("Instructions: To make a move, simply enter the file-rank coordinates of the two\n");
    printf("respective squares that constitute your move. Please enter them all in lower\n");
    printf("case, and without a space in between. For example, a move from the bottom,\n");
    printf("most left-hand square, to the the upper, most right-hand square, would be: a1h8\n\n");
    printf("Now let's get started! Please enter a 'w' if you would like to play as white,\n");
    printf("or any other character to play as black: ");
}

//returns a random integer between min and max (inclusive)
int randomNumber(int min, int max){
    max -= min - 1;
    return (rand() % max) + min;
}

//this function transfers a coded move (stored in a char array at run time) into an array that stores/holds it an enum/int array
void charToIntConverter(){

    switch(moveHolderInCharForm[0]){

        case 'a':
            moveHolderInSemiAlgebraicForm[0] = a;
            break;
        case 'b':
            moveHolderInSemiAlgebraicForm[0] = b;
            break;
        case 'c':
            moveHolderInSemiAlgebraicForm[0] = c;
            break;
        case 'd':
            moveHolderInSemiAlgebraicForm[0] = d;
            break;
        case 'e':
            moveHolderInSemiAlgebraicForm[0] = e;
            break;
        case 'f':
            moveHolderInSemiAlgebraicForm[0] = f;
            break;
        case 'g':
            moveHolderInSemiAlgebraicForm[0] = g;
            break;
        case 'h':
            moveHolderInSemiAlgebraicForm[0] = h;

    }

    switch(moveHolderInCharForm[1]){

        case '1':
            moveHolderInSemiAlgebraicForm[1] = one;
            break;
        case '2':
            moveHolderInSemiAlgebraicForm[1] = two;
            break;
        case '3':
            moveHolderInSemiAlgebraicForm[1] = three;
            break;
        case '4':
            moveHolderInSemiAlgebraicForm[1] = four;
            break;
        case '5':
            moveHolderInSemiAlgebraicForm[1] = five;
            break;
        case '6':
            moveHolderInSemiAlgebraicForm[1] = six;
            break;
        case '7':
            moveHolderInSemiAlgebraicForm[1] = seven;
            break;
        case '8':
            moveHolderInSemiAlgebraicForm[1] = eight;

    }

    switch(moveHolderInCharForm[2]){

        case 'a':
            moveHolderInSemiAlgebraicForm[2] = a;
            break;
        case 'b':
            moveHolderInSemiAlgebraicForm[2] = b;
            break;
        case 'c':
            moveHolderInSemiAlgebraicForm[2] = c;
            break;
        case 'd':
            moveHolderInSemiAlgebraicForm[2] = d;
            break;
        case 'e':
            moveHolderInSemiAlgebraicForm[2] = e;
            break;
        case 'f':
            moveHolderInSemiAlgebraicForm[2] = f;
            break;
        case 'g':
            moveHolderInSemiAlgebraicForm[2] = g;
            break;
        case 'h':
            moveHolderInSemiAlgebraicForm[2] = h;
    }

    switch(moveHolderInCharForm[3]){

        case '1':
            moveHolderInSemiAlgebraicForm[3] = one;
            break;
        case '2':
            moveHolderInSemiAlgebraicForm[3] = two;
            break;
        case '3':
            moveHolderInSemiAlgebraicForm[3] = three;
            break;
        case '4':
            moveHolderInSemiAlgebraicForm[3] = four;
            break;
        case '5':
            moveHolderInSemiAlgebraicForm[3] = five;
            break;
        case '6':
            moveHolderInSemiAlgebraicForm[3] = six;
            break;
        case '7':
            moveHolderInSemiAlgebraicForm[3] = seven;
            break;
        case '8':
            moveHolderInSemiAlgebraicForm[3] = eight;
    }
}

//this function randomly looks for the first legal move it finds and stores it in the array called "moveHolderInSemiAlgebraicForm"
void randomlyLookForLegalMove(){

    /*
        this function worked perfectly for its original purpose (generating random moves for manual/visual inspection), but it
        is probably much too inefficient for use in a chess engine. Get rid of (or improve) it later.
    */

    int i;

    while(true){

        for (i = 0; i < 4; i++)
            moveHolderInSemiAlgebraicForm[i] = randomNumber(0, 7);

        if(isTheMoveLegal(moveHolderInSemiAlgebraicForm[0], moveHolderInSemiAlgebraicForm[1], moveHolderInSemiAlgebraicForm[2], moveHolderInSemiAlgebraicForm[3]))
            break;
    }
}

//intended for use in the manuallyTestingFuction only
void moveThePieceFromSquare(int file, int rank){

    initialFile = file;
    initialRank = rank;
}

//intended for use in the manuallyTestingFuction only
void moveThePieceToSquare(int file, int rank){

    finalFile = file;
    finalRank = rank;
}

//this function is intended for use in the manuallyTestingFuction only
void checkIfTheMoveIsLegal(){

    bool legal = isTheMoveLegal(initialFile, initialRank, finalFile, finalRank);//store the legality of the move

    printMove(initialFile, initialRank, finalFile, finalRank);//reiterate the move by printing it in a semi algebraic form

    if(legal){

        printf("is a legal move. Here is the board after executing it:\n\n");//print that the was legal and....

        //...show new board set up after move is completed
        int pieceOnSquare = getPiece(initialFile, initialRank);
        setPieceOnSquare(noPiece, initialFile, initialRank);
        setPieceOnSquare(pieceOnSquare, finalFile, finalRank);
        showchessBoard();

    }
    else//i.e. move was not legal by default. Hence print that move was not legal
        printf("is NOT legal!\n");
}

//use this function for testing out specific board positions
void manuallyTestingFuction(){

    srand(time(NULL));//initiate random-generating capabilities

    //decide whose move it is here
    whoseMove = whitesMove;

    //set up the pieces here. Note that you have to spell out the number of the rank (e.g. write "one" instead of "1", "two" instead of "2" etc.)
    setPieceOnSquare(blackQueen, b, four);
    setPieceOnSquare(blackPawn, e, seven);
    setPieceOnSquare(whiteQueen, e, one);
    setPieceOnSquare(whiteKnight, d, five);

    //uncommenting this next line has the same effect as commenting out all the above setPieceOnSquare functions. It's useful if you want to set up a new board position without deleting a prior one
    //clearEntireBoard();

    //this function will show the current board set-up
    printf("Here is the initial position of the pieces:\n\n");
    showchessBoard();

    moveThePieceFromSquare(d, five);//enter the what piece's square your moving FROM here
    moveThePieceToSquare(e, seven);//enter the what piece's square your moving TO here

    //this checks/displays if the move was legal or not
    checkIfTheMoveIsLegal();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////end
