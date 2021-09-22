#pragma once
#include <Siv3D.hpp>
#include "Piece.h"

class Solver
{
	Array<Piece> board; //どこの端が１つのピースを形成しているか判別する値
	int32 pieceWH;	//ピースの縦横の大きさ
	int32 horDivNum;
	int32 verDivNum;
	Array<int32> graphMemo;	//一度通った事があるか判定
	Array<Array<std::pair<int32, int32>>> resultArray;
	Array<std::pair<int32, PieceInfo>> getBaseDiffList();
	Array<Array<std::pair<int32, int32>>> solveImage();
	Array<Array<Color>> edgePixelData;
	std::tuple<int32, PieceInfo, PieceInfo> searchConnectPiece(Array<std::pair<int32, PieceInfo>>, PieceInfo bl);
	void dfs(Array<std::tuple<int32, PieceInfo, PieceInfo>> cl, int32 index, Array<int32>& gm, Array<Array<std::pair<int32, int32>>>& ra, int32& x, int32& y, int32 rt);
	void moveResultData(const std::tuple<int32, PieceInfo, PieceInfo>& cl, Array<Array<std::pair<int32, int32>>>& ra, int32& x, int32& y, const int32 rt);
public:
	Solver(Image image, int32 WH, int32 hdn, int32 vdn);
};

