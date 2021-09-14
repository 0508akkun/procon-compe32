#pragma once
#include <Siv3D.hpp>

class Piece
{
	int32 pieceId;	//�s�[�X���̂�ID
	Array<int32> edgeIndex;	//�[�̃C���f�b�N�X�ԍ�
	Array<Array<Color>> edgePixelData;	//�摜��4�ӂ̒[�̃f�[�^
	Array<int32> edgeBaseDiff;	//�[�̉�f�Ɗ�̐F�̍������i�[
	Array<int32> edgeDiff;
	Array<int32> calcEdgeBaseDiff();
	Array<Piece> connectList;

public:
	Piece(int32 pi, Array<int32> ei, Array<Array<Color>> epd);
	int32 getPieceId();
	Array<int32> getEdgeIndex();
	Array<Array<Color>> getEdgePixelData();
	Array<int32> getEdgeBaseDiff();
	Array<int32> calcEdgeDiff(Array<Color> ed);	//	���̒[�Ɣ�r����
	void setConnectList();
};

