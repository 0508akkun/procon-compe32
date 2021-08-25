#pragma once
#include "TexturePiece.h"

class TextureControl
{
	Array<TexturePiece> board;	//ピース全体の集合
	Array<int32> pieceID;	//ピースの固有ID(最初の並びがどうなっているか)
	int32 selectCost;	//選択コスト
	int32 changeCost;	//交換コスト
	int32 selectLimitNum;	//選択限界回数
	int32 horDiviNum;	//横の分割数
	int32 verDiviNum;	//縦の分割数
	int32 imageWidth;	//画像自体の幅
	int32 imageHeight;	//画像自体の高さ
	Image image;	//画像そのもの
	int32 pieceWH;
	Texture texture;
public:
	TextureControl();
	void pieceSwap();
	Array<int32> setPieceID();
	void showBoard();
	void monitorState();
};

