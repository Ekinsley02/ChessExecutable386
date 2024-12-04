#include "chessUtility.h"

#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#elif defined(__APPLE__)
    #define OS_MAC
#endif

// function definitions



/*
Description: checks if the player can move a piece to block the current check using copied board method
and checks if the current piece has no obstructions.
Input: Current chess board, board row, board column
Output: the given chess piece type
Dependancies: NONE
*/
bool isCheckmate( ChessBoardType **board, char currentTurn, int currentCode )
   {
   // initialize functions/variables
      int rowIndex, colIndex, checkRowIndex = 0, checkColIndex = 0, currentState = MOVING, initialRow, initialCol;
      ChessBoardType **checkedBoard;
      char currentType;
      int targetRow = 0, targetCol = 0;
   checkedBoard = initializeChessBoard( BOARD_SIZE, BOARD_SIZE );
   
   copyBoard( checkedBoard, board );
   
   
   for( rowIndex = 0; rowIndex < BOARD_SIZE; rowIndex++ )
      {
      
      for( colIndex = 0; colIndex < BOARD_SIZE; colIndex++ )
         {
         
         initialRow = rowIndex;
         initialCol = colIndex;
         if( board[ rowIndex ][ colIndex ].side == currentTurn )
            {
            
            currentType = board[ rowIndex ][ colIndex ].type;

            // depending on the piece use every possible combination of currentRow and currentCol
            switch( currentType )
               {
                  
               case PAWN:
               if( checkAllValidPawnPositions( board, checkedBoard, currentTurn, currentType, currentState, initialRow, initialCol, currentCode, &targetRow, &targetCol ) ) 
                  {
                           
                  return true;
                  }
                  break;
                  
               case ROOK:
		         if( checkAllValidRookPositions( board, checkedBoard, checkRowIndex, checkColIndex, rowIndex, colIndex, currentType, currentTurn, initialRow, initialCol, currentState, currentCode, &targetRow, &targetCol ) )
                  {
                        
                  return true;
                  }
		            break; 
                  
               case KNIGHT:
               if( checkAllValidKnightPositions( board, checkedBoard, rowIndex, colIndex, currentType, currentTurn, initialRow, initialCol, currentState, currentCode, &targetRow, &targetCol ) )
                  {
                        
                  return true;
                  }
                  break;
                  
               
               case BISHOP:
               if( checkIfAllValidBishopPositions( board, checkedBoard, checkRowIndex, checkColIndex, rowIndex, colIndex, currentType, currentTurn, initialRow, initialCol, currentState, currentCode, &targetRow, &targetCol ) )
                  {
                           
                  return true;
                  }
                  break;

               case QUEEN:
               if( checkAllValidRookPositions( board, checkedBoard, checkRowIndex, checkColIndex, rowIndex, colIndex, currentType, currentTurn, initialRow, initialCol, currentState, currentCode, &targetRow, &targetCol ) || checkIfAllValidBishopPositions( board, checkedBoard, checkRowIndex, checkColIndex, rowIndex, colIndex, currentType, currentTurn, initialRow, initialCol, currentState, currentCode, &targetRow, &targetCol ) )
                  {
                        
                  return true;
                  }
                  break;
                  
               case KING:
               if( checkAllValidKingPositions( board, checkedBoard, currentTurn, currentType, currentState, initialRow, initialCol, &targetRow, &targetCol ) )
                  {
                  
                  return true;
                  }
                  break;
                  
               }
            }
         }
      }
     
   // clear temporary check board for data leaks
   for( int i = 0; i < BOARD_SIZE; i++ ) 
      {
         
      free( checkedBoard[ i ] );
      }
      
   free( checkedBoard );

   return false;
   }


/*
Description: checks if the given position is in view for battle
and checks if the current piece has no obstructions.
Input: Current chess board, board row, board column
Output: the given chess piece type
Dependancies: NONE
*/
bool checkIfValidPosition( ChessBoardType **board, char currentType,
                           char currentTurn, int initialRow, int initialCol, int currentRow, int currentCol, int *currentState, bool initialPawn )
   {

   // sort the current type
   switch( currentType )
      {
     
     case PAWN:
         if( checkPawnPositions( board, currentTurn, currentRow, currentCol, initialRow, initialCol, currentState, initialPawn ) )
            {
            
            return true;
            }
         break;

      case ROOK:
         if( checkRookPositions(board, currentTurn, currentRow, currentCol, initialRow, initialCol, currentState ) || *currentState == SELECTING )
            {
         
             return true;
            }
         break;

      case KNIGHT:
         // Add logic for knight
         if( checkKnightPositions( board, currentTurn, currentRow, currentCol, initialRow, initialCol, currentState ) || *currentState == SELECTING )
            {
         
            return true;
            }
         break;
      

      case BISHOP:
         // Add logic for bishop
         if( checkBishopPositions( board, currentTurn, currentRow, currentCol, initialRow, initialCol, currentState ) || *currentState == SELECTING )
            {
         
            return true;
            }
         break;

      case QUEEN:
         if( checkBishopPositions( board, currentTurn, currentRow, currentCol, initialRow, initialCol, currentState ) || checkRookPositions( board, currentTurn, currentRow, currentCol, initialRow, initialCol, currentState ) || *currentState == SELECTING  )
            {
         
            return true;
            }
         break;

      case KING:
         if( checkKingPositions( board, currentTurn, currentRow, currentCol, initialRow, initialCol, currentState ) ) 
            {
            
            return true;
            }
         break;
      }
   
   return false;
   }

