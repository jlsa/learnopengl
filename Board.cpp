//#include "TetrisShapes.h"
#include "Board.h"
#include <iostream>




Board::Board(Pieces *pPieces)
{
	mPieces = pPieces;
}


Board::~Board()
{
}


void Board::InitBoard()
{
	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			for (int z = 0; z < BOARD_DEPTH; z++)
			{
				mBoard[x][y][z] = POS_FREE;
			}
		}
	}
}

void Board::StorePiece(int pX, int pY, int pPiece, int pRotation)
{
	/*for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			std::cout << mPieces->GetBlockType(pPiece, pRotation, x, y) << " ";
		}
		std::cout << std::endl;
	}*/

	for (int x1 = pX, x2 = 0; x1 < pX + PIECE_BLOCKS; x1++, x2++)
	{
		for (int y1 = pY, y2 = 0; y1 < pY + PIECE_BLOCKS; y1++, y2++)
		{
			// store only the block of the piece that are not holes
			if (mPieces->GetBlockType(pPiece, pRotation, y2, x2) != 0) // reversed x and y axis
			{
				mBoard[x1][y1][0] = POS_FILLED;
			}
		}
	}

	/*for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			std::cout << mBoard[x][y][0] << " ";
		}
		std::cout << std::endl;
	}*/
}

bool Board::IsGameOver()
{
	// if the first line has blocks, then, the game is over.
	for (int y = 0; y < BOARD_HEIGHT; y++) // for (int x = 0; x < BOARD_WIDTH; x++)
	{
		if (mBoard[0][y][0] == POS_FILLED) // if (mBoard[x][0][0] == POS_FILLED)
		{
			return true;
		}
	}
	return false;
}

/**
 * Delete a line of the board by moving all above lines down
 */
void Board::DeleteLine(int pY)
{
	// move all the upper lines one row down
	for (int y = pY; y > 0; y--)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			mBoard[x][y][0] = mBoard[x][y - 1][0];
		}
	}
}

/**
 * Delete all the lines that should be removed
 * First it checks which should be removed ( the ones that have FILLED horizontal blocks )
 * Then, it uses the DeleteLine method to move all upper rows one row down.
 */
void Board::DeletePossibleLines()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		int x = 0; 
		while (x < BOARD_WIDTH)
		{
			if (mBoard[x][y][0] != POS_FILLED)
			{
				break;
			}
			x++;
		}

		if (x == BOARD_WIDTH)
		{
			DeleteLine(y);
		}
	}
}


/**
 * Returns 1 (true) if this position on the board is empty, 0 if it is filled.
 */
bool Board::IsFreeBlock(int pX, int pY)
{
	if (mBoard[pX][pY][0] == POS_FREE)
	{
		return true;
	}
	return false;
}

/**
 * Check if the piece can be stored at this position without any collision
 * Returns true if the movement is possible, false if it is not.
 */
bool Board::IsPossibleMovement(int pX, int pY, int pPiece, int pRotation)
{
	// check collisions with pieces already stored in the board or the board limits
	// this is just to check the 5x5 blocks of a piece with the appropriate area in the board
	for (int x1 = pX, x2 = 0; x1 < pX + PIECE_BLOCKS; x1++, x2++)
	{
		for (int y1 = pY, y2 = 0; y1 < pY + PIECE_BLOCKS; y1++, y2++)
		{
			// check if the piece is outside the limits of the board
			if (x1 < 0 || x1 > BOARD_WIDTH - 1 || y1 < 0 || y1 > BOARD_HEIGHT - 1)
			{
				if (mPieces->GetBlockType(pPiece, pRotation, y2, y1) != 0) // x and y axis switched
				{
					return false;
				}
			}
			// check if the piece has collisioned with a block that has already been stored on the board
			if (y1 >= 0)
			{
				if ((mPieces->GetBlockType(pPiece, pRotation, y2, x2) != 0) && (!IsFreeBlock(x1, y1))) // x and y axis switched
				{
					return false;
				}
			}
		}
	}
	// we dont have any collisions.
	return true;
}