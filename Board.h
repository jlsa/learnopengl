#pragma once

// ------ Includes ------
#include "Defines.h"

#include "Pieces.h"


// ------ Defines ------




class Board
{
public:
	Board(Pieces *pPieces);

	bool IsFreeBlock(int pX, int pY);
	bool IsPossibleMovement(int pX, int pY, int pPiece, int pRotation);
	void StorePiece(int pX, int pY, int pPiece, int pRotation);
	void DeletePossibleLines();
	bool IsGameOver();
	int mBoard[BOARD_WIDTH][BOARD_HEIGHT][BOARD_DEPTH]; // board that contains the pieces
	~Board();

private:

	enum { POS_FREE, POS_FILLED }; // POS_FREE = free position on the board; POS_FILLED = filled position on the board.
	//int mBoard[BOARD_WIDTH][BOARD_HEIGHT][BOARD_DEPTH]; // board that contains the pieces
	Pieces *mPieces;
	
	void InitBoard();
	void DeleteLine(int pY);
};

