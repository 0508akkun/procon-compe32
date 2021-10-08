#pragma once
#include "TexturePiece.h"

class TextureControl
{
	Array<TexturePiece> board;	//ピース全体の集合
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
	Array<Array<std::pair<int32, int32>>> result;
	bool swapFlag;
	void moveSolverResult(Array<TexturePiece>& bo, Array<Array<std::pair<int32, int32>>>& result);
public:
	TextureControl();
	TextureControl(int32 dami);
	void checkSelectFlag();
	void rotatedPiece(Array<TexturePiece>& bo, Array<int32> swapNum);
	void pieceSwap(Array<TexturePiece>& bo, int32 source, int32 to);
	Array<int32> getPieceID();
	void showBoard();
	void setSolverData();
	void writeResultText();
};