/*
Description: converts the players column input to an index
Input: players chosen column
Output: the index from the given column
Dependancies: NONE
*/
int columnToIndex( char chosenCol )
   {

   // determine the chosen collum
      // return appropriate index
   switch( chosenCol )
      {

      case 'H':
      case 'h':
         return 0;
         break;
         
      case 'G':
      case 'g':
         return 1;
         break;
         
      case 'F':
      case 'f':
         return 2;
         break;
         
      case 'E':
      case 'e':
         return 3;
         break;
         
      case 'D':
      case 'd':
         return 4;
         break;
         
      case 'C':
      case 'c':
         return 5;
         break;
         
      case 'B':
      case 'b':
         return 6;
         break;
         
      case 'A':
      case 'a':
         return 7;
         break;
         
      default:
         return -1;
         break;
      }

   }

/*
Description: Copies the current board to another board, used for looking into the next move
Input: destination board, source board
Output: copied board from source
Dependancies: NONE
*/
void copyBoard( ChessBoardType **destinationBoard, ChessBoardType **sourceBoard )
   {
   // initialize functions/variables
      int rowIndex, colIndex;
      
   for( rowIndex = 0; rowIndex < BOARD_SIZE; rowIndex++ )
      {
      
      for( colIndex = 0; colIndex < BOARD_SIZE; colIndex++ )
         {
         
         destinationBoard[ rowIndex ][ colIndex ].type = sourceBoard[ rowIndex ][ colIndex ].type;
         destinationBoard[ rowIndex ][ colIndex ].side = sourceBoard[ rowIndex ][ colIndex ].side;
         destinationBoard[ rowIndex ][ colIndex ].highlight = sourceBoard[ rowIndex ][ colIndex ].highlight;
         }
         
      }
   
   }

/*
Description: determines the opposite side guven the current side
Input: currentTurn
Output: The opposite of the current side
Dependancies: NONE
*/
char determineOppositeSide( char currentTurn )
   {
   if( currentTurn == 'P' )
      {
      
      return 'O';
      }
   
   else if( currentTurn == 'O' )
      {
      
      return 'P';
      }
   
   // if not given a valid current turn return X
   return 'X';
   }

/*
Description: determines the sides of the pieces on initial board
Input: Chessboard
Output: The pieces on the board sides identified
Dependancies: NONE
*/
void determineSides( ChessBoardType **board )
   {

   // initialize functions/variables
      int boardRowIndex, boardColIndex;
   
   // set the first 16 chess pieces members to oponent
   for( boardRowIndex = 0; boardRowIndex < 2; boardRowIndex++ )
      {

      for( boardColIndex = 0; boardColIndex < 8; boardColIndex++ )
         {

         board[ boardRowIndex ][ boardColIndex ].side = 'O';
         }
      }

   // set the last 16 chess pieces to player
   for( boardRowIndex = 6; boardRowIndex < 8; boardRowIndex++ )
      {

      for( boardColIndex = 0; boardColIndex < 8; boardColIndex++ )
         {

         board[ boardRowIndex ][ boardColIndex ].side = 'P';
         }

      }

   }
   
/*
Description: finds the piece type of the given board row and column
Input: Current chess board, board row, board column
Output: the given chess piece type
Dependancies: NONE
*/
char determineType( ChessBoardType **board, int boardRow, int boardCol )
   {

   // return the type at given position
   return board[ boardRow][ boardCol ].type;
   }





/*
Description: displays the entire chess board.
Input: given chess board
Output: printed chess board
Dependancies: NONE
*/
ChessBoardType **fillBoard( ChessBoardType **board)
   {
   
      // initilaize functions/variables
      int rowIndex, colIndex;

      // create the initial boards layout
      char initialPieces[] =
      {
       'R', 'G', 'B', 'Q', 'K', 'B', 'G', 'R',
       'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
       'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X',
       'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X',
       'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X',
       'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X',
       'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
       'R', 'G', 'B', 'Q', 'K', 'B', 'G', 'R',
      };

   // fill the board with the appropriate types and NON player
   for( rowIndex = 0; rowIndex < BOARD_SIZE; rowIndex++ )
      {

      for( colIndex = 0; colIndex < BOARD_SIZE; colIndex++ )
         {

         board[ rowIndex ][ colIndex ].type = initialPieces[ rowIndex * BOARD_SIZE + colIndex ];
         board[ rowIndex ][ colIndex ].side = NON_PLAYER;
         board[ rowIndex ][ colIndex ].highlight = false;
         board[ rowIndex ][ colIndex ].hasMoved = false;
         board[ rowIndex ][ colIndex ].castlePos = false;
         }

      }

   // determines the sides of every piece
   determineSides( board );
   return board;
   }

