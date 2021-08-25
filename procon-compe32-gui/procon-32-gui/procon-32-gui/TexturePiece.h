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
	void setSelectFlag();	//ピースを選択状態にする
	void liftSelectFlag();	//ピースの選択状態を解除する
	void turnRight();	//ピースを右に回転させる
	void turnLeft();	//ピースを左に回転させる
public:
	TexturePiece(Texture texture, int32 x, int32 y, int32 wh);
	TextureRegion showPieceTexture();	//ピースを表示する
	Rect getPiece(int32 y, int32 x);
	bool checkSelectFlag();	//選択されているか確認する
};

