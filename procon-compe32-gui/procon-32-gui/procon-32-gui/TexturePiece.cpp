#include "TexturePiece.h"

TexturePiece::TexturePiece(Texture texture, int32 id, Vec2 p, int32 wh, int32 iw, int32 ih)
	:
	selectFlag(false),
	pieceID(id),
	rotatedNum(0),
	pos(p),
	pieceWH(wh),
	imageWidth(iw),
	imageHeight(ih)
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
	if (imageHeight >= 640 || imageHeight >= 1280) {
		if (imageHeight >= 1280 || imageHeight >= 2560) {
			piece = Rect(((pieceWH + imageOffsetX) * pos.x) * 0.25 + imageOffsetX, ((pieceWH + imageOffsetY) * pos.y) * 0.25 + imageOffsetY, pieceWH, pieceWH).scaled(0.25).rotated(90_deg * rotatedNum);
		}
		else {
			piece = Rect(((pieceWH + imageOffsetX) * pos.x) * 0.5 + imageOffsetX, ((pieceWH + imageOffsetY) * pos.y) * 0.5 + imageOffsetY, pieceWH, pieceWH).scaled(0.5).rotated(90_deg * rotatedNum);
		}
	}
	else {
		piece = Rect((pieceWH + imageOffsetX) * pos.x + imageOffsetX, (pieceWH + imageOffsetY) * pos.y + imageOffsetY, pieceWH, pieceWH).rotated(90_deg * rotatedNum);
	}
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

int32 TexturePiece::getRotate()
{
	return rotatedNum;
}