/*
Description: highlights the potential attack positions based on the type
Input: chessboard, givenRow, givenCol
Output: highlighted location
Dependancies: NONE
*/
void highlightAttack( ChessBoardType **board, int initialRow, int initialCol,
                       char currentType, char currentTurn, char highlightFlag, int currentState, bool initialPawn )
   {
  
   // check current battle state
   if( currentState == PASSIVE || currentState == BOTH || currentState == SELECTING )
      {

      // checks if the flag is highlight
      if( highlightFlag == HIGHLIGHT )
         {
            
            switch( currentType )
               {
                  
               case PAWN:
                  if( currentTurn == 'P' )
                     {
                        
                     if( initialRow > 0 && !( putsOwnKingInCheck( board, currentTurn, initialRow, initialCol, initialRow - 1, initialCol ) ) )
                        {
                           
                        board[ initialRow - 1 ][ initialCol ].highlight = true;
                        }
                     // if it is the first move of pawn, highlight the next two
                     if( initialPawn && !( putsOwnKingInCheck( board, currentTurn, initialRow, initialCol, initialRow - 2, initialCol ) ) )
                        {
                        
                        board[ initialRow - 2 ][ initialCol ].highlight = true;
                        }
                     }
                  else if( currentTurn == 'O' )
                     {
                        
                     if( initialRow < BOARD_SIZE - 1 && !( putsOwnKingInCheck( board, currentTurn, initialRow, initialCol, initialRow + 1, initialCol ) ) )
                        {
                           
                        board[ initialRow + 1 ][ initialCol ].highlight = true;
                        }
                     // if it is the first move of pawn, highlight the next two
                     if( initialPawn && !( putsOwnKingInCheck( board, currentTurn, initialRow, initialCol, initialRow + 2, initialCol ) ) )
                        {
                           
                        board[ initialRow + 2 ][ initialCol ].highlight = true;
                        }
                     }
                  break;

            case ROOK:

               highlightRook( board, currentTurn, initialRow, initialCol, highlightFlag );
               break;

            case KNIGHT:

               highlightKnight( board, currentTurn, initialRow, initialCol, highlightFlag );
               break;

            case BISHOP:

               highlightBishop( board, currentTurn, initialRow, initialCol, highlightFlag );
               break;

            case QUEEN:

               highlightBishop( board, currentTurn, initialRow, initialCol, highlightFlag );
               highlightRook( board, currentTurn, initialRow, initialCol, highlightFlag );
               break;

            case KING:
               highlightKing( board, currentTurn, initialRow, initialCol, highlightFlag );
               break;
            }
         }

      // checks if the flag is dehighlight
      else if( highlightFlag == DEHIGHLIGHT ) 
         {
         switch( currentType )
            {

            case PAWN:
               if(  currentTurn == 'P' )
                  {
                  
                  if( initialRow > 0 )
                     {
                  
                     board[ initialRow - 1 ][ initialCol ].highlight = false;
                     }
                  if( initialPawn )
                     {
                  
                     board[ initialRow - 2 ][ initialCol ].highlight = false;
                     }
                  }
               else if( currentTurn == 'O' )
                  {
                  
                  if (initialRow < BOARD_SIZE - 1)
                     {
                  
                     board[ initialRow + 1 ][ initialCol ].highlight = false;
                     }
                  if( initialPawn )
                     {
                  
                     board[ initialRow + 2 ][ initialCol ].highlight = false;
                     }
                  }
               break;

            case ROOK:
               highlightRook( board, currentTurn, initialRow, initialCol, highlightFlag );
               break;

            case KNIGHT:
               highlightKnight( board, currentTurn, initialRow, initialCol, highlightFlag );
               break;

            case BISHOP:
               highlightBishop( board, currentTurn, initialRow, initialCol, highlightFlag );
               break;

            case QUEEN:
               highlightRook( board, currentTurn, initialRow, initialCol, highlightFlag );
               highlightBishop( board, currentTurn, initialRow, initialCol, highlightFlag );
               break;

            case KING:
               highlightKing( board, currentTurn, initialRow, initialCol, highlightFlag );
               break;
            }
         }
      }


   if( currentState == BATTLE || currentState == BOTH )
      {
         
      // checks if the flag is highlight
      if( highlightFlag == HIGHLIGHT )
         {
            
         switch( currentType )
            {
               
            case PAWN:
               if( currentTurn == 'P' && initialRow > 0 && initialCol >= 0 )
                  {
                  
                  // check left edge pieces for battle highlighting
                  if( !( checkEdgePieceLeft( board, initialCol ) ) && board[ initialRow - 1 ][ initialCol + 1 ].side == 'O' )
                     {
                     
                     board[ initialRow - 1 ][ initialCol + 1 ].highlight = true;
                     }
                     
                  // check left edge pieces for battle highlighting
                  if( !( checkEdgePieceRight( board, initialCol ) ) && board[ initialRow - 1 ][ initialCol - 1 ].side == 'O' )
                     {
                  
                     board[ initialRow - 1 ][ initialCol - 1 ].highlight = true;
                     }
                  }
               else if( currentTurn == 'O' && initialRow < BOARD_SIZE - 1 && initialCol >= 0 )
                  {
                     
                  // check left edge pieces for battle highlighting
                  if( !(checkEdgePieceLeft( board, initialCol ) ) && board[ initialRow + 1 ][ initialCol + 1 ].side == 'P' )
                     {
                  
                     board[ initialRow + 1 ][ initialCol + 1 ].highlight = true;
                     }
                     
                  // check right edge pieces for battle highlighting
                  if( !( checkEdgePieceRight( board, initialCol ) ) && board[ initialRow + 1 ][ initialCol - 1 ].side == 'P' )
                     {
                  
                     board[ initialRow + 1 ][ initialCol - 1 ].highlight = true;
                     }
                  }
               break;

            }
         }

         // checks if the flag is dehighlight
      else if( highlightFlag == DEHIGHLIGHT ) 
         {
         switch( currentType )
            {

            case PAWN:
               if( currentTurn == 'P' && initialRow > 0 && initialCol >= 0 )
                  {
                  // check left edge pieces for battle highlighting
                  if( !( checkEdgePieceLeft( board, initialCol ) ) && board[ initialRow - 1 ][ initialCol + 1 ].highlight )
                     {
                        
                     board[ initialRow - 1 ][ initialCol + 1 ].highlight = false;
                     }
                     
                  // check left edge pieces for battle highlighting
                  if( !( checkEdgePieceRight( board, initialCol ) ) && board[ initialRow - 1 ][ initialCol - 1 ].highlight )
                     {

                     board[ initialRow - 1 ][ initialCol - 1 ].highlight = false;
                     }
                  }
               // check left edge pieces for battle highlighting
               else if( currentTurn == 'O' && initialRow < BOARD_SIZE - 1 && initialCol >= 0 )
                  {
                  
                  if( !( checkEdgePieceLeft( board, initialCol ) ) && board[ initialRow + 1 ][ initialCol + 1 ].highlight )
                     {
                  
                     board[ initialRow + 1 ][ initialCol + 1 ].highlight = false;
                     }
                     
                  // check right edge pieces for battle highlighting
                  if( !( checkEdgePieceRight( board, initialCol ) ) && board[ initialRow + 1 ][ initialCol - 1 ].highlight )
                     {
                  
                     board[ initialRow + 1 ][ initialCol - 1 ].highlight = false;
                     }
                  }
               break;
            }
         }
      }
   }  

