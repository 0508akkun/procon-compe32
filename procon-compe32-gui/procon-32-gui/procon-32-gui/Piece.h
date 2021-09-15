#pragma once
#include <Siv3D.hpp>

class Piece
{
	int32 pieceId;	//ピース自体のID
	Array<int32> edgeIndex;	//端のインデックス番号
	Array<Array<Color>> edgePixelData;	//画像の4辺の端のデータ
	Array<std::pair<int32, int32>> edgeBaseDiff;	//端の画素と基準の色の差分を格納
	Array<int32> edgeDiff;
	Array<std::pair<int32, int32>> calcEdgeBaseDiff();
	Array<Piece> connectList;

public:
	Piece(int32 pi, Array<int32> ei, Array<Array<Color>> epd);
	int32 getPieceId();
	Array<int32> getEdgeIndex();
	Array<Array<Color>> getEdgePixelData();
	Array<std::pair<int32, int32>> getEdgeBaseDiff();
	Array<int32> calcEdgeDiff(Array<Color> ed);	//	他の端と比較する
	void setConnectList();
};

