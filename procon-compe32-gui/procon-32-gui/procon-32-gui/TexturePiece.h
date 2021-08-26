#pragma once
#include <Siv3D.hpp>

class TexturePiece
{
	TextureRegion pieceTexture;	//ピースのテクスチャ
	Rect piece;
	bool selectFlag;	//ピースが選択されているか
	int32 rotatedNum;	//回転情報
	int32 pieceWH;	//ピースの縦横の大きさ(正方形)
	int32 x, y;	//ピースの位置
	int32 imageOffsetX = 10;	//端に寄り過ぎると選択した時に色が見えなくなるためオフセットを作成
	int32 imageOffsetY = 10;
public:
	TexturePiece(Texture texture, int32 x, int32 y, int32 wh);
	void turnRight();	//ピースを右に回転させる
	void turnLeft();	//ピースを左に回転させる
	bool getSelectFlag();
	void liftSelectFlag();
	void setSelectFlag();	//ピースを選択状態にする
	TextureRegion getPieceTexture();	//ピースを表示する
	Rect getPiece();
	Vec2 getCoordinate();
	void setCoordinate(Vec2 vec);
};