/*
Description: Allocates memory for the chess board.
Input: number of rows and collumns
Output: allocated memory for the chess board
Dependancies: malloc w/ sizeof
*/
ChessBoardType **initializeChessBoard( )
   {
   // iniitlaize functions/variables
       ChessBoardType **newBoard;
       int index;

   newBoard = ( ChessBoardType ** )malloc( BOARD_SIZE * sizeof( ChessBoardType * ) );

   for( index = 0; index < BOARD_SIZE; index++ )
      {

      newBoard[ index ] = ( ChessBoardType * )malloc
                                           ( BOARD_SIZE * sizeof( ChessBoardType ) );
      }

   return newBoard;
   }

bool isChar( char inputCol )
    {
        
    return isalpha( inputCol );
    }

/*
Description: Checks every possible move of opponent and checks if the king is in scope
Input: chess board, current turn
Output: NONE
Dependancies: determineType
*/
bool isInCheck(ChessBoardType **board, char turn, char checkCode)
   {
    int rowIndex, colIndex;
    char currentType, currentTurn;
    int currentState = CHECK;

    // Iterate over the entire board
    for( rowIndex = 0; rowIndex < BOARD_SIZE; rowIndex++ )
      {
         
        for( colIndex = 0; colIndex < BOARD_SIZE; colIndex++ ) 
           {
              
            currentType = board[ rowIndex ][ colIndex ].type;
            currentTurn = board[ rowIndex ][ colIndex ].side;

            // Only check positions of the specified turn
            if( currentTurn == turn && checkCode == PUTCHECK )
               {
                  
                // Check if the current piece can move to a position that puts the opponent in check
                if( checkIfValidPosition( board, currentType, currentTurn, rowIndex, colIndex, rowIndex, colIndex, &currentState, false ) )
                  {
                     
                    if( currentState == CHECK )
                       {
                          
                        return true;
                       }
                    }
               }
               
            if( checkCode == INCHECK )
               {
                  
               if( checkIfValidPosition( board, currentType, currentTurn, rowIndex, colIndex, rowIndex, colIndex, &currentState, false ) )
                  {
                     
                   if( currentState == CHECK )
                     {
                    
                     return true;
                     }
                  }
               }
           }
      }

    // If no piece can put the opponent in check, return false
    return false;
   }

/*
Description: Uses isCheckMate with the flag stalemate to see if any pieces can move
Input: chess board, currentTurn
Output: if the board is in a stalemate
Dependancies: isCheckMate
*/
bool isStalemate( ChessBoardType **board, char currentTurn )
    {
    
    if( !isCheckmate( board, currentTurn, STALEMATE ) )
        {
        
        return true;
        }
    
    return false;
    }

