#pragma once

// ------ Includes ------

#include "Pieces.h"

// ------ Defines ------

#define BOARD_WIDTH 7		// Board width in blocks
#define BOARD_HEIGHT 14		// Board height in blocks
#define BOARD_DEPTH 7		// Board depth in blocks
#define PIECE_BLOCKS 5		// number of horizontal and vertical blocks of a matrix piece


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

