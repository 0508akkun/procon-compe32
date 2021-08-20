#pragma once
#include <Siv3D.hpp>

class TexturePiece
{
	Texture pieceTexture;
	bool selectFlag;
	int32 rotatedNum;
	int32 pieceWH;
	int32 x, y;
	void setSelectFlag();
	void liftSelectFlag();
	bool checkSelectFlag();
	void turnRight();
	void turnLeft();
public:
	TexturePiece(Texture texture, int32 x, int32 y, int32 wh);
	TextureRegion showPieceTexture();
};