/*
Description: Handles the move piece event based on whos turn it is and what
piece it is.
Input: chess board
Output: NONE
Dependancies: determineType, pieceHelperPawn
*/
void movePiece( ChessBoardType **board, char currentTurn,
                               int currentRow, int currentCol, int currentState,
                                                int initialRow, int initialCol )
   {
   // initialize functions/variables
      char currentType;

   // determine the type of chess piece played
   currentType = determineType( board, initialRow, initialCol );

   // determine if it is the players turn
      // handle the event of each piece
   switch( currentType )
      {

      case PAWN:
         pieceMoveHelper( board, initialRow, initialCol, currentRow, currentCol, currentType, currentTurn );
         break;
       
      case ROOK:
         pieceMoveHelper( board, initialRow, initialCol, currentRow, currentCol, currentType, currentTurn );
         break;
      
      case KNIGHT:
         pieceMoveHelper( board, initialRow, initialCol, currentRow, currentCol, currentType, currentTurn );
         break;
      
      case BISHOP:
         pieceMoveHelper( board, initialRow, initialCol, currentRow, currentCol, currentType, currentTurn );
         break;
      
      case KING:
         pieceMoveHelper( board, initialRow, initialCol, currentRow, currentCol, currentType, currentTurn );
         break;
      
      case QUEEN:
         pieceMoveHelper( board, initialRow, initialCol, currentRow, currentCol, currentType, currentTurn );
         break;
      }
   
   // mark that the piece has been moved at least once
   board[ currentRow ][ currentCol ].hasMoved = true;
   }


/*
Description: converts the given row into a usable index
Input: initialRow
Output: NONE
Dependancies: NONE
*/
int rowToIndex( int initialRow )
    {
    // initialize functions/variables
        // create a array of rows
        int rowIndexes[ BOARD_SIZE ] = { 7, 6, 5, 4, 3, 2, 1, 0 };
    
    return rowIndexes[ initialRow - 1 ];
    }

/*
Description: creates a new board with the players given move and analyses if it would put the king into check
Input: board, initialRow, initialCol, chosenRow, chosenCol
Output: if the players move will put the king into check
Dependancies: isInCheck
*/
bool putsOpponentKingInCheck(ChessBoardType **board, char currentTurn, int currentRow, int currentCol)
   {
   // initialize functions/variables
      char oppositeTurn;
      
   oppositeTurn = determineOppositeSide( currentTurn );
   
   return isInCheck( board, oppositeTurn, INCHECK );
   }

/*
Description: creates a new board with the players given move and analyses if it would put the king into check
Input: board, initialRow, initialCol, chosenRow, chosenCol
Output: if the players move will put the king into check
Dependancies: isInCheck
*/
bool putsOutOfCheck( ChessBoardType **board, char currentType, int initialRow, int initialCol, int currentRow, int currentCol, char currentTurn, bool initialPawn )
   {
   // intiialize functions/variables
      ChessBoardType **checkedBoard;
   
   int currentState = MOVING;

   checkedBoard = initializeChessBoard( BOARD_SIZE, BOARD_SIZE );
   
   copyBoard( checkedBoard, board );
   
   pieceMoveHelper( checkedBoard, initialRow, initialCol, currentRow, currentCol, currentType, currentTurn );

   if( !isInCheck( checkedBoard, currentTurn, INCHECK ) && currentRow != initialRow && currentCol != initialCol && checkIfValidPosition( checkedBoard, currentType,
                           currentTurn, initialRow, initialCol, currentRow, currentCol, &currentState, initialPawn ))
      {
      
      return true;
      }
   
   return false;
   }
  void aiTeacher(ChessBoardType **board, int *start_row, int *start_col, int *end_row, int *end_col, int code) {
    int bestValue = -10000; // Start with a very low score for AI
    int moveStartRow = -1, moveStartCol = -1, moveEndRow = -1, moveEndCol = -1;
    ChessBoardType **boardCopy;
    char currentType;
    int initialRow = 0, initialCol = 0;
    int currentState = MOVING;

    boardCopy = initializeChessBoard(BOARD_SIZE, BOARD_SIZE);

    // Iterate over the board to find all AI pieces ('O')
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col].side == 'O') {

                initialRow = row;
                initialCol = col;

                currentType = board[row][col].type;

                // Generate all valid moves for the current piece
                switch (currentType) {

                    case PAWN:
                        evaluatePawnMoves(board, boardCopy, initialRow, initialCol, &bestValue, &moveStartRow, &moveStartCol, &moveEndRow, &moveEndCol);
                        break;

                    case KNIGHT:
                        evaluateKnightMoves(board, boardCopy, initialRow, initialCol, &bestValue, &moveStartRow, &moveStartCol, &moveEndRow, &moveEndCol);
                        break;

                    case BISHOP:
                        evaluateBishopMoves(board, boardCopy, initialRow, initialCol, &bestValue, &moveStartRow, &moveStartCol, &moveEndRow, &moveEndCol);
                        break;

                    case ROOK:
                        evaluateRookMoves(board, boardCopy, initialRow, initialCol, &bestValue, &moveStartRow, &moveStartCol, &moveEndRow, &moveEndCol);
                        break;

                    case QUEEN:
                        evaluateQueenMoves(board, boardCopy, initialRow, initialCol, &bestValue, &moveStartRow, &moveStartCol, &moveEndRow, &moveEndCol);
                        break;

                    case KING:
                        evaluateKingMoves(board, boardCopy, initialRow, initialCol, &bestValue, &moveStartRow, &moveStartCol, &moveEndRow, &moveEndCol);
                        break;
                }
            }
        }
    }

    if (moveStartRow == -1) {
        // No valid moves available
        // Handle stalemate or checkmate as needed
    } else {

        *start_row = moveStartRow;
        *start_col = moveStartCol;
        *end_row = moveEndRow;
        *end_col = moveEndCol;
    }

    // Free the allocated memory for boardCopy
    freeBoard(boardCopy);
}

