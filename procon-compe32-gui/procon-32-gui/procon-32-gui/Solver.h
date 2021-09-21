#pragma once
#include <Siv3D.hpp>
#include "Piece.h"

class Solver
{
	Array<Piece> board; //どこの端が１つのピースを形成しているか判別する値
	int32 pieceWH;	//ピースの縦横の大きさ
	int32 horDivNum;
	int32 verDivNum;
	Array<Array<int32>> graph;
	Array<int32> graphMemo;	//一度通った事があるか判定
	Array<int32> prevPath;	//過去の経路を保存
	Array<Array<int32>> resultArray;
	Array<std::pair<int32, PieceInfo>> getBaseDiffList();
	Array<std::pair<int32, int32>> classifyPiece();
	Array<Array<Color>> edgePixelData;
	std::tuple<int32, PieceInfo, PieceInfo> searchConnectPiece(Array<std::pair<int32, PieceInfo>>, PieceInfo bl);
	void connectPiece(Array<std::tuple<int32, PieceInfo, PieceInfo>> cl);
	int32 cornerDfs(int32 index, int32 depth, Array<int32>& gm, Array<int32>& pp);
	Array<int32> indexDfs(int32 index, int32 depth, Array<int32>& gm, Array<int32>& pp, Array<int32>& sa);
public:
	Solver(Image image, int32 WH, int32 hdn, int32 vdn);
};

