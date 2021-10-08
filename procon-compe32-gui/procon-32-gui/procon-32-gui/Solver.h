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
	Array<std::tuple<int32, int32, int32, int32>> resultData;
	Array<std::pair<Array<Color>, PieceInfo>> getBaseDataList();
	Array<Array<Color>> edgePixelData;
	Array<std::tuple<int32, PieceInfo, PieceInfo>> searchConnectPiece(Array<std::pair<Array<Color>, PieceInfo>>& cl, std::pair<Array<Color>, PieceInfo> bl);
	void dfs(Array<std::tuple<int32, PieceInfo, PieceInfo>> cl, int32 pi, Array<int32>& gm, Array<Array<std::pair<int32, int32>>>& ra, int32 x, int32 y, int32 rt);
	void setResultData(int32 pieceId, int32 x, int32 y, int32 rt, Array<std::tuple<int32, int32, int32, int32>>& rd);
	void calcResultData(Array<std::tuple<int32, int32, int32, int32>> rd, Array<Array<std::pair<int32, int32>>>& ra);
	int32 fromRotateConnectPiece(int32 index);
	int32 toRotateConnectPiece(int32 index);
public:
	Solver(Image image, int32 WH, int32 hdn, int32 vdn); 
	Array<Array<std::pair<int32, int32>>> solveImage();
};