void evaluateBishopMoves(ChessBoardType **board, ChessBoardType **boardCopy, int row, int col, int *bestValue, int *moveStartRow, int *moveStartCol, int *moveEndRow, int *moveEndCol) {
    char currentType = board[row][col].type;
    int currentState = MOVING;
    int currentCode = AI;
    int directions[4][2] = {
        { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 }
    };

    // Generate all possible bishop moves in each diagonal direction
    for (int d = 0; d < 4; d++) {
        int deltaRow = directions[d][0];
        int deltaCol = directions[d][1];
        int targetRow = row + deltaRow;
        int targetCol = col + deltaCol;

        while (targetRow >= 0 && targetRow < BOARD_SIZE && targetCol >= 0 && targetCol < BOARD_SIZE) {
            if (checkIfValidPosition(board, currentType, 'O', row, col, targetRow, targetCol, &currentState, false)) {
                evaluateMove(board, boardCopy, row, col, targetRow, targetCol, currentType, bestValue, moveStartRow, moveStartCol, moveEndRow, moveEndCol);

                // Stop if we capture an opponent's piece
                if (board[targetRow][targetCol].side == 'P') {
                    break;
                }
            } else {
                break; // Blocked by own piece or invalid position
            }

            targetRow += deltaRow;
            targetCol += deltaCol;
        }
    }
}

void evaluateQueenMoves(ChessBoardType **board, ChessBoardType **boardCopy, int row, int col, int *bestValue, int *moveStartRow, int *moveStartCol, int *moveEndRow, int *moveEndCol) {
    // The queen combines the movements of the rook and bishop
    evaluateRookMoves(board, boardCopy, row, col, bestValue, moveStartRow, moveStartCol, moveEndRow, moveEndCol);
    evaluateBishopMoves(board, boardCopy, row, col, bestValue, moveStartRow, moveStartCol, moveEndRow, moveEndCol);
}

void evaluateKingMoves(ChessBoardType **board, ChessBoardType **boardCopy, int row, int col, int *bestValue, int *moveStartRow, int *moveStartCol, int *moveEndRow, int *moveEndCol) {
    int targetRow, targetCol;
    char currentType = board[row][col].type;
    int currentState = MOVING;
    int currentCode = AI;
    int directions[8][2] = {
        { -1, -1 }, { -1, 0 }, { -1, 1 },
        { 0, -1 },           { 0, 1 },
        { 1, -1 },  { 1, 0 },  { 1, 1 }
    };

    // Generate all possible king moves (one square in any direction)
    for (int i = 0; i < 8; i++) {
        targetRow = row + directions[i][0];
        targetCol = col + directions[i][1];

        if (targetRow >= 0 && targetRow < BOARD_SIZE && targetCol >= 0 && targetCol < BOARD_SIZE) {
            if (checkIfValidPosition(board, currentType, 'O', row, col, targetRow, targetCol, &currentState, false)) {
                // Ensure the king does not move into check
                copyBoard(boardCopy, board);
                pieceMoveHelper(boardCopy, row, col, targetRow, targetCol, currentType, 'O');
                if (!isInCheck(boardCopy, 'O', INCHECK)) {
                    evaluateMove(board, boardCopy, row, col, targetRow, targetCol, currentType, bestValue, moveStartRow, moveStartCol, moveEndRow, moveEndCol);
                }
            }
        }
    }
}



void evaluateRookMoves(ChessBoardType **board, ChessBoardType **boardCopy, int row, int col, int *bestValue, int *moveStartRow, int *moveStartCol, int *moveEndRow, int *moveEndCol) {
    char currentType = board[row][col].type;
    int currentState = MOVING;
    int currentCode = AI;
    int directions[4][2] = {
        { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 }
    };

    // Generate all possible rook moves in each straight direction
    for (int d = 0; d < 4; d++) {
        int deltaRow = directions[d][0];
        int deltaCol = directions[d][1];
        int targetRow = row + deltaRow;
        int targetCol = col + deltaCol;

        while (targetRow >= 0 && targetRow < BOARD_SIZE && targetCol >= 0 && targetCol < BOARD_SIZE) {
            if (checkIfValidPosition(board, currentType, 'O', row, col, targetRow, targetCol, &currentState, false)) {
                evaluateMove(board, boardCopy, row, col, targetRow, targetCol, currentType, bestValue, moveStartRow, moveStartCol, moveEndRow, moveEndCol);

                // Stop if we capture an opponent's piece
                if (board[targetRow][targetCol].side == 'P') {
                    break;
                }
            } else {
                break; // Blocked by own piece or invalid position
            }

            targetRow += deltaRow;
            targetCol += deltaCol;
        }
    }
}


