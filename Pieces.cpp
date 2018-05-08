#include "Pieces.h"

#include "TetrisShapes.h"

/**
 * Return the type of block (0 = no-block, 1 = normal block, 2 = pivot block)
 */
int Pieces::GetBlockType(int pPiece, int pRotation, int pX, int pY)
{
	return Tetris::Pieces[pPiece][pRotation][pX][pY];
}

/**
 * Returns the horizontal displacement of the piece that has to be applied 
 * in order to create it in the correct position.
 */
int Pieces::GetXInitialPosition(int pPiece, int pRotation)
{
	return Tetris::PiecesInitialPosition[pPiece][pRotation][0];
}

/**
 * Returns the vertical displacement of the piece that has to be applied
 * in order to create it in the correct position.
 */
int Pieces::GetYinitialPosition(int pPiece, int pRotation)
{
	return Tetris::PiecesInitialPosition[pPiece][pRotation][1];
}