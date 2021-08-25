#include "TexturePiece.h"

TexturePiece::TexturePiece(Texture texture, int32 x, int32 y, int32 wh)
	:
	selectFlag(false),
	rotatedNum(0),
	x(x),
	y(y),
	pieceWH(wh)
{
	pieceTexture = texture(x, y, pieceWH, pieceWH);
	piece = Rect();
}

void TexturePiece::setSelectFlag()
{
	selectFlag = true;
}

void TexturePiece::liftSelectFlag()
{
	selectFlag = false;
}

bool TexturePiece::checkSelectFlag()
{
	if (piece.leftClicked()) {
		Print << U"Click";
		setSelectFlag();
	}
	return selectFlag;
}

void TexturePiece::turnRight()
{
	pieceTexture.rotated(90_deg);
	if (rotatedNum < 3) {
		rotatedNum++;
	}
	else {
		rotatedNum = 0;
	}
}

void TexturePiece::turnLeft()
{
	pieceTexture.rotated(-90_deg);
	if (rotatedNum > 0) {
		rotatedNum--;
	}
	else {
		rotatedNum = 3;
	}
}

TextureRegion TexturePiece::showPieceTexture() {
	return pieceTexture;
}

Rect TexturePiece::getPiece(int32 y,int32 x) {
	piece = Rect((pieceWH + 10) * x, (pieceWH + 10) * y, pieceWH, pieceWH);
	return piece;
}