void evaluatePawnMoves(ChessBoardType **board, ChessBoardType **boardCopy, int row, int col, int *bestValue, int *moveStartRow, int *moveStartCol, int *moveEndRow, int *moveEndCol) {
    int targetRow, targetCol;
    char currentType = board[row][col].type;
    int currentState = MOVING;
    int currentCode = AI;

    // Generate all valid pawn moves
    // For the AI (assuming it moves down the board), pawns move forward (increasing row index)

    // Move forward
    targetRow = row + 1;
    targetCol = col;

    if (checkIfValidPosition(board, currentType, 'O', row, col, targetRow, targetCol, &currentState, false)) {
        evaluateMove(board, boardCopy, row, col, targetRow, targetCol, currentType, bestValue, moveStartRow, moveStartCol, moveEndRow, moveEndCol);
    }

    // Capture diagonally left
    targetRow = row + 1;
    targetCol = col - 1;
    if (checkIfValidPosition(board, currentType, 'O', row, col, targetRow, targetCol, &currentState, false)) {
        evaluateMove(board, boardCopy, row, col, targetRow, targetCol, currentType, bestValue, moveStartRow, moveStartCol, moveEndRow, moveEndCol);
    }

    // Capture diagonally right
    targetRow = row + 1;
    targetCol = col + 1;
    if (checkIfValidPosition(board, currentType, 'O', row, col, targetRow, targetCol, &currentState, false)) {
        evaluateMove(board, boardCopy, row, col, targetRow, targetCol, currentType, bestValue, moveStartRow, moveStartCol, moveEndRow, moveEndCol);
    }

    // Add initial double move if pawn is at starting position
    if (row == 1) {
        targetRow = row + 2;
        targetCol = col;
        if (checkIfValidPosition(board, currentType, 'O', row, col, targetRow, targetCol, &currentState, true)) {
            evaluateMove(board, boardCopy, row, col, targetRow, targetCol, currentType, bestValue, moveStartRow, moveStartCol, moveEndRow, moveEndCol);
        }
    }
}

void evaluateMove(ChessBoardType **board, ChessBoardType **boardCopy, int startRow, int startCol, int targetRow, int targetCol, char pieceType, int *bestValue, int *moveStartRow, int *moveStartCol, int *moveEndRow, int *moveEndCol) {
    copyBoard(boardCopy, board);

    // Simulate the AI's move
    pieceMoveHelper(boardCopy, startRow, startCol, targetRow, targetCol, pieceType, 'O');

    // Evaluate the board
    int boardValue = evaluateBoard(boardCopy);

    // Add bonuses or penalties based on the move
    boardValue += moveEvaluation(board, startRow, startCol, targetRow, targetCol, pieceType);

    // **Assess Potential Trades**
    int tradePenalty = assessTradeRisk(boardCopy, targetRow, targetCol, 'O', pieceType);
    boardValue -= tradePenalty;

    if (boardValue > *bestValue) {
        *bestValue = boardValue;
        *moveStartRow = startRow;
        *moveStartCol = startCol;
        *moveEndRow = targetRow;
        *moveEndCol = targetCol;
    }
}

int assessTradeRisk(ChessBoardType **board, int pieceRow, int pieceCol, char aiSide, char aiPieceType) {
    char opponentSide = (aiSide == 'O') ? 'P' : 'O';
    int maxPenalty = 0;
    int aiPieceValue = getPieceValue(aiPieceType);

    // Iterate over opponent pieces to find potential threats
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col].side == opponentSide) {
                char opponentPieceType = board[row][col].type;

                if (canCapturePiece(board, row, col, pieceRow, pieceCol, opponentPieceType, opponentSide)) {
                    int opponentPieceValue = getPieceValue(opponentPieceType);
                    int penalty = aiPieceValue - opponentPieceValue;

                    if (penalty > maxPenalty) {
                        maxPenalty = penalty;
                    }
                }
            }
        }
    }

    // Only penalize if the trade is unfavorable
    if (maxPenalty > 0) {
        return maxPenalty * 10; // Adjust multiplier as needed
    }
    return 0; // No significant risk
}


bool isPieceThreatened(ChessBoardType **board, int pieceRow, int pieceCol, char aiSide) {
    char opponentSide = (aiSide == 'O') ? 'P' : 'O';

    // Iterate over all opponent pieces
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col].side == opponentSide) {
                char opponentPieceType = board[row][col].type;

                // Check if the opponent piece can capture the AI's piece
                if (canCapturePiece(board, row, col, pieceRow, pieceCol, opponentPieceType, opponentSide)) {
                    return true;
                }
            }
        }
    }
    return false; // No immediate threat found
}

bool canCapturePiece(ChessBoardType **board, int startRow, int startCol, int targetRow, int targetCol, char pieceType, char side) {
    int currentState = MOVING;
    bool initialPawn = false;

    // Determine if the pawn is in its initial position
    if (pieceType == PAWN) {
        if ((side == 'P' && startRow == 6) || (side == 'O' && startRow == 1)) {
            initialPawn = true;
        }
    }

    // Check if the move is valid for the opponent piece
    if (checkIfValidPosition(board, pieceType, side, startRow, startCol, targetRow, targetCol, &currentState, initialPawn)) {
        // Ensure the target square contains an AI piece
        if (board[targetRow][targetCol].side != side && board[targetRow][targetCol].side != 'X') {
            return true; // The opponent can capture the AI's piece
        }
    }
    return false; // Move is invalid or doesn't result in a capture
}


