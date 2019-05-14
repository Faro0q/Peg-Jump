*/

#include <iostream>
#include <cctype>
using namespace std;
const char* INSTRUCTIONS =  
"Welcome to the peg jump puzzle. The board starts out with a \n"
"single blank position, represented by the '*'.  To make a \n"
"move, select the letter of a peg.  A peg must be able to \n"
"jump over an adjacent peg into a blank position for a move \n"
"to be valid.  The jumped peg is then removed from the board. \n"
"The game is over when there are no valid moves left. At any \n"
"point enter '-' to exit, or '+' for instructions. \n";
const char* startingMessage = 
"Welcome to the peg jump puzzle! \n" 
"Enter '-' to exit, or '+' for instructions.";

const char EMPTY_PIECE = '*';
const int BOARD_SIZE = 121;
const int PRINT_BOARD_ROW_SIZE = 11;
const int PRINT_BOARD_COL_SIZE = 12;
const int PRINT_BOARD_START_DRAWING_INDEX = 27;
const int PRINT_BOARD_END_DRAWING_INDEX = PRINT_BOARD_START_DRAWING_INDEX + 49;
const int PRINT_BOARD_SIZE = PRINT_BOARD_ROW_SIZE * PRINT_BOARD_COL_SIZE;
char printBoard[] = {
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
};
char multipleMovesBoard[] = {
  ' ',' ','*','*','*',' ',' ',
  ' ',' ','*','*','*',' ',' ',
  '*','*','*','*','*','*','*',
  '*','*','*','*','*','*','*',
  '*','*','*','*','*','*','*',
  ' ',' ','*','*','*',' ',' ',
  ' ',' ','*','*','*',' ',' '
};
char startingBoard[] = {
  ' ',' ','A','B','C',' ',' ',
  ' ',' ','D','E','F',' ',' ',
  'G','H','I','J','K','L','M',
  'N','O','P','*','Q','R','S',
  'T','U','V','W','X','Y','Z',
  ' ',' ','1','2','3',' ',' ',
  ' ',' ','4','5','6',' ',' '
};
struct moveData {
  int originalPieceLocation;
  int firstHopLocation;
  int secondHopLocation;
};
//board being displayed
void displayBoard(char board[], char output[]) {
  int gameBoardIndex = 0;
  for(int row = 2; row < 9; row++) {
    int rowIndex = 12 * row;
    for(int col = 2; col < 9; col++) {
      output[rowIndex + col] = board[gameBoardIndex++];
    }
  }
}
//checking the input
bool checkInput(char *inputPtr) 
{
  char input = *inputPtr;
  bool valid = false;
  if(isalpha(input)) {
    valid |= true;
    if(islower(input)) {
      *inputPtr = toupper(input);
    }	
  }
  valid |= isdigit(input);
  return valid;
}
//checking if the piece is oin the board
int isPieceOnBoard(char board[], char piece) 
{
  for(int i = 0; i < BOARD_SIZE; i++)
  {
    if(board[i] == piece) {
      return i;
    }
  }
  return -1;
}
// *** Abhinav: add comments for function
int* squareSearch(char board[], int pieceIndex, bool doubleMove, int valueBuffer[4]) {
  if(pieceIndex < 0 && pieceIndex > 49) {
    // invalid piece index, should have been checked, something went wrong
    return valueBuffer;
  }
  valueBuffer[0] = pieceIndex - (doubleMove ? 14 : 7);
  valueBuffer[1] = pieceIndex - (doubleMove ? 2 : 1);
  valueBuffer[2] = pieceIndex + (doubleMove ? 2 : 1);
  valueBuffer[3] = pieceIndex + (doubleMove ? 14 : 7);
  return valueBuffer;
}
// *** Abhinav: add comments for function
void updatePieces(char board[], int pieceLocation, int searchPieceLocation, int moveLocation) {
  char piece = board[pieceLocation];
  board[moveLocation] = piece;
  board[searchPieceLocation] = EMPTY_PIECE;
  board[pieceLocation] = EMPTY_PIECE;
}
int countPiecesRemaining(char board[]) { // *** Abhinav: add comments for function
  int pieceCount = 0;
  for(int i = 0; i < 49; i++) {
    if(board[i] == EMPTY_PIECE || board[i] == ' ') {
      continue;
    }
    pieceCount++;
  }
  return pieceCount;
}
//moving the piece
int movePiece(char board[], char piece, struct moveData moveInfo[4], bool patch) { // *** Abhinav: need to add comments within this function
  bool validMove = checkInput(&piece);
  int pieceLocation = isPieceOnBoard(board, piece);
  validMove &= pieceLocation != -1;
  if(!validMove) {
    return -1;
  }
  int singleSquareSearch[4] = {};
  int doubleSquareSearch[4] = {};
  squareSearch(board, pieceLocation, false, singleSquareSearch);
  squareSearch(board, pieceLocation, true, doubleSquareSearch);
  int validMoveCount = 0;
  for(int i = 0; i < 4; i++) {
    int searchPieceLocation = singleSquareSearch[i];
    char searchPiece = board[searchPieceLocation];
    if((searchPieceLocation < 0 || searchPieceLocation > 49) ||
	(searchPiece == ' ' || searchPiece == EMPTY_PIECE)) {
      continue;
    }
    int moveLocation = doubleSquareSearch[i];
    if(moveLocation > 0 && moveLocation < 49 && board[moveLocation] == EMPTY_PIECE) {
      validMoveCount++;
      moveInfo[i] = { pieceLocation,
	searchPieceLocation,
	moveLocation };
    } else {
      moveInfo[i] = {-1};
    }
  }
  if(validMoveCount == 0) { 
    return -1;
  }
  int possibleMoves = 1;
  for(int i = 0; i < 4; i++) {
    struct moveData moveInfoAtIndex = moveInfo[i];
    if(moveInfoAtIndex.originalPieceLocation == -1) {
      continue;
    }
    if(validMoveCount == 1) {
      if(patch) {
      updatePieces(board, moveInfoAtIndex.originalPieceLocation, moveInfoAtIndex.firstHopLocation, moveInfoAtIndex.secondHopLocation);
      }
    } else {
      multipleMovesBoard[moveInfoAtIndex.originalPieceLocation] = '+';
      multipleMovesBoard[moveInfoAtIndex.secondHopLocation] = (char) 48 + possibleMoves++;
    }
  }
  return validMoveCount;
}
bool existsValidMove(char board[]) { // *** Abhinav: add comments for function
  struct moveData moveInfo[4] = {{-1},{-1},{-1},{-1}};
  for(int i = 0; i < 49; i++) {
    char piece = board[i];
    if(board[i] == EMPTY_PIECE || board[i] == ' ') {
      continue;
    }
    if(movePiece(board,piece, moveInfo, false) > 0) {
      return true;
    }
  }
  return false;
}

