#pragma once
#include <Siv3D.hpp>

class Solver
{
	Array<Array<Color>> edgePixelData;	//画像の端のデータ
	Array<int32> edgeDiff;	//端の画素の差分を入れた式
	Array<Array<int32>> pieceEdgeIndex; //どこの端が１つのピースを形成しているか判別する値
	int32 pieceWH;
	Array<std::pair<int32, int32>> calcEdgeDiff();
	int32 searchConnectPiece(int32 index);
public:
	Solver(Image image, int32 WH, int32 horDivNum, int32 verDivNum);
};