int moveEvaluation(ChessBoardType **board, int startRow, int startCol, int targetRow, int targetCol, char pieceType) {
    int score = 0;
    ChessBoardType targetSquare = board[targetRow][targetCol];

    // Material gain
    if (targetSquare.side == 'P') {
        int capturedPieceValue = getPieceValue(targetSquare.type);
        score += capturedPieceValue * 10; // Multiply to prioritize captures
    }

    // Positional bonuses
    switch (pieceType) {
        case PAWN:
            // Encourage advancing pawns
            score += (targetRow - startRow) * 1.25; // Adjust as needed
            break;
        case KNIGHT:
        case BISHOP:
            // Encourage developing knights and bishops towards the center
            if ((targetRow >= 2 && targetRow <= 5) && (targetCol >= 2 && targetCol <= 5)) {
                score += 3;
            }
            break;
        case ROOK:
            // Encourage rooks on open files
            if (isOpenFile(board, targetCol)) {
                score += 2;
            }
            break;
        case QUEEN:
            // Be cautious with early queen development
            if (startRow <= 1) {
                score -= 2;
            }
            break;
        case KING:
            // Discourage moving the king unnecessarily
            score -= 5;
            break;
    }

    // Control of the center
    if ((targetRow == 3 || targetRow == 4) && (targetCol == 3 || targetCol == 4)) {
        score += 2; // Bonus for moving to central squares
    }

    // Avoid moving the same piece repeatedly without purpose
    if (startRow == targetRow && startCol == targetCol) {
        score -= 5;
    }

    return score;
}

bool isOpenFile(ChessBoardType **board, int col) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        if (board[row][col].type == PAWN) {
            return false; // File is not open if any pawn is present
        }
    }
    return true;
}


int getPieceValue(char pieceType) {
    switch (pieceType) {
        case PAWN:
            return 1;
        case KNIGHT:
            return 3;
        case BISHOP:
            return 3;
        case ROOK:
            return 5;
        case QUEEN:
            return 9;
        case KING:
            return 1000; // Arbitrary high value
        default:
            return 0;
    }
}

int evaluateBoard(ChessBoardType **board) {
    int score = 0;

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            ChessBoardType piece = board[row][col];

            if (piece.side == 'O') {
                score += getPieceValue(piece.type);
                // Add positional bonuses if desired
            } else if (piece.side == 'P') {
                score -= getPieceValue(piece.type);
                // Subtract positional bonuses if desired
            }
        }
    }

    return score;
}

void evaluateKnightMoves(ChessBoardType **board, ChessBoardType **boardCopy, int row, int col, int *bestValue, int *moveStartRow, int *moveStartCol, int *moveEndRow, int *moveEndCol) {
    int targetRow, targetCol;
    char currentType = board[row][col].type;
    int currentState = MOVING;
    int currentCode = AI;
    int knightMoves[8][2] = {
        { -2, -1 }, { -2, 1 }, { -1, -2 }, { -1, 2 },
        { 1, -2 },  { 1, 2 },  { 2, -1 },  { 2, 1 }
    };

    // Generate all possible knight moves
    for (int i = 0; i < 8; i++) {
        targetRow = row + knightMoves[i][0];
        targetCol = col + knightMoves[i][1];

        if (targetRow >= 0 && targetRow < BOARD_SIZE && targetCol >= 0 && targetCol < BOARD_SIZE) {
            if (checkIfValidPosition(board, currentType, 'O', row, col, targetRow, targetCol, &currentState, false)) {
                evaluateMove(board, boardCopy, row, col, targetRow, targetCol, currentType, bestValue, moveStartRow, moveStartCol, moveEndRow, moveEndCol);
            }
        }
    }
}


void freeBoard(ChessBoardType **board) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        free(board[row]);
    }
    free(board);
}

/*
Description: creates a new board with the players given move and analyses if it would put the king into check
Input: board, initialRow, initialCol, chosenRow, chosenCol
Output: if the players move will put the king into check
Dependancies: isInCheck
*/
bool putsOwnKingInCheck( ChessBoardType **board, char currentTurn, int initialRow, int initialCol, int currentRow, int currentCol )
   {
   // initialize functions/variables
      ChessBoardType **checkedBoard;
      int rowIndex;
      char currentType, oppositeTurn;
      bool result;
   
   checkedBoard = initializeChessBoard( BOARD_SIZE, BOARD_SIZE );
   
   copyBoard( checkedBoard, board );
   
   currentType = determineType( board, initialRow, initialCol );
   
   pieceMoveHelper( checkedBoard, initialRow, initialCol, currentRow, currentCol, currentType, currentTurn );

   oppositeTurn = determineOppositeSide( currentTurn );
   
   result = isInCheck( checkedBoard, oppositeTurn, PUTCHECK );
   
   // Free memory to avoid leaks
   for( rowIndex = 0; rowIndex < BOARD_SIZE; rowIndex++ )
      {
         
      free(checkedBoard[ rowIndex ]);
      }
      
   free( checkedBoard );
   
   return result;
   }



/*
Description: Switches the current turn to the opposite side
Input: current turn
Output: opposite side
Dependancies: NONE
*/
char switchTurn( char currentTurn )
   {

   if( currentTurn == 'P' )
      {

      return 'O';
      }
   else
      {

      return 'P';
      }
   }
