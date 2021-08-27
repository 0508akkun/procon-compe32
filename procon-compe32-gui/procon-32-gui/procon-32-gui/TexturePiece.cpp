#include "TexturePiece.h"

TexturePiece::TexturePiece(Texture texture, int32 id, Vec2 p, int32 wh)
	:
	selectFlag(false),
	pieceID(id),
	rotatedNum(0),
	pos(p),
	pieceWH(wh)
{
	pieceTexture = texture(pos.x * pieceWH, pos.y * pieceWH, pieceWH, pieceWH);
	piece = Quad();
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
		Print << pieceID;
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
	if (rotatedNum < 3) {
		rotatedNum++;
	}
	else {
		rotatedNum = 0;
	}
}

void TexturePiece::turnLeft()
{
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

Quad TexturePiece::getPiece() {
	piece = Rect((pieceWH + 10) * pos.x + imageOffsetX, (pieceWH + 10) * pos.y + imageOffsetY, pieceWH, pieceWH).rotated(90_deg * rotatedNum);
	return piece;
}

Vec2 TexturePiece::getCoordinate() {
	return pos;
}

void TexturePiece::setCoordinate(Vec2 vec) {
	pos = vec;
}

int32 TexturePiece::getPieceID()
{
	return pieceID;
}
