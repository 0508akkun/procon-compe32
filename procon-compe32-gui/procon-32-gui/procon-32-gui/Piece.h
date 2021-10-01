#pragma once
#include <Siv3D.hpp>

struct PieceInfo {
	int32 pieceId;
	int32 edgeIndex;
	PieceInfo();
	PieceInfo(int32 pi, int32 ei);
};

class Piece
{
	int32 pieceId;	//�s�[�X���̂�ID
	Array<int32> edgeIndex;	//�[�̃C���f�b�N�X�ԍ�
	Array<Array<Color>> edgePixelData;	//�摜��4�ӂ̒[�̃f�[�^
	Array<std::pair<int32, PieceInfo>> edgeBaseDiff;	//�[�̉�f�Ɗ�̐F�̍������i�[
	Array<int32> edgeDiff;
	Array<std::pair<int32, PieceInfo>> calcEdgeBaseDiff();
	Array<Piece> connectList;

public:
	Piece(int32 pi, Array<int32>& ei, Array<Array<Color>>& epd);
	int32 getPieceId();
	Array<int32> getEdgeIndex();
	Array<Array<Color>> getEdgePixelData();
	Array<std::pair<int32, PieceInfo>> getEdgeBaseDiff();
	Array<int32> calcEdgeDiff(Array<Color> ed);	//	���̒[�Ɣ�r����
};

