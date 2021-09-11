#pragma once
#include <Siv3D.hpp>

class Solver
{
	Array<Array<Color>> edgePixelData;	//�摜�̒[�̃f�[�^
	Array<int32> edgeDiff;	//�[�̉�f�̍�������ꂽ��
	Array<Array<int32>> pieceEdgeIndex; //�ǂ��̒[���P�̃s�[�X���`�����Ă��邩���ʂ���l
	int32 pieceWH;
	Array<std::pair<int32, int32>> calcEdgeDiff();
	int32 searchConnectPiece(int32 index);
public:
	Solver(Image image, int32 WH, int32 horDivNum, int32 verDivNum);
};

