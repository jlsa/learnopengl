#ifndef _GAME_H_
#define _GAME_H_

// ------ Includes ------

#include "Defines.h"
#include "Board.h"
#include "Pieces.h"
#include <time.h>


struct Piece
{
	int x;
	int y;
	int type;
	int rotation;
};

class Game
{
public:
	Game(Board *pBoard, Pieces *pPieces);
	~Game();

	void Render();
	void CreateNewPiece();

	int mPosX, mPosY;		// position of the piece that is falling down
	int mPiece, mRotation;	// Kind and rotation of the piece that is falling down
};

#endif