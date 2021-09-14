#pragma once
#include <Siv3D.hpp>
#include "Piece.h"

struct PieceInfo {
	int32 pieceId;
	int32 edgeIndex;
	PieceInfo(int32 pi, int32 ei);
};

class Solver
{
	Array<Piece> board; //どこの端が１つのピースを形成しているか判別する値
	int32 pieceWH;	//ピースの縦横の大きさ
	Array<Array<std::tuple<int32, PieceInfo, PieceInfo>>> searchConnectPiece(int32 index);	//ピース単位で探索
	void connectPiece();
public:
	Solver(Image image, int32 WH, int32 horDivNum, int32 verDivNum);
};

