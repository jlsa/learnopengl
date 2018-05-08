#pragma once
class Pieces
{
public:
	int GetBlockType(int pPiece, int pRotation, int pX, int pY);
	int GetXInitialPosition(int pPiece, int pRotation);
	int GetYinitialPosition(int pPiece, int pRotation);
};

