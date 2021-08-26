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

bool TexturePiece::getSelectFlag()
{
	return selectFlag;
}

void TexturePiece::liftSelectFlag() {
	selectFlag = false;
}

void TexturePiece::setSelectFlag()
{
	if (piece.leftClicked()) {
		Print << U"Click";
		if (selectFlag) {
			selectFlag = false;
		}
		else{
			selectFlag = true;
		}
	}
}

void TexturePiece::turnRight()
{
	piece.rotated(90_deg);
	if (rotatedNum < 3) {
		rotatedNum++;
	}
	else {
		rotatedNum = 0;
	}
}

void TexturePiece::turnLeft()
{
	piece.rotated(-90_deg);
	if (rotatedNum > 0) {
		rotatedNum--;
	}
	else {
		rotatedNum = 3;
	}
}

TextureRegion TexturePiece::getPieceTexture() {
	return pieceTexture;
}

Rect TexturePiece::getPiece(int32 y,int32 x) {
	piece = Rect((pieceWH + 10) * x + imageOffsetX, (pieceWH + 10) * y + imageOffsetY, pieceWH, pieceWH);
	return piece;
}