//-------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  int moveCount = 1;

  /* logic:
   *
   * a valid move on the board is possible, if
   * input is a number or letter, and (checkInput(char* ..))
   * the piece is on the board, and (isPieceOnBoard(char .., char[] ..)
   * a jump leads to an empty space, and
   * the jump overtakes an existing piece
   * and the board contains a possible move
   * 
   */
  //Variable declarations

  displayBoard(startingBoard,printBoard);

  cout << startingMessage 
    << printBoard
    << endl;
  bool playing = true;
  // Main loop to play the game
  int mode = 0;
  struct moveData moveInfo[4] = {{-1},{-1},{-1},{-1}};
  while(playing) {
    char input;
    if(mode > 0) {
      moveCount++;
    }
    else if(mode == -1) {
      cout << "*** Invalid move, please retry. ***" << endl;
    } 
    if(mode == 1) {
      cout << printBoard;
    }
    if(mode > 1) {
      displayBoard(multipleMovesBoard,printBoard);
      cout << printBoard;
      cout << "There is more than one possible move. Which move do you want? " ;
      cin >> input;
      int selectionLocation = isPieceOnBoard(multipleMovesBoard,input);
      if(selectionLocation < 0) {
	return 1;
      } else {
	for(int i = 0; i < 4; i++) {
	  struct moveData moveInfoAtIndex = moveInfo[i];
	  if(moveInfoAtIndex.secondHopLocation == selectionLocation) {
	    updatePieces(startingBoard, moveInfoAtIndex.originalPieceLocation, moveInfoAtIndex.firstHopLocation, moveInfoAtIndex.secondHopLocation);
	  }
	}
      }
      displayBoard(startingBoard,printBoard);
      cout << printBoard;
    } 
    else {
      cout << endl << moveCount << ". Enter the peg to move: ";
      cin >> input;
    }
    switch(input) {
      // Check for '-' to exit, and '+' for displaying instructions
      case '+': cout << INSTRUCTIONS;
		break;
      case '-': playing = false;
		break;
      default:  mode = movePiece(startingBoard,input,moveInfo, true);
		break;
    }
    displayBoard(startingBoard,printBoard);
    playing &= countPiecesRemaining(startingBoard) > 1 
      && existsValidMove(startingBoard);
  }// how many pieces reamingng
  cout << endl << "Exiting..." << endl << endl;
  int piecesRemaining = countPiecesRemaining(startingBoard);
  cout << piecesRemaining << " pieces remaining. ";
  if(piecesRemaining >= 5){
    cout << "Ouch!" << endl;
  }
  else if(piecesRemaining == 4){
    cout << "Needs Improvement." << endl;
  }
  else if(piecesRemaining == 3){
    cout << "OK." << endl;
  }
  else if(piecesRemaining == 2){
    cout << "Good!" << endl;
  }
  else if(piecesRemaining == 1){
    cout << "Excellent!" << endl;
  }


  return 0;
}
