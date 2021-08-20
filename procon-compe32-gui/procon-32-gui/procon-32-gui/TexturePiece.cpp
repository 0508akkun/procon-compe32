#include "TexturePiece.h"

TexturePiece::TexturePiece(Texture texture, int32 x, int32 y, int32 wh)
	:pieceTexture(texture),
	selectFlag(false),
	rotatedNum(0),
	x(x),
	y(y),
	pieceWH(wh)
{
	
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
	return pieceTexture(x, y, pieceWH, pieceWH);
}